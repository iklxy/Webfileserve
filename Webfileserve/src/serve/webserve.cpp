#include "../include/serve/webserve.h"
#include "../include/serve/config.h" // Need full definition of Config
#include "../include/http/HttpResponse.h"
#include "../include/http/HttpRequest.h"
#include "../include/utils/utils.h"
#include "../include/epoll/Epoll.h"
#include "../include/threadpoll/threadpool.h"
#include "../include/log/log.h"
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <fstream>
#include <sstream>

webserve::webserve(Config &config) : port_(config.PORT), openLinger_(config.OPT_LINGER),
                                     trigMode_(config.TrigMode), timeoutMS_(config.timeoutMS),
                                     threadNum_(config.ThreadNum), sqlConnPoolNum_(config.ConnPoolNum)
{
    serveSocket = new Socket();

    InetAddress addr("0.0.0.0", port_);
    serveSocket->bind(addr);
    serveSocket->listen();

    LOG_INFO("webserve start at port:%d", port_);

    // 设置文件资源路径
    srcDir = "resources";
    // 创建连接数组
    users_ = new HttpConn[65536];
    // 创建线程池
    threadpool_ = new ThreadPool(threadNum_);
    // 创建定时器
    timer_ = new HeapTimer();
}

webserve::~webserve()
{
    delete serveSocket;
    delete[] users_;
    delete epoll_;
    delete threadpool_;
    delete timer_;
}

// 关闭连接的回调
void webserve::CloseConn_(HttpConn *client)
{
    assert(client);
    client->closeConn();
}

void webserve::start()
{
    epoll_ = new Epoll(1024);
    HttpConn::epollfd_ = epoll_->get_epollfd();

    // 将服务器套接字设置为非阻塞状态
    set_nonblocking(serveSocket->get_fd());
    // 将服务器套接字接入epoll
    epoll_->addfd(serveSocket->get_fd(), EPOLLIN | EPOLLET); // 有新连接，且为边缘触发模式
    LOG_INFO("Server is running in Epoll ET mode...");

    while (true)
    {
        // 获取下一次超时时间
        int timeMS = timer_->getNextTick();
        // 等待事件发生 返回事件的数量
        int eventNum = epoll_->wait(timeMS);
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
                            LOG_ERROR("accept failed");
                            break;
                        }
                    }
                    // 接入新客户端后 要将其设置为非阻塞状态
                    set_nonblocking(clientfd);
                    users_[clientfd].init(clientfd, clientaddr.get_ip());

                    // 添加定时器
                    if (clientfd > 0)
                    {
                        timer_->add(clientfd, timeoutMS_, std::bind(&webserve::CloseConn_, this, &users_[clientfd]));
                    }

                    epoll_->addfd(clientfd, EPOLLIN | EPOLLET);
                    LOG_INFO("Client[%d](%s) connected", clientfd, inet_ntoa(clientaddr.get_ip().sin_addr));
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
    int saveErrno = 0;
    ssize_t ret = users_[fd].read(&saveErrno);

    if (ret < 0 && saveErrno != EAGAIN)
    {
        LOG_WARN("Read error, closing connection fd: %d", fd);

        timer_->doWork(fd);
        return;
    }
    else if (ret == 0)
    {
        LOG_INFO("Client closed connection fd: %d", fd);
        timer_->doWork(fd);
        return;
    }
    // 业务逻辑返回真
    if (users_[fd].process())
    {
        // 延长定时器
        timer_->adjust(fd, timeoutMS_);
        // 告诉Epoll改为写事件
        epoll_->modfd(fd, EPOLLOUT);
    }
    else
    {
        // 延长定时器
        timer_->adjust(fd, timeoutMS_);
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
            timer_->doWork(fd);
        }
        else
        {
            // 如果是长连接模式 继续等待读
            // 延长定时器
            timer_->adjust(fd, timeoutMS_);
            users_[fd].process();
            epoll_->modfd(fd, EPOLLIN);
        }
    }
    else if (ret < 0)
    {
        if (saveErrno == EAGAIN)
        {
            // 数据未发送完成 继续等待
            // 延长定时器
            timer_->adjust(fd, timeoutMS_);
            epoll_->modfd(fd, EPOLLOUT);
        }
        else
        {
            LOG_WARN("Write error, closing connection fd: %d", fd);
            timer_->doWork(fd);
        }
    }
    else
    {
        // 写失败 说明数据未发送完成 继续等待
        // 延长定时器
        timer_->adjust(fd, timeoutMS_);
        epoll_->modfd(fd, EPOLLOUT);
    }
    return;
}
