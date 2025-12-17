#include "../include/serve/InetAddress.h"
#include <cstring>

InetAddress::InetAddress(const std::string &ip, uint16_t port)
{
    memset(&addr_, 0, sizeof(addr_));
    addr_.sin_family = AF_INET; // IPv4
    addr_.sin_addr.s_addr = inet_addr(ip.c_str());
    addr_.sin_port = htons(port);
}

InetAddress::~InetAddress()
{
}

const sockaddr_in &InetAddress::get_ip() const
{
    return this->addr_;
}

socklen_t InetAddress::get_addrlen() const
{
    return sizeof(addr_);
}