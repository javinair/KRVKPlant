#ifndef TIMEHELPER_H
#define TIMEHELPER_H

#include <time.h>

class TimeHelper {
public:
    static void syncTime();
    static char* getCurrentTime();
    
private:
    static bool timeSynced;
};

#endif
