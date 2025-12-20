#pragma once
#include <fcntl.h> // fcntl

// 设置文件描述符为非阻塞状态
int set_nonblocking(int fd);