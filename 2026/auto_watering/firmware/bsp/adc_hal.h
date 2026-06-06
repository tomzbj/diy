#ifndef _ADC_HAL_H
#define _ADC_HAL_H

#include <cstdio>

void ADC_Config(void);
void ADC_View(void);
void ADC_Get(void);
void ADC_Get(uint16_t* pdata, int size);
uint16_t ADC_GetVbat(void);

#endif
