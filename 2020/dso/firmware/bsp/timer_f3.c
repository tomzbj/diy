#include "misc.h"


void TIMER_Config(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

    TIM_TimeBaseInitTypeDef tbs;
    TIM_OCInitTypeDef tois;

    TIM_TimeBaseStructInit(&tbs);
    tbs.TIM_Prescaler = 0;
    tbs.TIM_ClockDivision = TIM_CKD_DIV2;
    tbs.TIM_Period = 3999;    // 80M / 2 / 4000 = 10kHz
    tbs.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM6, &tbs);

    TIM_ARRPreloadConfig(TIM6, ENABLE);
    TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
    TIM_Cmd(TIM6, ENABLE);    // TIM6 enable counter

    // T3 for 1kHz test sig
    TIM_TimeBaseStructInit(&tbs);
    tbs.TIM_Prescaler = 0;
    tbs.TIM_ClockDivision = TIM_CKD_DIV1;
    tbs.TIM_Period = 35999;    // 72M / 36000 / 2 = 1kHz
    tbs.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &tbs);

    TIM_OCStructInit(&tois);
    tois.TIM_OCMode = TIM_OCMode_Toggle;
    tois.TIM_OutputState = TIM_OutputState_Enable;
    tois.TIM_Pulse = 0;
    tois.TIM_OCPolarity = TIM_OCPolarity_High;
    tois.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OC3Init(TIM3, &tois);
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM3, ENABLE);
    TIM_CtrlPWMOutputs(TIM3, ENABLE);
    TIM_Cmd(TIM3, ENABLE);    // TIM3 enable counter
}
