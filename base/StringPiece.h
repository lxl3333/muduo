#ifndef MUDUO_BASE_STRINGOIECE_H
#define MUDUO_BASE_STRINGPIECE_H

#include<string_view>

#include<base/Types.h>

namespace muduo
{
class StringArg
{
public:
    StringArg(const char* str)
    :str_(str)
    {

    }

    StringArg(const string& str)
    :str_(str.c_str())
    {}

    const char * c_str() const
    {
        return str_;
    }
private:
    const char* str_;
};

typedef std::string_view StringPiece;
}


#endif