#ifndef _ADC_F0_H
#define _ADC_F0_H

#include "misc.h"

typedef enum {
    BATT_ERR, BATT_PRE_CHARGE, BATT_FAST_CHARGE, BATT_TICKLE_CHARGE, BATT_FULL
} BattStatusTypeDef;

void ADC_Config(void);
void ADC_Conversion_Complete_IRQHandler(void);   // callback
void ADCDMA_Conv_Complete_IRQHandler(void);
bool ADC_IsConvCompleted(void);
void ADC_UpdateData(void);

BattStatusTypeDef ADC_GetBattStatus(void);

#endif
