#include "nrf24le1.h"

void PWM_Init(PWM_InitTypeDef* PWM_InitStructure)
{
    PWMCON = *(unsigned char*)PWM_InitStructure;
}

void PWM_SetDutyCycle(PWM_ChannelTypeDef channel, unsigned char duty_cycle)
{
    if(channel == PWM_Channel_0) {
        PWMDC0 = duty_cycle;
    }
    else if(channel == PWM_Channel_1) {
        PWMDC1 = duty_cycle;
    } 
}
