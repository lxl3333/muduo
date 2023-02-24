#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H


namespace muduo
{

class noncopyable
{
public:
    noncopyable operator=(const noncopyable&)=delete;
    noncopyable(const noncopyable&)=delete;
protected:    
    noncopyable()=default;
    ~noncopyable()=default;
};


}

#endif