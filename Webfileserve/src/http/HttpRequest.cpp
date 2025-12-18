#include "../include/http/HttpRequest.h"
#include <sstream>
#include <iostream>

HttpCode HttpRequest::parse()
{
    HttpCode ret = HttpCode::NO_REQUEST;

    // 处理请求数据
    while (buffer_.size() > 0)
    {
        if (check_state == CheckState::CONTENT)
        {
            if (buffer_.size() >= content_length)
            {
                // 获取需要长度的内容
                std::string content = buffer_.substr(0, content_length);
                // 移除这一行
                buffer_.erase(0, content_length);
                // 解析这一行
                ret = parse_content(content);
                if (ret == HttpCode::BAD_REQUEST)
                    return HttpCode::BAD_REQUEST;
                return ret;
            }
            else
            {
                return HttpCode::NO_REQUEST;
            }
        }

        // 查找换行符 换行符是一行结束的标志
        size_t lineend = buffer_.find("\r\n");
        if (lineend == std::string::npos)
        {
            return HttpCode::NO_REQUEST;
        }
        // 把这一行的内容切出来
        std::string line = buffer_.substr(0, lineend);
        // 移除这一行
        buffer_.erase(0, lineend + 2);
        // 解析这一行
        switch (check_state)
        {
        // 主状态机处于请求行状态
        case CheckState::REQUEST_LINE:
            ret = parse_request_line(line);
            if (ret == HttpCode::BAD_REQUEST)
                return HttpCode::BAD_REQUEST;
            break;
        // 主状态机处于请求头状态
        case CheckState::HEADERS:
            ret = parse_headers(line);
            if (ret == HttpCode::BAD_REQUEST)
                return HttpCode::BAD_REQUEST;
            if (ret == HttpCode::GET_REQUEST)
                return HttpCode::GET_REQUEST;
            break;
        default:
            break;
        }
    }
    return HttpCode::NO_REQUEST;
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
    // 防止请求体过大 导致内存溢出
    if (buffer_.size() > 10 * 1024 * 1024)
    {
        buffer_.clear();
    }
}

HttpCode HttpRequest::parse_request_line(const std::string &line)
{
    // 解析请求行
    std::stringstream request_line(line);
    request_line >> method_ >> url_ >> version_;

    if (method_.empty() || url_.empty() || version_.empty())
        return HttpCode::BAD_REQUEST;

    if (method_ == "POST")
    {
        is_POST = true;
    }

    check_state = CheckState::HEADERS;
    return HttpCode::NO_REQUEST;
}

HttpCode HttpRequest::parse_headers(const std::string &line)
{
    // 解析请求头  如果当前行为空 则根据method的种类判断是否继续解析请求体
    if (line.empty())
    {
        if (content_length > 0)
        {
            check_state = CheckState::CONTENT;
            return HttpCode::NO_REQUEST;
        }
        else
        {
            check_state = CheckState::FINISH;
            return HttpCode::GET_REQUEST;
        }
    }
    // 解析请求头
    // 查找冒号来对每一行内容进行切分
    size_t colon_pos = line.find(':');
    if (colon_pos == std::string::npos)
    {
        return HttpCode::BAD_REQUEST;
    }
    std::string key = line.substr(0, colon_pos);
    colon_pos++;
    while (colon_pos < line.length() && line[colon_pos] == ' ')
    {
        colon_pos++;
    }
    std::string value = line.substr(colon_pos);
    headers_[key] = value;
    if (key == "Content-Length")
    {
        content_length = std::stoi(value);
        if (content_length <= 0)
        {
            content_length = 0;
            return HttpCode::BAD_REQUEST;
        }
    }
    return HttpCode::NO_REQUEST;
}
// 解析请求体
HttpCode HttpRequest::parse_content(const std::string &line)
{
    // 解析请求体
    if (line.size() != content_length)
    {
        return HttpCode::BAD_REQUEST;
    }
    else
    {
        content_ = line;
        check_state = CheckState::FINISH;
        return HttpCode::GET_REQUEST;
    }
}