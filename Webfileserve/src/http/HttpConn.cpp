#include "../include/http/HttpConn.h"
#include "../include/epoll/Epoll.h" // 需要用到 epoll_ctl
#include "../include/utils/utils.h" // setNonBlocking
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

// 初始化连接
void HttpConn::init(int fd, const struct sockaddr_in &addr)
{
    fd_ = fd;
    addr_ = addr;
    isConnected_ = true;
    writebuffer_.clear();
    request_.init();
    // response_.init(); // HttpResponse目前比较简单，每次process都会重新设置，暂时可以不重置
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
        std::cout << "Client disconnected, fd: " << fd_ << std::endl;
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
        std::string line(buffer, len); // 修正：只构造实际读取长度的string
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
    const char *buffer = writebuffer_.c_str();
    // 用于记录发送了多少数据
    size_t total_len = 0;

    while (total_len < writebuffer_.size())
    {
        len = ::write(fd_, buffer + total_len, writebuffer_.size() - total_len);
        if (len == -1)
        {
            *saveErrno = errno;
            if (*saveErrno == EAGAIN)
            {
                // 说明资源不可用 而这在边缘触发模式下是正常的 先不考虑 先退出
                break;
            }
            return -1;
        }
        // 擦除已经发送的数据
        writebuffer_.erase(0, len);
        // 发送数据成功 记录已经发送的数据
        total_len += len;
    }
    return total_len;
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
        struct stat fileStat;
        if (stat(filepath.c_str(), &fileStat) < 0)
        {
            response_.set_code(404);
            filepath = srcDir + "/404.html";
        }
        else if (!S_ISREG(fileStat.st_mode)) // 判断是否为普通文件 如果不是普通文件 则返回403
        {
            response_.set_code(403);
            filepath = srcDir + "/403.html";
        }
        else
        {
            response_.set_code(200);
        }

        std::fstream file(filepath, std::ios::in | std::ios::binary);
        if (!file.is_open())
        {
            response_.set_code(500);
            response_.set_body("<html><body><h1>500 Internal Server Error</h1></body></html>");
        }
        else
        {
            std::stringstream ss;
            ss << file.rdbuf();
            response_.set_body(ss.str());
            std::string mimeType = getMimeType(path);
            response_.set_header("Content-Type", mimeType);
        }
        file.close();
        // 构建 HTTP 响应
        writebuffer_ = response_.makeResponse();
        return true;
    }
    return false;
}