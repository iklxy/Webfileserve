#pragma once
#include <arpa/inet.h>
#include <string>
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
};