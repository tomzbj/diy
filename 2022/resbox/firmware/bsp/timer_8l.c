#include "misc.h"
#include "platform.h"

static unsigned long g_nms;

void TIMER_Config(void)
{
    TIM4_DeInit();
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);    //  开启定时器3的时钟
    TIM4_TimeBaseInit(TIM4_Prescaler_1, 9); // 16M/16000=1k
    TIM4_ARRPreloadConfig(ENABLE);
    TIM4_ClearITPendingBit(TIM4_IT_Update);    //参考库
    TIM4_ITConfig(TIM4_IT_Update, ENABLE);
    TIM4_Cmd(ENABLE);

    /*
     TIM1_DeInit();
     CLK_PeripheralClockConfig(CLK_Peripheral_TIM1, ENABLE);    //  开启定时器1的时钟
     TIM1_TimeBaseInit(0, TIM1_CounterMode_Up, 1024, 0);    // 16M / 1024 = 16K
     TIM1_OC2Init(TIM1_OCMode_PWM1, TIM1_OutputState_Enable,
     TIM1_OutputNState_Disable, 500, TIM1_OCPolarity_High,
     TIM1_OCNPolarity_High, TIM1_OCIdleState_Reset, TIM1_OCNIdleState_Reset);
     TIM1_OC2PreloadConfig(ENABLE);
     TIM1_ARRPreloadConfig(ENABLE);
     //    TIM1_ClearITPendingBit(TIM1_IT_Update);    //参考库
     //    TIM1_ITConfig(TIM1_IT_Update, ENABLE);
     TIM1_CtrlPWMOutputs(ENABLE);
     TIM1_Cmd(ENABLE);
     */
}
