#include "../include/serve/webserve.h"
#include "../include/log/log.h"
#include "../include/serve/config.h"
#include <iostream>
#include <signal.h>

int main(int argc, char *argv[])
{
    // 配置类
    Config config;
    config.parse_arg(argc, argv);

    // 初始化日志系统
    Log::Instance()->init(config.LOGLevel, "./log", ".log", 1024);

    if (!config.openLog)
    {
        Log::Instance()->init(0, "./log", ".log", 1024);
    }

    signal(SIGPIPE, SIG_IGN);

    try
    {
        webserve lxy_serve(config);
        lxy_serve.start();
    }
    catch (const std::exception &e)
    {
        LOG_ERROR("Error: %s", e.what());
    }
    return 0;
}