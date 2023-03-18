#include<base/Logging.h>

#include<base/CurrentThread.h>
#include<base/Timestamp.h>
#include<base/TimeZone.h>


#include<errno.h>
#include<stdio.h>
#include<string.h>

#include<sstream>

namespace muduo
{

__thread char t_errnobuf[512];
__thread char t_time[64];
__thread time_t t_lastSecond;

const char* strerror_tl(int savedErrno)
{
    
}






}//end of muduo