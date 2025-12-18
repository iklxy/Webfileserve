#include "../include/http/HttpResponse.h"
#include <sstream>
#include <iostream>

std::string HttpResponse::code_status()
{
    switch (code_)
    {
    case 200:
        return "OK";
        break;
    case 404:
        return "Not Found";
        break;
    default:
        return "Unknown";
        break;
    }
}

void HttpResponse::set_code(int code)
{
    code_ = code;
}

void HttpResponse::set_body(const std::string &body)
{
    body_ = body;
    // 设置Content-Length头
    headers_["Content-Length"] = std::to_string(body_.length());
}

void HttpResponse::set_header(const std::string &key, const std::string &value)
{
    headers_[key] = value;
}

std::string HttpResponse::makeResponse()
{
    std::stringstream ss;
    ss << "HTTP/1.1 " << code_ << " " << code_status() << "\r\n";
    for (const auto &headers : headers_)
    {
        ss << headers.first << ": " << headers.second << "\r\n";
    }
    ss << "\r\n";
    ss << body_;
    return ss.str();
}

int HttpResponse::get_code() const
{
    return code_;
}
