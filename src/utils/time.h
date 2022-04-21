#ifndef UTILS_TIME_H
#define UTILS_TIME_H

#include <types.h>

int64 GetTime();
int64 GetAdjustedTime();
void AddTimeData(unsigned int ip, int64 nTime);

#endif