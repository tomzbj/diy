#ifndef _RTC_H
#define _RTC_H

#include <cstdint>

void RTC_Config(void);
int RTC_GetBootCount(void);
void RTC_MaybeNtp(void);
void RTC_DeepSleep(uint64_t time_us);

#endif
