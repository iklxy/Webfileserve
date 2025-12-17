#pragma once
#include "socket.h"

class webserve
{
    public:
    webserve(const std::string &ip, uint16_t port);
    ~webserve();

    void start();
    private:
    Socket* serveSocket;
    void handle_client(int clientfd);
};