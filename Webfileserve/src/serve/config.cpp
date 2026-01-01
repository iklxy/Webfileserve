#include "../../include/serve/config.h"
#include <unistd.h> // for getopt
#include <stdlib.h> // for atoi

Config::Config()
{
    // --- 默认配置 ---
    PORT = 8080;

    // 日志配置
    LOGLevel = 1;
    OPT_LINGER = false;
    openLog = true;

    // 数据库默认配置
    SQLPort = 3306;
    SQLUser = "root";
    SQLPwd = "your_password";
    DBName = "webdb";
    ConnPoolNum = 12;

    // 线程池
    ThreadNum = 6;

    // 触发模式 (默认 ET + ET)
    TrigMode = 3;

    // 超时时间 (默认60秒)
    timeoutMS = 60000;
}

void Config::parse_arg(int argc, char *argv[])
{
    int opt;
    const char *str = "p:l:m:o:s:t:c:a:";
    // p: 端口
    // l: 日志等级
    // m: 触发模式
    // o: Linger
    // s: 数据库端口
    // t: 线程数
    // c: 是否开启日志
    // a: 数据库连接池数量

    while ((opt = getopt(argc, argv, str)) != -1)
    {
        switch (opt)
        {
        case 'p':
            PORT = atoi(optarg);
            break;
        case 'l':
            LOGLevel = atoi(optarg);
            break;
        case 'm':
            TrigMode = atoi(optarg);
            break;
        case 'o':
            OPT_LINGER = atoi(optarg);
            break;
        case 's':
            SQLPort = atoi(optarg);
            break;
        case 't':
            ThreadNum = atoi(optarg);
            break;
        case 'c':
            openLog = atoi(optarg);
            break;
        case 'a':
            ConnPoolNum = atoi(optarg);
            break;
        default:
            break;
        }
    }
}