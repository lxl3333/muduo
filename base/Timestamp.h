#ifndef MUDUO_BASE_TIMESTAMP_H
#define MUDUO_BASE_TIMESTAMP_H

#include "base/copyable.h"
#include "base/Types.h"
namespace muduo
{

class Timestamp:public muduo::copyable//时间戳，可拷贝
{
public:
    Timestamp():microSecondsSinceEpoch_(0){}//invalid Timestamp

    explicit Timestamp(int64_t microSecondsSinceEpoch):microSecondsSinceEpoch_(microSecondsSinceEpoch){}//传递参数构造时间戳类

    void swap(Timestamp& that){//交换值
        std::swap(microSecondsSinceEpoch_,that.microSecondsSinceEpoch_);//内置引用
    }

    string toString() const;
    string toFormattedString(bool showMicroseconds = true) const;
    bool valid() const{//判断时间戳是否有效
        return microSecondsSinceEpoch_>0;
    }
    int64_t microSecondsSinceEpoch() const{
        return microSecondsSinceEpoch_;
    }
    time_t secondsSinceEpoch() const{
        return microSecondsSinceEpoch_/kMicroSecondsPerSecond;
    }
    static Timestamp now();
    static Timestamp invalid(){
        return Timestamp();
    }

    static Timestamp fromUnixTime(time_t t){
        return Timestamp(t*kMicroSecondsPerSecond);
    }

    static Timestamp fromUnixTIme(time_t t,int microseconds){
        return Timestamp(t*kMicroSecondsPerSecond+microseconds);
    }

    static const int kMicroSecondsPerSecond =1000*1000;
private:
    int64_t microSecondsSinceEpoch_;
};

inline bool operator<(Timestamp lhs,Timestamp rhs);

inline bool operator==(Timestamp lhs,Timestamp rhs);

inline double timeDifference(Timestamp high,Timestamp low);

inline Timestamp addTime(Timestamp timestamp,double seconds);

}















#endif