#include "misc.h"

void TIMER2_Config(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseInitTypeDef tbs;
    TIM_OCInitTypeDef tois;
    tbs.TIM_Period = 999;
    tbs.TIM_Prescaler = 0;
    tbs.TIM_ClockDivision = 0;
    tbs.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &tbs);

    tois.TIM_OCMode = TIM_OCMode_PWM2;
    tois.TIM_OutputState = TIM_OutputState_Enable;
    tois.TIM_Pulse = 100;
    tois.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OC4Init(TIM2, &tois);
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM2, ENABLE);
    TIM_Cmd(TIM2, ENABLE); // TIM2 enable counter
    TIM_CtrlPWMOutputs(TIM2, ENABLE);
}
