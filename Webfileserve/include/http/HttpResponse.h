#pragma once
#include <string>
#include <unordered_map>

class HttpResponse
{
private:
    int code_;                                             // 状态码
    std::string body_;                                     // 响应体
    std::unordered_map<std::string, std::string> headers_; // 响应头

public:
    HttpResponse() = default;
    ~HttpResponse() = default;
    void set_code(int code);
    void set_body(const std::string &body);
    void set_header(const std::string &key, const std::string &value);
    std::string code_status(); // 根据状态码设置状态行
    std::string makeResponse();
};