#include "misc.h"
#include "platform.h"

void TIMER_Config(void)
{
    TIM_TimeBaseInitTypeDef tbis;
    TIM_OCInitTypeDef tocis;

    // TIM4 for motors, and adc trig, interrupt
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    TIM_TimeBaseStructInit(&tbis);
    tbis.TIM_Period = 999;
    tbis.TIM_Prescaler = 79;
    tbis.TIM_ClockDivision = 0;
    tbis.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &tbis);

    TIM_OCStructInit(&tocis);
    tocis.TIM_OCMode = TIM_OCMode_PWM2;
    tocis.TIM_OutputState = TIM_OutputState_Enable;
    tocis.TIM_Pulse = 0;
    tocis.TIM_OCPolarity = TIM_OCPolarity_Low;

    TIM_OC1Init(TIM4, &tocis);
    TIM_OC2Init(TIM4, &tocis);
    TIM_OC3Init(TIM4, &tocis);
    TIM_OC4Init(TIM4, &tocis);

    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM4, ENABLE);
    TIM_CtrlPWMOutputs(TIM4, ENABLE);
    TIM_Cmd(TIM4, ENABLE);    // TIMx enable counter
}

