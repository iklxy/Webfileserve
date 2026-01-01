#include "../include/log/log.h"
#include <vector>

// 构造函数
Log::Log()
{
    lineCount_ = 0;
    isAsync_ = false;
    writeThread_ = nullptr;
    deque_ = nullptr;
    toDay_ = 0;
    fp_ = nullptr;
    level_ = 0;
}

Log::~Log()
{
    if (writeThread_ && writeThread_->joinable())
    {
        while (deque_ && !deque_->empty())
        {
            deque_->flush(); // 等待队列清空
        };
        deque_->close();      // 关闭队列
        writeThread_->join(); // 等待线程结束
    }
    if (fp_)
    {
        std::lock_guard<std::mutex> locker(mtx_);
        flush();
        fclose(fp_);
    }
    if (deque_)
    {
        delete deque_;
        deque_ = nullptr;
    }
    if (writeThread_)
    {
        delete writeThread_;
        writeThread_ = nullptr;
    }
}

// 获取单例
Log *Log::Instance()
{
    static Log inst;
    return &inst;
}

// 静态函数，作为线程入口
void Log::FlushLogThread()
{
    Log::Instance()->AsyncWrite_();
}

void Log::AsyncWrite_()
{
    std::string str;
    // 从队列里不断取出日志字符串
    while (deque_->pop(str))
    {
        std::lock_guard<std::mutex> locker(mtx_);
        fputs(str.c_str(), fp_);
    }
}

// 初始化
void Log::init(int level, const char *path, const char *suffix, int maxQueueCapacity)
{
    isOpen_ = true;
    level_ = level;
    path_ = path;
    suffix_ = suffix;

    // 如果 capacity > 0，说明要开启异步模式
    if (maxQueueCapacity > 0)
    {
        isAsync_ = true;
        // 创建阻塞队列
        if (!deque_)
        {
            deque_ = new BlockQueue<std::string>(maxQueueCapacity);

            // 创建后台线程
            writeThread_ = new std::thread(FlushLogThread);
        }
    }
    else
    {
        isAsync_ = false;
    }

    lineCount_ = 0;
    time_t t = time(nullptr);
    struct tm *sys_tm = localtime(&t);
    struct tm my_tm = *sys_tm;
    toDay_ = my_tm.tm_mday;

    char fileName[LOG_NAME_LEN] = {0};
    snprintf(fileName, LOG_NAME_LEN - 1, "%s/%04d_%02d_%02d%s",
             path_, my_tm.tm_year + 1900, my_tm.tm_mon + 1, my_tm.tm_mday, suffix_);

    {
        std::lock_guard<std::mutex> locker(mtx_);

        if (fp_)
        {
            flush();
            fclose(fp_);
        }

        fp_ = fopen(fileName, "a");
        if (fp_ == nullptr)
        {
            // 尝试创建目录
            mkdir(path_, 0777);
            fp_ = fopen(fileName, "a");
        }
    }
}

void Log::write(int level, const char *format, ...)
{
    struct timeval now = {0, 0};
    gettimeofday(&now, nullptr);
    time_t tSec = now.tv_sec;
    struct tm *sys_tm = localtime(&tSec);
    struct tm my_tm = *sys_tm;
    va_list vaList;

    // 日志日期 日志行数
    // 如果日期变了，或者行数超了，需要换文件
    if (toDay_ != my_tm.tm_mday || (lineCount_ && (lineCount_ % MAX_LINES == 0)))
    {
        std::unique_lock<std::mutex> locker(mtx_);
        locker.unlock();

        char newFile[LOG_NAME_LEN];
        char tail[36] = {0};
        snprintf(tail, 36, "%04d_%02d_%02d", my_tm.tm_year + 1900, my_tm.tm_mon + 1, my_tm.tm_mday);

        if (toDay_ != my_tm.tm_mday) // 新的一天
        {
            snprintf(newFile, LOG_NAME_LEN - 72, "%s/%s%s", path_, tail, suffix_);
            toDay_ = my_tm.tm_mday;
            lineCount_ = 0;
        }
        else // 同一天，但是超行数了
        {
            snprintf(newFile, LOG_NAME_LEN - 72, "%s/%s-%d%s", path_, tail, (lineCount_ / MAX_LINES), suffix_);
        }

        locker.lock();
        flush();
        if (fp_)
        {
            fclose(fp_);
        }
        fp_ = fopen(newFile, "a");
        locker.unlock();
    }

    {
        std::unique_lock<std::mutex> locker(mtx_);
        lineCount_++;
        int n = 0;

        // 格式：2023-10-01 12:00:00.123456 [INFO]:
        int bufSize = 256;
        char buffer[bufSize]; // 栈上分配一个buffer先存头部

        n = snprintf(buffer, bufSize, "%d-%02d-%02d %02d:%02d:%02d.%06ld ",
                     my_tm.tm_year + 1900, my_tm.tm_mon + 1, my_tm.tm_mday,
                     my_tm.tm_hour, my_tm.tm_min, my_tm.tm_sec, now.tv_usec);

        std::string s;
        switch (level)
        {
        case 0:
            s = "[DEBUG]: ";
            break;
        case 1:
            s = "[INFO] : ";
            break;
        case 2:
            s = "[WARN] : ";
            break;
        case 3:
            s = "[ERROR]: ";
            break;
        default:
            s = "[INFO] : ";
            break;
        }

        // 组合头部
        std::string log_str = std::string(buffer) + s;

        // 处理变长参数
        va_start(vaList, format);
        int m = vsnprintf(nullptr, 0, format, vaList);
        va_end(vaList);

        if (m > 0)
        {
            std::vector<char> buf(m + 1);
            va_start(vaList, format);
            vsnprintf(buf.data(), m + 1, format, vaList);
            va_end(vaList);
            log_str.append(buf.data(), m);
        }

        log_str += "\n";

        if (isAsync_ && deque_ && !deque_->full())
        {
            deque_->push_back(log_str);
        }
        else
        {
            fputs(log_str.c_str(), fp_);
        }
    }
}

void Log::flush()
{
    if (isAsync_)
    {
        deque_->flush();
    }
    fflush(fp_);
}

int Log::GetLevel()
{
    return level_;
}

void Log::SetLevel(int level)
{
    level_ = level;
}

void Log::AppendLogLevelTitle_(int level)
{
}