#include "base/Timestamp.h"

#include <sys/time.h>
#include <stdio.h>

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include <inttypes.h>


namespace muduo
{

static_assert(sizeof(Timestamp)==sizeof(int64_t))

Timestamp  Timestamp::now(){

}

string Timestamp::toString() const{

}

string Timestamp::toFormattedString(bool showMicroseconds = true) const{

}

}