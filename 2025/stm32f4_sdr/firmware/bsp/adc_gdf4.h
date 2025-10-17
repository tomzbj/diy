#ifndef _ADC_GDF3_H
#define _ADC_GDF3_H

void ADC_Config(void);
void ADC_GetDataOnce(void* dst);
//void ADC_Start(void);
void ADC_Start(void* buf, int count);
bool ADC_IsDone(void);

#endif
