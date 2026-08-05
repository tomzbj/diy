#ifndef _ADC_HAL_H
#define _ADC_HAL_H

void ADC_Config(void);
void ADC_View(void);
// Out-params avoid std::pair sret ABI through function pointers on armv6-m.
void ADC_Get(float* volt, float* curr);

#endif
