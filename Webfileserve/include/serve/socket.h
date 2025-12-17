#pragma once
#include "InetAddress.h"

class Socket
{
    public:
    Socket();
    ~Socket();

    void bind(const InetAddress &addr);
    void listen();
    //accept return the client socket fd
    int accept(InetAddress &client_addr);
    int get_fd()const;//return the socket id
    private:
    int fd_;//socket id
};
