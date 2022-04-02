#include "misc.h"
#include "platform.h"

void TIMER_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);

    TIM_TimeBaseInitTypeDef tbs;
    TIM_OCInitTypeDef tois;

    TIM_TimeBaseStructInit(&tbs);
    tbs.TIM_Prescaler = 0;
    tbs.TIM_ClockDivision = TIM_CKD_DIV1;
    tbs.TIM_Period = 999;    // 48M / 1000 = 48kHz
    tbs.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM15, &tbs);

    TIM_OCStructInit(&tois);
    tois.TIM_OCMode = TIM_OCMode_PWM1;
    tois.TIM_OutputState = TIM_OutputState_Enable;
    tois.TIM_Pulse = 0;
    tois.TIM_OCPolarity = TIM_OCPolarity_High;
    tois.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OC1Init(TIM15, &tois);
    TIM_OC1PreloadConfig(TIM15, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM15, ENABLE);
    TIM_CtrlPWMOutputs(TIM15, ENABLE);
    TIM_Cmd(TIM15, ENABLE);    // TIM15 enable counter

    TIM_SetCompare1(TIM15, 1);

    TIM_TimeBaseStructInit(&tbs);
    tbs.TIM_Prescaler = 0;
    tbs.TIM_ClockDivision = TIM_CKD_DIV1;
    tbs.TIM_Period = 999;    // 48M / 1000 = 48kHz
    tbs.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM16, &tbs);

    TIM_OCStructInit(&tois);
    tois.TIM_OCMode = TIM_OCMode_PWM1;
    tois.TIM_OutputState = TIM_OutputState_Enable;
    tois.TIM_Pulse = 0;
    tois.TIM_OCPolarity = TIM_OCPolarity_High;
    tois.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OC1Init(TIM16, &tois);
    TIM_OC1PreloadConfig(TIM16, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM16, ENABLE);
    TIM_CtrlPWMOutputs(TIM16, ENABLE);
    TIM_Cmd(TIM16, ENABLE);    // TIM16 enable counter
}
