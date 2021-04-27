#include "misc.h"

void TIMER_Config(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    TIM_TimeBaseInitTypeDef tbs;
    TIM_OCInitTypeDef tois;

    // T2_C4 as LCD Backlight
    TIM_TimeBaseStructInit(&tbs);
    tbs.TIM_Prescaler = 0;
    tbs.TIM_ClockDivision = TIM_CKD_DIV1;
    tbs.TIM_Period = 999;    // 72M / 1000  = 72kHz
    tbs.TIM_CounterMode = TIM_CounterMode_Up;
    tbs.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &tbs);
    TIM_TimeBaseInit(TIM1, &tbs);

    TIM_OCStructInit(&tois);
    tois.TIM_OCMode = TIM_OCMode_PWM1;
    tois.TIM_OutputState = TIM_OutputState_Enable;
    tois.TIM_Pulse = 500;
    tois.TIM_OCPolarity = TIM_OCPolarity_High;
    tois.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OC4Init(TIM2, &tois);
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM2, ENABLE);
    TIM_CtrlPWMOutputs(TIM2, ENABLE);
    TIM_Cmd(TIM2, ENABLE);    // TIM2 enable counter

    TIM_OC4Init(TIM1, &tois);
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    TIM_Cmd(TIM1, ENABLE);    // TIM2 enable counter
}
