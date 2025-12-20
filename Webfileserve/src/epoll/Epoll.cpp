#include "../include/epoll/Epoll.h"
#include <unistd.h> //用于close
#include <cstring>
#include <iostream>
#include <stdexcept> //异常处理

Epoll::Epoll(int maxevents)
{
    epollfd_ = epoll_create(1); // 创建epoll实例子
    if (epollfd_ < 0)
    {
        throw std::runtime_error("epoll create failed");
    }
    events_.resize(maxevents);
}

Epoll::~Epoll()
{
    if (epollfd_ >= 0)
    {
        close(epollfd_);
    }
}

// 添加文件描述符到epoll
bool Epoll::addfd(int fd, uint32_t events)
{
    if (fd < 0)
        return false;
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = events;
    return epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &ev);
}

// 修改文件描述符号
bool Epoll::modfd(int fd, uint32_t events)
{
    if (fd < 0)
        return false;
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = events;
    return epoll_ctl(epollfd_, EPOLL_CTL_MOD, fd, &ev);
}

// 删除文件描述符
bool Epoll::delfd(int fd)
{
    if (fd < 0)
        return false;
    struct epoll_event ev;
    ev.data.fd = fd;
    return epoll_ctl(epollfd_, EPOLL_CTL_DEL, fd, &ev);
}

// 等待事件发生
int Epoll::wait(int timeout)
{
    return epoll_wait(epollfd_, &events_[0], events_.size(), timeout);
}

// 获取事件发生的文件描述符
int Epoll::get_event_fd(int index) const
{
    if (index < 0 || index >= events_.size())
        return -1;
    return events_[index].data.fd;
}

// 获取事件发生的事件
uint32_t Epoll::get_event_events(int index) const
{
    if (index < 0 || index >= events_.size())
        return 0;
    return events_[index].events;
}