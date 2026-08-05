#ifndef _TIMER15_HAL_H
#define _TIMER15_HAL_H

#include <cstdint>

void TIMER15_Config(void);
void TIMER15_SetDuty(int channel, int duty);
void TIMER15_SetVoltCurr(float volt, float curr);

#endif
