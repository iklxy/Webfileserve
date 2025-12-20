#include "../include/serve/webserve.h"
#include "../include/http/HttpResponse.h"
#include "../include/http/HttpRequest.h"
#include "../include/utils/utils.h"
#include "../include/epoll/Epoll.h"
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
    users_ = new HttpConn[65536];
}

webserve::~webserve()
{
    delete serveSocket;
    delete[] users_;
    delete epoll_;
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
            else if (events & EPOLLIN) // 说明有数据可读
            {
                int saveErrno = 0;
                ssize_t ret = users_[fd].read(&saveErrno);

                // ret < 0 且不是 EAGAIN (虽然read内部处理了EAGAIN返回total_len，但在没有任何数据时可能返回-1且EAGAIN)
                // 或者 ret == 0 (对方关闭连接)
                if (ret < 0 && saveErrno != EAGAIN)
                {
                    std::cout << "Read error, closing connection." << std::endl;
                    users_[fd].closeConn();
                }
                else if (ret == 0)
                {
                    std::cout << "Client closed connection." << std::endl;
                    users_[fd].closeConn();
                }
                else
                {
                    // 业务逻辑返回真
                    if (users_[fd].process())
                    {
                        // 发送数据
                        users_[fd].write(&saveErrno);
                        users_[fd].closeConn(); // 短连接模式
                    }
                }
            }
        }
    }
}