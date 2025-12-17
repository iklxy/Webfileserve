#pragma once
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

class InetAddress
{
private:
    struct sockaddr_in addr_;

public:
    InetAddress(const std::string &ip, uint16_t port);
    ~InetAddress();
    const sockaddr_in &get_ip() const;
    socklen_t get_addrlen() const;
};
