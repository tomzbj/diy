#include "misc.h"
#include "platform.h"

void TIMER_Config(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);

    NVIC_InitTypeDef nis;
    nis.NVIC_IRQChannelPriority = 1;
    nis.NVIC_IRQChannelCmd = ENABLE;
    nis.NVIC_IRQChannel = TIM16_IRQn;
    NVIC_Init(&nis);

    TIM_TimeBaseInitTypeDef tbs;
    TIM_OCInitTypeDef tois;

    // T14 for bias
    TIM_TimeBaseStructInit(&tbs);
    tbs.TIM_Prescaler = 0;
    tbs.TIM_ClockDivision = TIM_CKD_DIV1;
    tbs.TIM_Period = 999;    // 48M / 1000 = 48kHz
    tbs.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM14, &tbs);

    TIM_OCStructInit(&tois);
    tois.TIM_OCMode = TIM_OCMode_PWM1;
    tois.TIM_OutputState = TIM_OutputState_Enable;
    tois.TIM_Pulse = 0;
    tois.TIM_OCPolarity = TIM_OCPolarity_High;
    tois.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OC1Init(TIM14, &tois);
    TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM14, ENABLE);
    TIM_CtrlPWMOutputs(TIM14, ENABLE);
    TIM_Cmd(TIM14, ENABLE);    // TIM14 enable counter

    TIM_SetCompare1(TIM14, 1);

    // T16 for adc
    TIM_TimeBaseStructInit(&tbs);
    tbs.TIM_Prescaler = 0;
    tbs.TIM_ClockDivision = TIM_CKD_DIV1;
    tbs.TIM_Period = 9599;    // 48M / 9600 = 5kHz
    tbs.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM16, &tbs);
    TIM_ITConfig(TIM16, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM16, ENABLE);    // TIM14 enable counter
}
