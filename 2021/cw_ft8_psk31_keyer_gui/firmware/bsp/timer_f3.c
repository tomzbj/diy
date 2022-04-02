#include "misc.h"

void TIMER_Config(void)
{
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE);

    TIM_TimeBaseInitTypeDef tbs;
    TIM_OCInitTypeDef tois;

    // T17_C1 as LCD Backlight
    TIM_TimeBaseStructInit(&tbs);
    tbs.TIM_Prescaler = 0;
    tbs.TIM_ClockDivision = TIM_CKD_DIV1;
    tbs.TIM_Period = 999;    // 72M / 3600  = 2kHz
    tbs.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM17, &tbs);

    TIM_OCStructInit(&tois);
    tois.TIM_OCMode = TIM_OCMode_PWM1;
    tois.TIM_OutputState = TIM_OutputState_Enable;
    tois.TIM_Pulse = 500;
    tois.TIM_OCPolarity = TIM_OCPolarity_High;
    tois.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OC1Init(TIM17, &tois);
    TIM_OC1PreloadConfig(TIM17, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM17, ENABLE);
    TIM_CtrlPWMOutputs(TIM17, ENABLE);
    TIM_Cmd(TIM17, ENABLE);    // TIM17 enable counter

    NVIC_InitTypeDef nis;
    nis.NVIC_IRQChannel = TIM7_IRQn;
    nis.NVIC_IRQChannelPreemptionPriority = 1;
    nis.NVIC_IRQChannelSubPriority = 1;
    nis.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nis);

    // TIM7 for PSK31 & FT8 timing
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
    TIM_TimeBaseStructInit(&tbs);
    tbs.TIM_Prescaler = 0;
    tbs.TIM_ClockDivision = TIM_CKD_DIV1;
    tbs.TIM_Period = 8999;    // 72M / 9000 = 8000Hz
    tbs.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM7, &tbs);
    TIM_ARRPreloadConfig(TIM7, ENABLE);
    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM7, ENABLE);

    // t6  for dac trigger
    TIM_TimeBaseStructInit(&tbs);
    tbs.TIM_Period = 562;       // 72e6 / 562 / 256 = 500Hz
    tbs.TIM_Prescaler = 0;
    tbs.TIM_ClockDivision = TIM_CKD_DIV1;
    tbs.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM6, &tbs);
    TIM_ARRPreloadConfig(TIM6, ENABLE);
    TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
    TIM_Cmd(TIM6, ENABLE);

    printf("[%s: %d] TIMER Initialized.\n", __FILE__, __LINE__);
}
