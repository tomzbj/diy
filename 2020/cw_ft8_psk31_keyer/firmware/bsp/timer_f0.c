#include "misc.h"

void TIMER_Config(void)
{
    // TIM6 for DAC
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
    TIM_TimeBaseInitTypeDef tbs;
    TIM_TimeBaseStructInit(&tbs);
    tbs.TIM_Prescaler = 0;
    tbs.TIM_ClockDivision = TIM_CKD_DIV1;
    tbs.TIM_Period = 374;    // 48M / 375 / 256 = 500Hz
    tbs.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM6, &tbs);
    TIM_ARRPreloadConfig(TIM6, ENABLE);
    TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
    TIM_Cmd(TIM6, DISABLE);    // TIM6 enable counter

    NVIC_InitTypeDef nis;
    nis.NVIC_IRQChannel = TIM7_IRQn;
    nis.NVIC_IRQChannelPriority = 1;
    nis.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nis);

    // TIM7 for PSK31
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
    TIM_TimeBaseStructInit(&tbs);
    tbs.TIM_Prescaler = 0;
    tbs.TIM_ClockDivision = TIM_CKD_DIV1;
    tbs.TIM_Period = 5999;    // 48M / 6000 = 8000Hz
    tbs.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM7, &tbs);
    TIM_ARRPreloadConfig(TIM7, ENABLE);
    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM7, ENABLE);

    nis.NVIC_IRQChannel = TIM17_IRQn;
    nis.NVIC_IRQChannelPriority = 1;
    nis.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nis);

    // TIM17 for FT8
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE);
    TIM_TimeBaseStructInit(&tbs);
    tbs.TIM_Prescaler = 255;
    tbs.TIM_ClockDivision = TIM_CKD_DIV1;
    tbs.TIM_Period = 29999;    // 48M / 256 / 30000 = 6.25Hz
    tbs.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM17, &tbs);
    TIM_ARRPreloadConfig(TIM17, ENABLE);
    TIM_ITConfig(TIM17, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM17, ENABLE);
}
