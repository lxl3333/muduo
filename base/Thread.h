#ifndef MUDUO_BASE_THREAD_H
#define MUDUO_BASE_THREAD_H

#include<base/Atomic.h>
#include<base/CountDownLatch.h>
#include<base/Types.h>

#include<functional>
#include<memory>
#include<pthread.h>

namespace muduo
{

class Thread:noncopyable//封装线程，在这里定义了一个类，其具有启动函数，加入函数等，内部封装了正常的启动函数
{
public:
    typedef std::function<void ()> ThreadFunc;

    explicit Thread(ThreadFunc,const string& name=string());

    ~Thread();

    void start();

    int join();

    bool started() const {return started_;}

    pid_t tid() const { return tid_};

    const string& name() const { return name_}

    static int numCreated() { return numCreated_.get(); }


private:
    void serDefaultName();//设置默认函数

    bool        started_;
    bool        joined_;
    pthread_t   pthreadId_;
    pid_t       tid_;
    ThreadFunc  func_;
    string      name_;
    CountDownLatch  latch_;

    static AtomicInt32 numCreated_;
};



}










#endif