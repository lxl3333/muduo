#ifndef MUDUO_NET_POLLER_H
#define MUDUO_NET_POLLER_H

#include<map>
#include<vector>

#include<base/Timestamp.h>
#include<net/EventLoop.h>
#include<base/noncopyable.h>
namespace muduo
{

namespace net
{

class Channel;

class Poller:noncopyable
{
public:
    typedef std::vector<Channel*> ChannelList;

    Poller(EventLoop* loop);
    virtual ~Poller();

    virtual Timestamp poll(int timeoutMs ,ChannelList* activeChannels) =0;

    virtual void updateChannel(Channel * Channel) =0;

    virtual void removeChannel(Channel * channel ) =0;

    virtual bool hasChannel(Channel* channel) const;
    static Poller* newDefaultPoller(EventLoop* loop);

    void assertInLoopThread() const
    {
        ownerLoop_->assertInLoopThread();
    }

protected:
    typedef std::map<int,Channel*> ChannelMap;
    ChannelMap channels_;

private:


    EventLoop* ownerLoop_;
};



}




}





#endif