#ifndef MUDUO_BASE_BOUNDEDBLOCKINGQUEUE_H
#define MUDUO_BASE_BOUNDEDBLOCKINGQUEUE_H

#include<Mutex.h>
#include<Condition.h>

#include<deque>
#include<assert.h>

namespace muduo
{
template<typename T>
class BoundedBlockingQueue:noncopyable
{
public:
    BoundedBlockingQueue(unsigned maxSize)
    :maxSize_(maxSize),
    mutex_(),
    notEmpty_(mutex_),
    notFull_(mutex_),
    deque_()
    {
    }
    void put(const T& x)
    {
        MutexLockGuard lock(mutex_);
        while(deque_.size()==maxSize)
        {
            notFull_.wait();
        }
        assert(deque_.size()<maxSize_);
        deque_.push_back(x);
        notEmpty_.notify();
    }

    T take()
    {
        MutexLockGuard lock(mutex_);
        while (deque_.empty())
        {
            notEmpty_.wait();
        }
        T front=deque_.front();
        deque_.pop_front();
        notFull_.notify();
        return front;
    }

    bool empty() const
    {
        MutexLockGuard lock(mutex_);
        return deque_.empty();
    }

    bool full() const
    {
        MutexLockGuard lock(mutex_);
        return deque_.size()==maxSize_;
    }

    size_t size() const
    {
        MutexLockGuard lock(mutex_);
        return deque_.size();
    }

private:
    const unsigned      maxSize_;
    mutable MutexLock   mutex_;
    Condition           notEmpty_ GUARDED_BY(mutex_);
    Condition           notFull_  GUARDED_BY(mutex_);
    std::deque<T>       deque_;

};
}

#endif