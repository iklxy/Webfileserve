#ifndef HEAP_TIMER_H
#define HEAP_TIMER_H

#include <queue>
#include <unordered_map>
#include <time.h>
#include <algorithm>
#include <arpa/inet.h>
#include <functional>
#include <assert.h>
#include <chrono>
#include <mutex>
#include "../log/log.h"

// 定义回调函数类型 (用来关闭连接)
typedef std::function<void()> TimeoutCallBack;
// 定义时钟类型 (使用高精度单调时钟)
typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds MS;
typedef Clock::time_point TimeStamp;

struct TimerNode
{
    int id;
    TimeStamp expires;  // 超时时刻
    TimeoutCallBack cb; // 超时后的回调函数

    // 重载 < 运算符，用于堆排序
    bool operator<(const TimerNode &t)
    {
        return expires < t.expires;
    }
};

class HeapTimer
{
public:
    HeapTimer() { heap_.reserve(64); }
    ~HeapTimer() { clear(); }

    // 调整定时器 (续命)
    void adjust(int id, int timeout);

    // 添加定时器
    void add(int id, int timeOut, const TimeoutCallBack &cb);

    // 删除指定定时器 (即使没超时也删，比如用户主动断开)
    void doWork(int id);

    // 清理所有
    void clear();

    // 处理过期的定时器 (核心心跳函数)
    void tick();

    // 还有多久发生下一次超时 (用于 epoll_wait 的 timeout)
    int getNextTick();

    // 辅助: 弹出一个定时器
    void pop();

private:
    void del_(size_t i);                    // 删除指定位置的节点
    void siftup_(size_t i);                 // 向上调整
    bool siftdown_(size_t index, size_t n); // 向下调整
    void swapNode_(size_t i, size_t j);     // 交换节点

    std::vector<TimerNode> heap_; // 堆本体 (数组实现)

    std::unordered_map<int, size_t> ref_;

    std::mutex mtx_; // 互斥锁，保证线程安全
};

#endif // HEAP_TIMER_H