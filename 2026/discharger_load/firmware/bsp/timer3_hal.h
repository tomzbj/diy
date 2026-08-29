#ifndef _TIMER3_HAL_H
#define _TIMER3_HAL_H

#include <cstdint>

void TIMER3_Config(void);
void TIMER3_SetDuty(int duty);
void TIMER3_SetCurr(float curr);

#endif
