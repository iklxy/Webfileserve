#pragma once

#include <vector>             //线程池
#include <mutex>              //互斥锁
#include <condition_variable> //条件变量
#include <functional>         //函数对象
#include <thread>             //线程
#include <queue>              //任务队列

class ThreadPool
{
private:
    std::vector<std::thread> threads_;        // 线程池
    std::queue<std::function<void()>> tasks_; // 任务队列

    std::mutex queueMutex_;             // 保护队列的锁
    std::condition_variable condition_; // 条件变量
    bool stop_;                         // 停止标志
public:
    explicit ThreadPool(size_t numThreads = 8) : stop_(false)
    {
        for (size_t i = 0; i < numThreads; ++i)
        {
            threads_.emplace_back([this]()
                                  {
                while (true)
                {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queueMutex_);
                        condition_.wait(lock, [this]()
                        {
                            return stop_ || !tasks_.empty();
                        });
                        if (stop_ && tasks_.empty())
                        {
                            return;
                        }
                        task = std::move(tasks_.front());
                        tasks_.pop();
                    }
                    task();
                } });
            threads_.back().detach(); // 分离线程，线程池中的线程不会阻塞主线程
        }
    }
    ~ThreadPool()
    {
        {
            std::unique_lock<std::mutex> lock(queueMutex_);
            stop_ = true;
        }
        // 唤醒所有睡着的线程，让它们把手头的活干完或者直接退出
        condition_.notify_all();
    }

    template <typename F>
    void addTask(F &&task)
    {
        {
            std::unique_lock<std::mutex> lock(queueMutex_);
            tasks_.emplace(std::forward<F>(task));
        }
        condition_.notify_one(); // 唤醒一个睡着的线程
    }
};