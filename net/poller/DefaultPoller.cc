#include<net/Poller.h>
#include<net/poller/PollPpller.h>

#include<stdlib.h>

using namespace muduo::net;

Poller* Poller::newDefaultPoller(EventLoop* loop)
{
    if(::getenv("MUDUO_USE_POLL"))
    {
        return PollPoller(loop);
    }
    else
    {
        //return new 
    }
}