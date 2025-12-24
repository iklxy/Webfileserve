#pragma once
#include <sys/epoll.h> //epoll
#include <vector>
#include <unistd.h> //用于close

class Epoll
{
public:
    // 防止未定义行为 在一般的单参构造函数中 应该使用explict 除非有充足的理由不使用
    explicit Epoll(int maxevents = 1024);
    ~Epoll();
    bool addfd(int fd, uint32_t events);         // 添加文件描述符到epoll
    bool modfd(int fd, uint32_t events);         // 修改文件描述符的事件
    bool delfd(int fd);                          // 删除文件描述符
    int wait(int timeout = -1);                  // 等待事件发生
    int get_event_fd(int index) const;           // 获取事件发生的文件描述符
    uint32_t get_event_events(int index) const;  // 获取事件发生的事件
    int get_epollfd() const { return epollfd_; } // 获取epoll文件描述符

private:
    int epollfd_;                            // epoll文件描述符
    std::vector<struct epoll_event> events_; // 事件数组
};