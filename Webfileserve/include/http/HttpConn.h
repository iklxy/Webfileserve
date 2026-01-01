#pragma once
#include <arpa/inet.h>
#include <string>
#include <sys/uio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "../include/serve/InetAddress.h"
#include "../include/http/HttpRequest.h"
#include "../include/http/HttpResponse.h"
#include "../include/epoll/Epoll.h"

class HttpConn
{
private:
    int fd_;
    sockaddr_in addr_;
    HttpRequest request_;
    HttpResponse response_;
    bool isConnected_;
    std::string writebuffer_;

    char *mmFile_;         // mmap 映射的文件指针
    struct stat fileStat_; // 文件状态信息
    struct iovec iov_[2];  // IO向量：0为Header，1为File Body
    int iovCnt_;           // 当前需要发送的 iovec 数量

public:
    HttpConn();
    ~HttpConn();
    // 初始化
    void init(int fd, const sockaddr_in &addr);
    // 关闭连接
    void closeConn();
    // read
    ssize_t read(int *saveErrno);
    // write
    ssize_t write(int *saveErrno);
    // 业务处理入口
    bool process();
    // 获取socket文件描述符
    int get_fd() const { return fd_; }
    // 所有HttpConn共享一个epoll实例
    static int epollfd_;
    // 根资源目录
    static std::string srcDir;
    // 返回writebuffer的字节
    size_t toWriteBytes() const { return iov_[0].iov_len + iov_[1].iov_len; }
    // 是否保持连接
    bool isKeepAlive() const { return request_.isKeepAlive(); }

    void unmapFile();
};