#include "misc.h"

#define PRESCALER_28 28
#define PRESCALER_2800 2800

void TIMER_Config(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    TIM_TimeBaseInitTypeDef tbs;
    TIM_TimeBaseStructInit(&tbs);
    tbs.TIM_Prescaler = PRESCALER_28 - 1;
    tbs.TIM_ClockDivision = TIM_CKD_DIV1;
    tbs.TIM_Period = 1499;   // 84M / 28 / 150 = 20kHz, 20kHz / 2 = 10kHz
    tbs.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &tbs);
    TIM_ARRPreloadConfig(TIM3, ENABLE);
    TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);
//    TIM_Cmd(TIM3, ENABLE); // TIM3 enable counter
    //    TIM_Cmd(TIM3, DISABLE); // TIM3 enable counter

    // T3_C2 for PLL_REF
    TIM_OCInitTypeDef tois;
    tois.TIM_OCMode = TIM_OCMode_Toggle;
    tois.TIM_OutputState = TIM_OutputState_Enable;
    tois.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OC2Init(TIM3, &tois);
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM3, ENABLE);
    TIM_Cmd(TIM3, ENABLE);  //使能TIM3

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    TIM_ETRClockMode1Config(TIM1, TIM_ExtTRGPSC_OFF,
        TIM_ExtTRGPolarity_Inverted, 0);
    TIM_TimeBaseStructInit(&tbs);
    tbs.TIM_Prescaler = 0;
    tbs.TIM_ClockDivision = TIM_CKD_DIV1;
    tbs.TIM_Period = 4900;      // FOUT / 8
    tbs.TIM_CounterMode = TIM_CounterMode_Up;
    tbs.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &tbs);
    tois.TIM_OCMode = TIM_OCMode_Toggle;
    tois.TIM_OutputState = TIM_OutputState_Enable;
    tois.TIM_OCPolarity = TIM_OCPolarity_Low;
//    tois.TIM_OCIdleState=TIM_OCIdleState_Set;
    tois.TIM_Pulse = tbs.TIM_Period / 2;
    TIM_OC4Init(TIM1, &tois);
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    TIM_Cmd(TIM1, ENABLE);  //使能TIM1

    // T5_C2 for HDD PWM
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
    TIM_TimeBaseStructInit(&tbs);
    tbs.TIM_Prescaler = 25;
    tbs.TIM_ClockDivision = TIM_CKD_DIV1;
    tbs.TIM_Period = 99;      // 84M / 21 / 100 = 40kHz
    tbs.TIM_CounterMode = TIM_CounterMode_Up;
    tbs.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM5, &tbs);
    tois.TIM_OCMode = TIM_OCMode_PWM2;
    tois.TIM_OutputState = TIM_OutputState_Enable;
    tois.TIM_OCPolarity = TIM_OCPolarity_Low;
    tois.TIM_Pulse = 30;
    TIM_OC2Init(TIM5, &tois);
    TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM5, ENABLE);
    TIM_Cmd(TIM5, ENABLE);

    // T2_E for HDD measure
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_ETRClockMode1Config(TIM2, TIM_ExtTRGPSC_OFF,
        TIM_ExtTRGPolarity_Inverted, 0);
    TIM_TimeBaseStructInit(&tbs);
    tbs.TIM_Prescaler = 0;
    tbs.TIM_ClockDivision = TIM_CKD_DIV1;
    tbs.TIM_Period = 0xffff;
    tbs.TIM_CounterMode = TIM_CounterMode_Up;
    tbs.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &tbs);
    TIM_ARRPreloadConfig(TIM2, ENABLE);
    TIM_Cmd(TIM2, ENABLE);  //使能TIM2
}

void TIMER_SetSamplingRate(int rate)
{
    unsigned short prescaler;
    if(rate >= 100)
        prescaler = PRESCALER_28;
    else
        prescaler = PRESCALER_2800;

    TIM_PrescalerConfig(TIM3, prescaler - 1, TIM_PSCReloadMode_Immediate);
    TIM_SetAutoreload(TIM3, SystemCoreClock / rate / prescaler - 1);
}
