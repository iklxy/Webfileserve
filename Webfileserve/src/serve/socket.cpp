#include "../include/serve/socket.h"
#include <sys/socket.h> //socker system inteface
#include <netinet/in.h> //address definition
#include <unistd.h>     //for close
#include <stdexcept>    // for exception
#include <cstring>
#include <iostream>

Socket::Socket()
{
    fd_ = socket(AF_INET, SOCK_STREAM, 0); // IPv4,TCP
    if (fd_ < 0)
    {
        throw std::runtime_error("socket create failed");
    }
}

Socket::~Socket()
{
    if (fd_ >= 0)
    {
        close(fd_);
    }
}

void Socket::bind(const InetAddress &addr)
{
    // set port reuse
    int opt = 1;
    if (setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        throw std::runtime_error("socket bind failed");
    }
    if (::bind(fd_, (sockaddr *)&addr.get_ip(), addr.get_addrlen()) < 0)
    {
        throw std::runtime_error("socket bind failed");
    }
}

void Socket::listen()
{
    if (::listen(fd_, SOMAXCONN) == -1)
    {
        throw std::runtime_error("socket listen failed");
    }
}

int Socket::accept(InetAddress &clientaddr)
{
    socklen_t len = clientaddr.get_addrlen();
    int clientfd = ::accept(fd_, (sockaddr *)&clientaddr.get_ip(), &len);
    return clientfd;
}

int Socket::get_fd() const
{
    return this->fd_;
}