#include "../include/serve/webserve.h"
#include <iostream>
#include <signal.h>

int main()
{
    // 忽略SIGPIPE信号，防止写入已关闭的socket导致进程退出
    signal(SIGPIPE, SIG_IGN);

    try
    {
        webserve lxy_serve("192.168.64.5", 8080);
        lxy_serve.start();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}