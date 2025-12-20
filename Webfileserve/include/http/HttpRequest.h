#pragma once
#include <string>
#include <unordered_map>

// 主状态机的状态
enum class CheckState
{
    REQUEST_LINE,
    HEADERS,
    CONTENT,
    FINISH
};

enum class HttpCode
{
    NO_REQUEST,  // 请求不完整，需要继续读取
    GET_REQUEST, // 获得了完整的请求
    BAD_REQUEST, // 请求有语法错误
    NO_RESOURCE, // 404
    FORBIDDEN_REQUEST,
    FILE_REQUEST,
    INTERNAL_ERROR,
    CLOSED_CONNECTION
};

class HttpRequest
{
private:
    // 请求行的内容
    std::string method_;
    std::string url_;
    std::string version_;
    std::unordered_map<std::string, std::string> headers_; // 存储请求头
    std::string content_;                                  // 存储请求体

    CheckState check_state = CheckState::REQUEST_LINE; // 当前状态机的状态
    std::string buffer_;                               // 存储请求数据的缓冲区

    size_t content_length = 0; // 存储请求体的长度
    bool is_POST = false;      // 是否为POST请求

public:
    HttpRequest() = default;
    ~HttpRequest() = default;

    void init(); // 重置状态
    HttpCode parse();
    std::string get_method() const { return method_; }
    std::string get_url() const { return url_; }
    std::string get_version() const { return version_; }
    std::string get_header(const std::string &key) const;

    void append_buffer(const std::string &data);
    // 解析请求行
    HttpCode parse_request_line(const std::string &line);
    // 解析请求头
    HttpCode parse_headers(const std::string &line);
    // 解析请求体
    HttpCode parse_content(const std::string &line);
};
