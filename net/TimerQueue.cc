#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif

#include<net/TimerQueue.h>


#include<base/Logging.h>
#include<net/EventLoop.h>
#include<net/Timer.h>
#include<net/TimerId.h>

#include<sys/timerfd.h>
#include<unistd.h>

namespace muduo
{

namespace net
{

namespace detail
{

int createTimerfd()
{
    int timerfd =::timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK|TFD_CLOEXEC);
    if(timerfd<0)
    {
        LOG_SYSFATAL<<"Failed in timerfd_create";
    }
    return timerfd;
}

struct timespec howMuchTimeFromNow(Timestamp when)
{
    int64_t microseconds=when.microSecondsSinceEpoch()-Timestamp::now().microSecondsSinceEpoch();

    if(microseconds<100)
    {
        microseconds=100;
    }
    struct timespec ts;
    ts.tv_nsec=static_cast<time_t>(microseconds/Timestamp::kMicroSecondsPerSecond);

    ts.tv_sec=static_cast<long>((microseconds%Timestamp::kMicroSecondsPerSecond)*1000);
    return ts;
}

void readTimerfd(int timerfd,Timestamp now)
{
    uint64_t howmany;
    ssize_t n=::read(timerfd,&howmany,sizeof howmany);
    LOG_TRACE <<"TimerQueue::handleRead()"<<howmany<<" at "<<now.toString();
    if(n!=sizeof howmany)
    {
        LOG_ERROR <<"TimerQueue::handleRead() reads" <<n<<" bytes instead of 8";
    }
}


void resetTimerfd(int timerfd,Timestamp expiration)
{
    struct itimerspec newValue;
    struct itimerspec oldValue;
    memZero(&newValue,sizeof newValue);
    memZero(&oldValue,sizeof oldValue);
    newValue.it_value=howMuchTimeFromNow(expiration);
    int ret=::timerfd_settime(timerfd,0,&newValue,&oldValue);
    if(ret)
    {
        LOG_SYSERR<<"timerfd_settime()";
    }
}

}//detail

}//net


}//muduo

using namespace muduo;
using namespace muduo::net;
using namespace muduo::net::detail;

TimerQueue::TimerQueue(EventLoop* loop)
    :loop_(loop),
    timerfd_(createTimerfd()),
    timerfdChannel_(loop,timerfd_),
    timers_(),
    callingExpiredTimers_(false)
{
    timerfdChannel_.setReadCallback(std::bind(&TimerQueue::handleRead,this));
    timerfdChannel_.enableReading();
}