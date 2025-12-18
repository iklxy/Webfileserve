#include "../include/serve/webserve.h"
#include "../include/http/HttpResponse.h"
#include "../include/http/HttpRequest.h"
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <fstream>
#include <sstream>

std::string getMimeType(const std::string &path)
{
    if (path.find(".html") != std::string::npos)
        return "text/html";
    if (path.find(".css") != std::string::npos)
        return "text/css";
    if (path.find(".js") != std::string::npos)
        return "text/javascript";
    if (path.find(".jpg") != std::string::npos || path.find(".jpeg") != std::string::npos)
        return "image/jpeg";
    if (path.find(".png") != std::string::npos)
        return "image/png";
    return "text/plain";
}

webserve::webserve(const std::string &ip, uint16_t port)
{
    serveSocket = new Socket();
    InetAddress addr(ip, port);
    serveSocket->bind(addr);
    serveSocket->listen();
    std::cout << "webserve start at" << ip << ":" << port << std::endl;

    // 设置文件资源路径
    srcDir = "resources";
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
    HttpRequest request;
    char buffer[4096];

    while (true)
    {
        ssize_t bytesRead = read(clientfd, buffer, sizeof(buffer));
        if (bytesRead <= 0)
        {
            break;
        }
        // 把读取的内容给到状态机
        request.append_buffer(std::string(buffer, bytesRead));
        HttpCode parseStatus = request.parse();
        // 检查状态机是否解析完成
        if (parseStatus == HttpCode::NO_REQUEST)
        {
            continue;
        }
        else if (parseStatus == HttpCode::GET_REQUEST)
        {
            HttpResponse response;
            std::string path = request.get_url();
            if (path == "/")
            {
                path = "/index.html";
            }
            // 检查文件是否存在
            std::string filepath = srcDir + path;
            struct stat fileStat;
            if (stat(filepath.c_str(), &fileStat) < 0)
            {
                response.set_code(404);
                filepath = srcDir + "/404.html";
                break;
            }
            else if (!S_ISREG(fileStat.st_mode)) // 判断是否为普通文件 如果不是普通文件 则返回403
            {
                response.set_code(403);
                filepath = srcDir + "/403.html";
                break;
            }
            else
            {
                response.set_code(200);
            }

            std::fstream file(filepath, std::ios::in | std::ios::binary);
            if (!file.is_open())
            {
                response.set_code(500);
                response.set_body("<html><body><h1>500 Internal Server Error</h1></body></html>");
                break;
            }
            else
            {
                std::stringstream ss;
                ss << file.rdbuf();
                response.set_body(ss.str());
                std::string mimeType = getMimeType(path);
                response.set_header("Content-Type", mimeType);
            }
            file.close();
            // 构建 HTTP 响应
            std::string response_message = response.makeResponse();
            write(clientfd, response_message.c_str(), response_message.size());
            break;
        }
        else
        {
            break;
        }
    }
    close(clientfd);
}