#ifndef _ADC_F1_H
#define _ADC_F1_H

#include "misc.h"

typedef struct {
    float volt;
} ADC_Result_t;

void ADC_Config(void);
void ADC_Conversion_Complete_IRQHandler(void);   // callback
void ADCDMA_Conv_Complete_IRQHandler(void);
void ADC_UpdateData(void);
ADC_Result_t* ADC_GetResult(void);

#endif
