#include "../include/http/HttpRequest.h"
#include <sstream>
#include <iostream>

HttpCode HttpRequest::parse()
{
    HttpCode ret = HttpCode::NO_REQUEST;

    // 处理请求数据
    while (buffer_.size() > 0)
    {
    }
    /*
    // 存储请求头
        std::stringstream ss(request);
    // 读取请求行
    std::string line;
    std::getline(ss, line);
    // 解析请求行
    std::istringstream request_line(line);
    request_line >> method_ >> url_ >> version_;
    if (url_ == "/")
    {
        url_ = "index.html";
    }

    while (std::getline(ss, line) && line != "\r")
    {
        // 解析请求头
        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }
        size_t colon_pos = line.find(':');
        if (colon_pos != std::string::npos)
        {
            std::string key = line.substr(0, colon_pos);
            colon_pos++;
            if (colon_pos < line.length() && line[colon_pos] == ' ')
            {
                colon_pos++;
            }
            std::string value = line.substr(colon_pos);
            headers_[key] = value;
        }
    }*/
}

std::string HttpRequest::get_header(const std::string &key) const
{
    auto it = headers_.find(key);
    if (it != headers_.end())
    {
        // 返回value
        return it->second;
    }
    return "";
}

void HttpRequest::append_buffer(const std::string &data)
{
    buffer_ += data;
}