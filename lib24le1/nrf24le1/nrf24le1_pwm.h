#ifndef _NRF24LE1_PWM_H_
#define _NRF24LE1_PWM_H_

typedef enum {
    PWM_PeriodLength_5bits = 0,
    PWM_PeriodLength_6bits = 1,
    PWM_PeriodLength_7bits = 2,
    PWM_PeriodLength_8bits = 3
} PWM_PeriodLengthTypeDef;

typedef enum {
    PWM_FrequencyPrescaler_0 = 0,
    PWM_FrequencyPrescaler_1 = 1,
    PWM_FrequencyPrescaler_2 = 2,
    PWM_FrequencyPrescaler_3 = 3,
    PWM_FrequencyPrescaler_4 = 4,
    PWM_FrequencyPrescaler_5 = 5,
    PWM_FrequencyPrescaler_6 = 6,
    PWM_FrequencyPrescaler_7 = 7,
    PWM_FrequencyPrescaler_8 = 8,
    PWM_FrequencyPrescaler_9 = 9,
    PWM_FrequencyPrescaler_10 = 10,
    PWM_FrequencyPrescaler_11 = 11,
    PWM_FrequencyPrescaler_12 = 12,
    PWM_FrequencyPrescaler_13 = 13,
    PWM_FrequencyPrescaler_14 = 14,
    PWM_FrequencyPrescaler_15 = 15
} PWM_FrequencyPrescalerTypeDef;

typedef struct {
    unsigned char PWM_Channel_0_State:1;
    unsigned char PWM_Channel_1_State:1;
    unsigned char PWM_FrequencyPrescaler:4;
    unsigned char PWM_PeriodLength:2;
} PWM_InitTypeDef;

typedef enum {
    PWM_Channel_0 = 0, PWM_Channel_1 = 1
} PWM_ChannelTypeDef;

void PWM_Init(PWM_InitTypeDef* PWM_InitStructure);
void PWM_SetDutyCycle(PWM_ChannelTypeDef channel, unsigned char duty_cycle);

#endif
