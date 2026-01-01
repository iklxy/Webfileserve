#ifndef LOG_H
#define LOG_H

#include <mutex>
#include <string>
#include <thread>
#include <sys/time.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <sys/stat.h>
#include "blockqueue.h"

class Log
{
public:
    void init(int level, const char *path = "./log",
              const char *suffix = ".log", int maxQueueCapacity = 1024);

    static Log *Instance();
    static void FlushLogThread(); // 后台线程的工作函数

    void write(int level, const char *format, ...); // 写日志的主接口
    void flush();

    int GetLevel();
    void SetLevel(int level);
    bool IsOpen() { return isOpen_; }

private:
    Log();
    virtual ~Log();
    void AppendLogLevelTitle_(int level); // 添加日志等级标签 [INFO]
    void AsyncWrite_();                   // 真正写文件的逻辑

private:
    static const int LOG_PATH_LEN = 256;
    static const int LOG_NAME_LEN = 256;
    static const int MAX_LINES = 50000; // 日志文件最大行数，超过就分文件

    const char *path_;
    const char *suffix_;

    int MAX_LINES_;
    int lineCount_;
    int toDay_;

    bool isOpen_;
    int level_;
    bool isAsync_;

    BlockQueue<std::string> *deque_;

    std::thread *writeThread_; // 后台写线程
    std::mutex mtx_;
    FILE *fp_; // 打开的日志文件指针
};

#define LOG_BASE(level, format, ...)                   \
    do                                                 \
    {                                                  \
        Log *log = Log::Instance();                    \
        if (log->IsOpen() && log->GetLevel() <= level) \
        {                                              \
            log->write(level, format, ##__VA_ARGS__);  \
            log->flush();                              \
        }                                              \
    } while (0);

// 四个等级的宏
#define LOG_DEBUG(format, ...) LOG_BASE(0, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...) LOG_BASE(1, format, ##__VA_ARGS__)
#define LOG_WARN(format, ...) LOG_BASE(2, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) LOG_BASE(3, format, ##__VA_ARGS__)

#endif // LOG_H