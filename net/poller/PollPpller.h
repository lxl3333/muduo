#ifndef MUDUO_NET_POLLER_POLLPOLLER_H
#define MUDUO_NET_POLLER_POLLPOLLER_H
#include<net/Poller.h>

struct pollfd;

namespace muduo
{

namespace net
{

class PollPoller:public Poller
{
public:
    PollPoller(EventLoop* loop);
    ~PollPoller() override;

    Timestamp poll(int timeoutMs, ChannelList* activeChannels) override;
    void updateChannel(Channel* channel) override;
    void removeChannel(Channel* channel) override;
    
private:
    void fillActiveChannels(int numEvents,ChannelList *activeChannels) const;

    typedef std::vector<struct pollfd> PollFdList;
    PollFdList pollfds_;
}

}//end of net

}//end of muduo


#endif