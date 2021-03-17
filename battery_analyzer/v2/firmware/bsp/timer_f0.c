#include "misc.h"
#include "platform.h"

void TIMER_Config(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    TIM_TimeBaseInitTypeDef tbs;
    TIM_OCInitTypeDef tois;

    TIM_TimeBaseStructInit(&tbs);
    tbs.TIM_Prescaler = 0;
    tbs.TIM_ClockDivision = TIM_CKD_DIV1;
    tbs.TIM_Period = 239;   // 48M / 240 / 2 = 100kHz
    tbs.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &tbs);

    TIM_OCStructInit(&tois);
    tois.TIM_OCMode = TIM_OCMode_Toggle;
    tois.TIM_OutputState = TIM_OutputState_Enable;
    tois.TIM_Pulse = 0;
    tois.TIM_OCPolarity = TIM_OCPolarity_High;
    tois.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OC1Init(TIM3, &tois);
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM3, ENABLE);
    TIM_CtrlPWMOutputs(TIM3, ENABLE);
    TIM_Cmd(TIM3, ENABLE); // TIM3 enable counter

    TIM_TimeBaseStructInit(&tbs);
    tbs.TIM_Prescaler = 99;
    tbs.TIM_ClockDivision = TIM_CKD_DIV1;
    tbs.TIM_Period = 7999;   // 8M / 100 / 8000 = 10Hz
    tbs.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &tbs);

    TIM_OCStructInit(&tois);
    tois.TIM_OCMode = TIM_OCMode_PWM2;
//    tois.TIM_OutputState = TIM_OutputState_Enable;
    tois.TIM_OutputNState = TIM_OutputNState_Enable;
    tois.TIM_Pulse = 80;
    tois.TIM_OCNPolarity = TIM_OCPolarity_High;
    tois.TIM_OCNIdleState = TIM_OCIdleState_Reset;
    TIM_OC1Init(TIM1, &tois);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Update);
    TIM_ARRPreloadConfig(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    TIM_Cmd(TIM1, ENABLE); // TIM1 enable counter
}
