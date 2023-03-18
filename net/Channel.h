#ifndef MUDUO_NET_CHANNEL_H
#define MUDUO_NET_CHANNEL_H

#include<base/noncopyable.h>
#include<base/Timestamp.h>

#include<functional>
#include<memory>

namespace muduo
{

namespace net
{

class EventLoop;

class Channel:noncopyable
{
public:
    typedef std::function<void()> EventCallback;
    typedef std::function<void(Timestamp)> ReadEventCallback;

    Channel(EventLoop* loop,int fd);
    ~Channel();

    void handleEvent(Timestamp receiveTime);
    void setReadCallback(ReadEventCallback cb){
        readCallback_=std::move(cb);
    }
    void setWriteCallback(EventCallback cb)
    {
        writeCallback_=std::move(cb);
    }
    void setCloseCallback(EventCallback cb)
    {
        closeCallback_=std::move(cb);
    }
    void setErrorCallback(EventCallback cb)
    {
        errorCallback_=std::move(cb);
    }

    void tie(const std::shared_ptr<void>&);

    int fd() const {return fd_;}
    int events() const {return events_;}
    int set_revents(int revt){revents_=revt;}

    bool isNoneEvent() const {return events_ == kNoneEvent;}

    void enableReading(){ events_|=kReadEvent;update();}
    void disableReading(){events_&=~kReadEvent;update();}
    void enableWriting(){events_|=kWriteEvent;update();}
    void disableWriting(){events_&=~kWriteEvent;update();}
    void disableAll(){events_=kNoneEvent;update();}
    bool isReading() const {return events_& kReadEvent;}
    bool isWriting() const {return events_&kWriteEvent;}

    int index(){return index_;}
    void set_index(int idx) {index_ =idx;}

    string reventsToString() const;
    string eventsToString() const;

    void doNotLogHup() {logHup_ =false;}

    EventLoop* ownerLoop(){return loop_;}

    void remove();
private:
    static string eventsToString(int fd ,int ev);

    void update();
    void handleEventWithGuard(Timestamp receiveTime);
    
    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;

    EventLoop*  loop_;//记录归属的EventLoop对象
    const int   fd_;//记录文件描述符
    int         events_;//记录文件描述符的关注事件
    int         revents_;//记录关注事件中已经发生的事件
    int         index_;//记录在Poller中，关注列表的位置
    bool        logHup_;

    bool                tied_;
    bool                eventHandling_;
    bool                addedToLoop_;
    ReadEventCallback   readCallback_;
    EventCallback       writeCallback_;
    EventCallback       closeCallback_;
    EventCallback       errorCallback_;
    std::weak_ptr<void> tie_;
};




}

}

#endif