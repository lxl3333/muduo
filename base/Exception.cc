#include <base/Exception.h>
#include <base/CurrentThread.h>

namespace muduo
{

Exception::Exception(string what):
message_(std::move(what)),stack_(CurrentThread::stackTrace(/*demangle是否解码函数名*/false));//调用线程的追踪


}