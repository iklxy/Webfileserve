#include "../include/http/HttpConn.h"
#include "../include/epoll/Epoll.h"
#include "../include/utils/utils.h"
#include "../include/log/log.h"
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <sys/stat.h>
#include <fstream>
#include <sstream>
#include <string>

std::string getMimeType(const std::string &path)
{
    if (path.find(".html") != std::string::npos)
        return "text/html";
    if (path.find(".css") != std::string::npos)
        return "text/css";
    if (path.find(".js") != std::string::npos)
        return "text/javascript";
    if (path.find(".jpg") != std::string::npos || path.find(".jpeg") != std::string::npos)
        return "image/jpeg";
    if (path.find(".png") != std::string::npos)
        return "image/png";
    return "text/plain";
}

// 静态成员变量初始化
int HttpConn::epollfd_ = -1;
std::string HttpConn::srcDir = "resources";

HttpConn::HttpConn() : fd_(-1), isConnected_(false) {};

HttpConn::~HttpConn()
{
    closeConn();
}

void HttpConn::unmapFile()
{
    if (mmFile_)
    {
        munmap(mmFile_, fileStat_.st_size);
        mmFile_ = nullptr;
    }
}
// 初始化连接
void HttpConn::init(int fd, const struct sockaddr_in &addr)
{
    fd_ = fd;
    addr_ = addr;
    isConnected_ = true;
    writebuffer_.clear();
    request_.init();
    set_nonblocking(fd_);
}

// 关闭连接
void HttpConn::closeConn()
{
    if (isConnected_ == true)
    {
        isConnected_ = false;
        epoll_ctl(epollfd_, EPOLL_CTL_DEL, fd_, 0);
        close(fd_);
        unmapFile();
        LOG_INFO("Client disconnected, fd: %d", fd_);
        fd_ = -1;
    }
}

// read
ssize_t HttpConn::read(int *saveErrno)
{
    ssize_t len = -1;
    ssize_t total_len = 0;
    char buffer[4096];
    while (true)
    {
        len = ::read(fd_, buffer, sizeof(buffer));
        if (len <= 0)
        {
            *saveErrno = errno;
            break;
        }
        // 读取到数据，追加到请求缓冲区
        std::string line(buffer, len);
        request_.append_buffer(line);
        total_len += len;
    }
    // 如果读到了数据，则返回数据长度
    if (total_len > 0)
    {
        return total_len;
    }
    // 如果没读到数据且是EAGAIN，返回-1，且saveErrno为EAGAIN
    if (len == -1 && *saveErrno == EAGAIN)
    {
        return -1;
    }
    return len;
}

// wirte
ssize_t HttpConn::write(int *saveErrno)
{
    ssize_t len = -1;
    while (true)
    {
        len = writev(fd_, iov_, iovCnt_);

        if (len <= 0)
        {
            *saveErrno = errno;
            if (len == -1 && *saveErrno == EAGAIN)
                break; // 缓冲区满，下次再来
            return -1;
        }
        // 检查是否所有数据都发完了
        if (iov_[0].iov_len + iov_[1].iov_len == 0)
        {
            unmapFile(); // 发送完毕，解除映射
            break;
        }

        // 如果这次发送的数据 超过了 头部长度
        if (static_cast<size_t>(len) >= iov_[0].iov_len)
        {
            // 说明头部发完了，Body 发了一部分
            size_t body_sent = len - iov_[0].iov_len; // 算出 Body 发了多少

            // 调整 Body 的指针和长度
            iov_[1].iov_base = (uint8_t *)iov_[1].iov_base + body_sent;
            iov_[1].iov_len -= body_sent;

            // 头部清零
            if (iov_[0].iov_len)
            {
                writebuffer_.clear();
                iov_[0].iov_len = 0;
            }
        }
        else
        {
            iov_[0].iov_base = (uint8_t *)iov_[0].iov_base + len;
            iov_[0].iov_len -= len;
        }
    }
    return len;
}

// process
bool HttpConn::process()
{
    HttpCode parsestatus = request_.parse();

    // 解析失败 需要继续读取内容
    if (parsestatus == HttpCode::NO_REQUEST)
    {
        return false;
    }

    if (parsestatus == HttpCode::GET_REQUEST)
    {
        std::string path = request_.get_url();
        if (path == "/")
        {
            path = "/index.html";
        }
        // 检查文件是否存在
        std::string filepath = srcDir + path;

        if (stat(filepath.c_str(), &fileStat_) < 0)
        {
            response_.set_code(404);
            filepath = srcDir + "/404.html";
            stat(filepath.c_str(), &fileStat_);
        }
        else if (!S_ISREG(fileStat_.st_mode)) // 判断是否为普通文件 如果不是普通文件 则返回403
        {
            response_.set_code(403);
            filepath = srcDir + "/403.html";
            stat(filepath.c_str(), &fileStat_);
        }
        else
        {
            response_.set_code(200);
        }

        int srcFd = open(filepath.c_str(), O_RDONLY);
        if (srcFd < 0)
        {
            // 极其罕见的情况：文件存在但打不开
            response_.set_code(500);
            response_.set_body("<html><body><h1>500 Internal Server Error</h1></body></html>");
            writebuffer_ = response_.makeResponse();
            iov_[0].iov_base = (char *)writebuffer_.c_str();
            iov_[0].iov_len = writebuffer_.size();
            iovCnt_ = 1;
            return true;
        }

        // mmap 零拷贝：将磁盘文件直接映射到内存
        // MAP_PRIVATE 建立写时复制页
        mmFile_ = (char *)mmap(0, fileStat_.st_size, PROT_READ, MAP_PRIVATE, srcFd, 0);
        close(srcFd); // 映射建立后，文件描述符就可以关了

        // 构建响应头
        response_.set_body(""); // Body 不需要放入 string 了
        response_.set_header("Content-Type", getMimeType(path));
        response_.set_header("Content-Length", std::to_string(fileStat_.st_size));

        writebuffer_ = response_.makeResponse();

        // 填充 iovec 结构体 (分散写)
        // 第一块：响应头 (在堆内存/栈内存的 string 里)
        iov_[0].iov_base = (char *)writebuffer_.c_str();
        iov_[0].iov_len = writebuffer_.size();

        // 文件内容 (在 mmap 的内存里)
        iov_[1].iov_base = mmFile_;
        iov_[1].iov_len = fileStat_.st_size;

        iovCnt_ = 2; // 发送两块数据
        return true;
    }
    return false;
}