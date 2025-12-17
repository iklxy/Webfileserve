#include "../include/serve/webserve.h"
#include <iostream>
int main()
{
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