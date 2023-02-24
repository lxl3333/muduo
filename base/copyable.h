#ifndef MUDUO_BASE_COPYABLE_H
#define MUDUO_BASE_COPYABLE_H

namespace muduo
{

class copyable//值类，可以被拷贝
{
protected:
    copyable()=default;
    ~copyable()=default;
};

}

#endif