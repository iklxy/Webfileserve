#include "../include/serve/webserve.h"
#include "../include/http/HttpResponse.h"
#include "../include/http/HttpRequest.h"
#include <iostream>
#include <unistd.h>

webserve::webserve(const std::string &ip, uint16_t port)
{
    serveSocket = new Socket();
    InetAddress addr(ip, port);
    serveSocket->bind(addr);
    serveSocket->listen();
    std::cout << "webserve start at" << ip << ":" << port << std::endl;
}

webserve::~webserve()
{
    delete serveSocket;
}

void webserve::start()
{
    while (true)
    {
        InetAddress clientaddr("0.0.0.0", 0);
        int clientfd = serveSocket->accept(clientaddr);
        if (clientfd < 0)
        {
            std::cerr << "accept failed" << std::endl;
            continue;
        }
        std::cout << "new client connected" << std::endl;
        handle_client(clientfd);
    }
}

void webserve::handle_client(int clientfd)
{
    char buffer[1024];

    ssize_t bytesRead = read(clientfd, buffer, sizeof(buffer) - 1);
    if (bytesRead > 0)
    {
        buffer[bytesRead] = '\0';
        std::cout << "Received request:\n"
                  << buffer << std::endl;

        // 构建 HTTP 响应
        HttpRequest request;
        request.parse(buffer);

        std::cout << "收到请求" << request.get_method() << " " << request.get_url() << " " << request.get_version() << std::endl;

        HttpResponse response;
        if (request.get_method() == "GET")
        {
            response.set_code(200);
            response.set_body("<html><body><h1>Welcome to My WebServer!</h1></body></html>");
        }
        /*else
        {
            response.set_code(404);
            response.set_body("<html><body><h1>404 Not Found</h1></body></html>");
        }*/
        std::string response_message = response.makeResponse();

        write(clientfd, response_message.c_str(), response_message.size());
    }
    else if (bytesRead < 0)
    {
        std::cerr << "read failed" << std::endl;
    }

    close(clientfd);
}