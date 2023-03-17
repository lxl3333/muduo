#include<base/LogFile.h>

#include<base/FileUtil.h>
#include<base/ProcessInfo.h>

#include<assert.h>
#include<stdio.h>
#include<time.h>

namespace muduo
{
LogFile::LogFile(const string& basename,
                off_t rollSize,
                bool threadSafe,
                int flushInterval,
                int checkEveryN)
    :basename_(basename),
    rollSize_(rollSize),
    flushInterval_(flushInterval),
    checkEveryN_(checkEveryN),
    count_(0),
    mutex_(threadSafe?new MutexLock:NULL),
    startOfPeriod_(0),
    lastRoll_(0),
    lastFlush_(0)
{
    assert(basename.find('/')== string::npos);
    rollFile();
}

LogFile::~LogFile()=default;

void LogFile::append(const char * logline,int len)
{
    if(mutex_)
    {
        MutexLockGuard lock(*mutex_);
        append_unlocked(logline,len);
    }
    else
    {
        append_unlocked(logline,len);
    }
}

void LogFile::flush()
{
    if(mutex_)
    {
        MutexLockGuard lock(*mutex_);
        file_->flush();
    }
    else
    {
        file_->flush();
    }
}

void LogFile::append_unlocked(const char* logline,int len)
{
    file_->append(logline,len);
    if(file_->writtenBytes()>rollSize_)
    {
        rollFile();
    }
    else
    {
        ++count_;
        if(count_>=checkEveryN_)
        {
            count_=0;
            time_t now =::time(NULL);
            time_t thisPeriod_=now/kPollPerSeconds_ *kPollPerSeconds_;
            if(thisPeriod_ != startOfPeriod_)
            {
                rollFile();
            }
            else if(now -lastFlush_>flushInterval_)
            {
                lastFlush_ = now;
                file_->flush();
            }
        }
    }
}

bool LogFile::rollFile()
{
    time_t now =0;
    string filename =getLogFileName(basename_,&now);
    time_t start =now/kPollPerSeconds_ * kPollPerSeconds_;

    if(now>lastRoll_)
    {
        lastRoll_ =now;
        lastFlush_ =now;
        startOfPeriod_ =start;
        file_.reset(new FileUtil::AppendFile(filename));
        return true;
    }
    return false;
}

string LogFile::getLogFileName(const string & basename,time_t* now)
{
    string filename;
    filename.reserve(basename.size()+64);
    filename =basename;

    char timebuff[32];
    struct tm tm;
    *now=time(NULL);
    gmtime_r(now,&tm);
    strftime(timebuff,sizeof timebuff, ".%Y%m%d-%H%M%S.",&tm);
    filename+=timebuff;

    filename+=ProcessInfo::hostname();
}
}