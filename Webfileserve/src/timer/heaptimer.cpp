#include "../../include/timer/heaptimer.h"

void HeapTimer::swapNode_(size_t i, size_t j)
{
    assert(i >= 0 && i < heap_.size());
    assert(j >= 0 && j < heap_.size());
    std::swap(heap_[i], heap_[j]);
    // 更新索引映射
    ref_[heap_[i].id] = i;
    ref_[heap_[j].id] = j;
}

void HeapTimer::siftup_(size_t i)
{
    assert(i >= 0 && i < heap_.size());
    while (i > 0)
    {
        size_t j = (i - 1) / 2; // 父节点索引
        if (heap_[j] < heap_[i])
        {
            break;
        } // 父节点小，满足堆性质，停止
        swapNode_(i, j);
        i = j;
    }
}

// 返回 false 表示无法继续下沉
bool HeapTimer::siftdown_(size_t index, size_t n)
{
    assert(index >= 0 && index < heap_.size());
    assert(n >= 0 && n <= heap_.size());
    size_t i = index;
    size_t j = i * 2 + 1; // 左子节点
    while (j < n)
    {
        // 如果有右子节点，且右子节点比左子节点还小，那就选右边的
        if (j + 1 < n && heap_[j + 1] < heap_[j])
            j++;
        // 如果当前节点比最小的子节点还小，说明位置对了，不用沉了
        if (heap_[i] < heap_[j])
            return true;

        swapNode_(i, j);
        i = j;
        j = i * 2 + 1;
    }
    return i > index;
}

// 添加定时器
void HeapTimer::add(int id, int timeOut, const TimeoutCallBack &cb)
{
    assert(id >= 0);
    std::lock_guard<std::mutex> locker(mtx_);
    size_t i;
    if (ref_.count(id) == 0)
    {
        // 新节点：插到堆尾，然后上浮
        i = heap_.size();
        ref_[id] = i;
        heap_.push_back({id, Clock::now() + MS(timeOut), cb});
        siftup_(i);
    }
    else
    {
        // 已有的节点：更新时间，然后下沉 (因为时间变大了)
        i = ref_[id];
        heap_[i].expires = Clock::now() + MS(timeOut);
        heap_[i].cb = cb;
        if (!siftdown_(i, heap_.size()))
        {
            siftup_(i);
        }
    }
}

// 删除指定id的定时器 (触发回调)
void HeapTimer::doWork(int id)
{
    // 如果堆里没有这个id，直接返回
    if (heap_.empty() || ref_.count(id) == 0)
    {
        return;
    }
    std::lock_guard<std::mutex> locker(mtx_);
    // 双重检查，防止在获取锁之前被删除
    if (heap_.empty() || ref_.count(id) == 0)
        return;

    size_t i = ref_[id];
    TimerNode node = heap_[i];
    node.cb(); // 执行回调函数
    del_(i);   // 从堆中物理删除
}

// 删除指定位置的节点
void HeapTimer::del_(size_t i)
{
    /* 1. 先检查是否为空，或者索引是否越界 */
    if (heap_.empty() || i >= heap_.size())
    {
        return;
    }

    /* 2. 获取最后一个元素的索引 */
    size_t n = heap_.size() - 1;

    /* 3. 如果要删除的不是最后一个元素，才需要交换 */
    if (i < n)
    {
        swapNode_(i, n); // 把要删除的节点换到队尾
        if (!siftdown_(i, n))
        { // 调整堆结构
            siftup_(i);
        }
    }

    /* 4. 删除队尾元素 */
    ref_.erase(heap_.back().id);
    heap_.pop_back();
}

// 调整过期时间 (续命)
void HeapTimer::adjust(int id, int timeout)
{
    assert(!heap_.empty() && ref_.count(id) > 0);
    std::lock_guard<std::mutex> locker(mtx_);
    heap_[ref_[id]].expires = Clock::now() + MS(timeout);
    ;
    siftdown_(ref_[id], heap_.size());
}

// 清除所有
void HeapTimer::clear()
{
    std::lock_guard<std::mutex> locker(mtx_);
    ref_.clear();
    heap_.clear();
}

int HeapTimer::getNextTick()
{
    tick(); // 先清理一遍过期的
    std::lock_guard<std::mutex> locker(mtx_);
    size_t res = -1;
    if (!heap_.empty())
    {
        // 计算堆顶元素还有多久过期
        res = std::chrono::duration_cast<MS>(heap_.front().expires - Clock::now()).count();
        if (res < 0)
        {
            res = 0;
        }
    }
    return res;
}

// 核心：心跳函数，清除超时的节点
void HeapTimer::tick()
{
    std::lock_guard<std::mutex> locker(mtx_);
    if (heap_.empty())
    {
        return;
    }

    while (!heap_.empty())
    {
        TimerNode node = heap_.front();
        // 如果堆顶还没过期，那后面的肯定也没过期，直接break
        if (std::chrono::duration_cast<MS>(node.expires - Clock::now()).count() > 0)
        {
            break;
        }
        // 过期了：执行回调，弹出
        node.cb();
        del_(0);
    }
}

void HeapTimer::pop()
{
    std::lock_guard<std::mutex> locker(mtx_);
    assert(!heap_.empty());
    del_(0);
}