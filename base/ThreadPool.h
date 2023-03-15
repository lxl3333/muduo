#ifndef MUDUO_BASE_THREADPOOL_H
#define MUDUO_BASE_THREADPOOL_H

#include <base/Condition.h>
#include<base/Mutex.h>
#include<base/Thread.h>
#include<base/Types.h>

#include<deque>
#include<vector>

namespace muduo
{
class ThreadPool:noncopyable
{
public:
    typedef std::function<void ()> Task;

    explicit ThreadPool(const string & nameArg =string("ThreadPool"));
    ~ThreadPool();

    void setMaxQueueSize(int maxSize){
        maxQueueSize_ =maxSize;
    }

    void setThreadInitCallback(const Task& cb)
    {
        threadIintCallback_=cb;
    }

    void start(int numThreads);
    void stop();

    const string& name() const{
        return name_;
    }

    size_t queueSize() const;

    void run(Task f);
private:
    bool isFull() const REQUIRES(mutex_);
    void runInThread();
    Task take();

    mutable MutexLock mutex_;
    Condition notEmpty_ GUARDED_BY(mutex_);
    Condition notFull_ GUARDED_BY(mutex_);
    string name_;
    Task threadIintCallback_;
    std::vector<std::unique_ptr<muduo::Thread>> threads_;
    std::deque<Task> queue_ GUARDED_BY(mutex_);
    size_t maxQueueSize_;
    bool running_;
};

}




#endif