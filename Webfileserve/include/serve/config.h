#ifndef CONFIG_H
#define CONFIG_H

#include "webserve.h"

class Config
{
public:
    Config();
    ~Config() {};

    void parse_arg(int argc, char *argv[]); // 核心：解析命令行参数

    // 端口号
    int PORT;

    // 日志相关
    int LOGLevel;
    bool OPT_LINGER;

    // 数据库配置
    int SQLPort;
    const char *SQLUser;
    const char *SQLPwd;
    const char *DBName;
    int ConnPoolNum;

    // 线程池数量
    int ThreadNum;

    // 是否开启日志
    bool openLog;

    // 并发模型选择 (0: Proactor, 1: Reactor)
    // 你的目前实现是 Reactor 模式 (ET + Nonblocking)
    int TrigMode;

    // 超时时间
    int timeoutMS;
};

#endif