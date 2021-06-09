#include "misc.h"

void TIMER_Config(void)
{
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15, ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);

    TIM_TimeBaseInitTypeDef tbs;

    // t6  for dac trigger
    TIM_TimeBaseStructInit(&tbs);
    tbs.TIM_Period = 27;
    tbs.TIM_Prescaler = 0;
    tbs.TIM_ClockDivision = TIM_CKD_DIV1;
    tbs.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM6, &tbs);
    TIM_ARRPreloadConfig(TIM6, ENABLE);
    TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
    TIM_Cmd(TIM6, ENABLE);

    // t15  for dac trigger
    TIM_TimeBaseStructInit(&tbs);
    tbs.TIM_Period = 71;    // 1MHz
    tbs.TIM_Prescaler = 0;
    tbs.TIM_ClockDivision = TIM_CKD_DIV1;
    tbs.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM15, &tbs);
    TIM_ARRPreloadConfig(TIM15, ENABLE);
    TIM_SelectOutputTrigger(TIM15, TIM_TRGOSource_Update);
    TIM_Cmd(TIM15, ENABLE);
}
