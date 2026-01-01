#ifndef BLOCKQUEUE_H
#define BLOCKQUEUE_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include <sys/time.h>

template <class T>
class BlockQueue
{
public:
    explicit BlockQueue(size_t maxCapacity = 1000);
    ~BlockQueue();

    void clear();
    bool empty();
    bool full();
    void close(); // 关闭队列

    size_t size();
    size_t capacity();

    T front();
    T back();

    void push_back(const T &item);
    bool pop(T &item);              // 弹出队头元素
    bool pop(T &item, int timeout); // 带超时的弹出

    void flush(); // 通知消费者

private:
    std::deque<T> deq_;                    // 底层数据结构
    size_t capacity_;                      // 容量
    std::mutex mtx_;                       // 互斥锁
    bool isClose_;                         // 队列关闭标志
    std::condition_variable condConsumer_; // 消费者条件变量 (队列不空时通知)
    std::condition_variable condProducer_; // 生产者条件变量 (队列不满时通知)
};

template <class T>
BlockQueue<T>::BlockQueue(size_t maxCapacity) : capacity_(maxCapacity)
{
    assert(maxCapacity > 0);
    isClose_ = false;
}

template <class T>
BlockQueue<T>::~BlockQueue()
{
    close();
}

template <class T>
void BlockQueue<T>::close()
{
    {
        std::lock_guard<std::mutex> locker(mtx_);
        deq_.clear();
        isClose_ = true;
    }
    condProducer_.notify_all();
    condConsumer_.notify_all();
}

template <class T>
void BlockQueue<T>::clear()
{
    std::lock_guard<std::mutex> locker(mtx_);
    deq_.clear();
}

template <class T>
bool BlockQueue<T>::empty()
{
    std::lock_guard<std::mutex> locker(mtx_);
    return deq_.empty();
}

template <class T>
bool BlockQueue<T>::full()
{
    std::lock_guard<std::mutex> locker(mtx_);
    return deq_.size() >= capacity_;
}

template <class T>
void BlockQueue<T>::push_back(const T &item)
{
    std::unique_lock<std::mutex> locker(mtx_);

    while (deq_.size() >= capacity_)
    {
        condProducer_.wait(locker);
    }
    deq_.push_back(item);
    condConsumer_.notify_one();
}

template <class T>
bool BlockQueue<T>::pop(T &item)
{
    std::unique_lock<std::mutex> locker(mtx_);
    // 如果队列空了，消费者等待 (直到有数据)
    while (deq_.empty())
    {
        if (isClose_)
        {
            return false;
        }
        condConsumer_.wait(locker);
    }
    item = deq_.front();
    deq_.pop_front();
    condProducer_.notify_one();
    return true;
}

template <class T>
void BlockQueue<T>::flush()
{
    condConsumer_.notify_one();
}

#endif // BLOCKQUEUE_H