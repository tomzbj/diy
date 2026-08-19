#ifndef _TIMER5_HAL_H
#define _TIMER5_HAL_H

void TIMER5_Config(void);

// duty 0..999 (‰ of period); 500 ≈ 50% at resonance
void TIMER5_SetDuty(int duty_permille);

#endif
