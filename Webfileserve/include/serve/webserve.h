#pragma once
#include "socket.h"
#include "../include/epoll/Epoll.h"
#include "../include/http/HttpConn.h"
#include <string>
class webserve
{
public:
    webserve(const std::string &ip, uint16_t port);
    ~webserve();

    // 启动服务器
    void start();

private:
    // 服务器监听的socket
    Socket *serveSocket;
    // 处理客户端请求
    void handle_client(int clientfd);
    // 文件资源路径
    std::string srcDir;
    // epoll
    Epoll *epoll_;
    // 连接数组
    HttpConn *users_;
};