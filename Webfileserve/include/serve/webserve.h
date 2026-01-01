#pragma once
#include "socket.h"
#include "../include/epoll/Epoll.h"
#include "../include/http/HttpConn.h"
#include "../include/threadpoll/threadpool.h"
#include "../include/timer/heaptimer.h"
#include <string>

class Config;

class webserve
{
public:
    // 使用 Config 对象初始化
    webserve(Config &config);
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
    // 线程池
    ThreadPool *threadpool_;
    // 定时器
    HeapTimer *timer_;
    // 默认超时时间 5000 (ms)
    // 应该从 config 中读取
    int timeoutMS_;

    // 端口
    int port_;
    // 是否开启 Linger
    bool openLinger_;
    // 触发模式
    int trigMode_;
    // 数据库连接池数量
    int sqlConnPoolNum_;
    // 线程池数量
    int threadNum_;

    // 子线程处理读事件
    void Onread(int fd);
    // 子线程处理写事件
    void Onwrite(int fd);
    // 关闭连接的回调
    void CloseConn_(HttpConn *client);
};