#include "../include/serve/webserve.h"
#include "../include/http/HttpResponse.h"
#include "../include/http/HttpRequest.h"
#include "../include/utils/utils.h"
#include "../include/epoll/Epoll.h"
#include "../include/threadpoll/threadpool.h"
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <fstream>
#include <sstream>

webserve::webserve(const std::string &ip, uint16_t port)
{
    serveSocket = new Socket();
    InetAddress addr(ip, port);
    serveSocket->bind(addr);
    serveSocket->listen();
    std::cout << "webserve start at" << ip << ":" << port << std::endl;

    // 设置文件资源路径
    srcDir = "resources";
    // 创建连接数组
    users_ = new HttpConn[65536];
    // 创建线程池
    threadpool_ = new ThreadPool(8);
}

webserve::~webserve()
{
    delete serveSocket;
    delete[] users_;
    delete epoll_;
    delete threadpool_;
}

// 启动服务 重写为epoll模型
void webserve::start()
{
    epoll_ = new Epoll(1024);
    HttpConn::epollfd_ = epoll_->get_epollfd();

    // 将服务器套接字设置为非阻塞状态
    set_nonblocking(serveSocket->get_fd());
    // 将服务器套接字接入epoll
    epoll_->addfd(serveSocket->get_fd(), EPOLLIN | EPOLLET); // 有新连接，且为边缘触发模式
    std::cout << "Server is running in Epoll ET mode..." << std::endl;

    while (true)
    {
        // 等待事件发生 返回事件的数量
        int eventNum = epoll_->wait(-1);
        for (int i = 0; i < eventNum; i++)
        {
            int fd = epoll_->get_event_fd(i);
            uint32_t events = epoll_->get_event_events(i);

            // 这说明有新连接
            if (fd == serveSocket->get_fd())
            {
                while (true)
                {
                    InetAddress clientaddr("0.0.0.0", 0);
                    int clientfd = serveSocket->accept(clientaddr);
                    if (clientfd < 0)
                    {
                        // EAGAIN错误说明资源不可用 而这在边缘触发模式下是正常的
                        if (errno == EAGAIN || errno == EWOULDBLOCK)
                        {
                            break;
                        }
                        else
                        {
                            std::cerr << "accept failed" << std::endl;
                            break;
                        }
                    }
                    // 接入新客户端后 要将其设置为非阻塞状态
                    set_nonblocking(clientfd);
                    users_[clientfd].init(clientfd, clientaddr.get_ip());
                    epoll_->addfd(clientfd, EPOLLIN | EPOLLET);
                }
            }
            else if (events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
            {
                // 关闭连接
                users_[fd].closeConn();
            }
            else if (events & EPOLLIN) // 说明有数据可读
            {
                // 加入线程池
                threadpool_->addTask(std::bind(&webserve::Onread, this, fd));
            }
            else if (events & EPOLLOUT)
            {
                // 加入线程池
                threadpool_->addTask(std::bind(&webserve::Onwrite, this, fd));
            }
        }
    }
}

// 子线程处理读事件
void webserve::Onread(int fd)
{
    std::cout << "Thread [" << std::this_thread::get_id() << "] handling fd: " << fd << std::endl;
    int saveErrno = 0;
    ssize_t ret = users_[fd].read(&saveErrno);

    // ret < 0 且不是 EAGAIN (虽然read内部处理了EAGAIN返回total_len，但在没有任何数据时可能返回-1且EAGAIN)
    // 或者 ret == 0 (对方关闭连接)
    if (ret < 0 && saveErrno != EAGAIN)
    {
        std::cout << "Read error, closing connection." << std::endl;
        users_[fd].closeConn();
        return;
    }
    else if (ret == 0)
    {
        std::cout << "Client closed connection." << std::endl;
        users_[fd].closeConn();
        return;
    }
    // 业务逻辑返回真
    if (users_[fd].process())
    {
        // 告诉Epoll改为写事件
        epoll_->modfd(fd, EPOLLOUT);
    }
    else
    {
        // 业务逻辑返回假 说明数据还未处理完成 继续等待
        epoll_->modfd(fd, EPOLLIN);
    }
    return;
}

// 子线程处理写事件
void webserve::Onwrite(int fd)
{
    int saveErrno = 0;
    ssize_t ret = users_[fd].write(&saveErrno);
    // 写成功 说明数据已发送完成
    if (users_[fd].toWriteBytes() == 0)
    {
        // 如果是短连接模式 关闭连接
        if (!users_[fd].isKeepAlive())
        {
            users_[fd].closeConn();
        }
        else
        {
            // 如果是长连接模式 继续等待读
            users_[fd].process();
            epoll_->modfd(fd, EPOLLIN);
        }
    }
    else if (ret < 0)
    {
        if (saveErrno == EAGAIN)
        {
            // 数据未发送完成 继续等待
            epoll_->modfd(fd, EPOLLOUT);
        }
        else
        {
            std::cout << "Write error, closing connection." << std::endl;
            users_[fd].closeConn();
        }
    }
    else
    {
        // 写失败 说明数据未发送完成 继续等待
        epoll_->modfd(fd, EPOLLOUT);
    }
    return;
}
