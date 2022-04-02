#ifndef _ADC_F0_H
#define _ADC_F0_H

#include "misc.h"

void ADC_Config(void);
void ADC_Conversion_Complete_IRQHandler(void);    // callback
void ADCDMA_Conv_Complete_IRQHandler(void);
bool ADC_IsConvCompleted(void);
void ADC_UpdateData(void);

#endif
