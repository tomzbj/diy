#ifndef _ADC_HAL_H
#define _ADC_HAL_H

struct ADC_RESULT {
    float curr_a, curr_b, volt_a, volt_b;
};

void ADC_Config(void);
//void ADC_Get(float* curr);
//void ADC_Get(float** curr);
ADC_RESULT* ADC_Get(void);

#endif
