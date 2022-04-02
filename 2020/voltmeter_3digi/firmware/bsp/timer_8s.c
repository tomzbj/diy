#include "misc.h"
#include "ztask.h"

void TIMER_Config(void)
{
    unsigned long freq = CLK_GetClockFreq();
    int tmp = freq / 255 / 1000;
    int prescaler;
    int period;

    // T4 as systick, 1kHz
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, ENABLE);
    if(tmp < 128)
        prescaler = TIM4_PRESCALER_128;
    else if(tmp < 64)
        prescaler = TIM4_PRESCALER_64;
    else if(tmp < 32)
        prescaler = TIM4_PRESCALER_32;
    else if(tmp < 16)
        prescaler = TIM4_PRESCALER_16;
    else if(tmp < 8)
        prescaler = TIM4_PRESCALER_8;
    else if(tmp < 4)
        prescaler = TIM4_PRESCALER_4;
    else if(tmp < 2)
        prescaler = TIM4_PRESCALER_2;
    else
        prescaler = TIM4_PRESCALER_1;

    TIM4_DeInit();
    period = freq / _BV(prescaler) / 1000;
    TIM4_TimeBaseInit(prescaler, period - 1);
    TIM4_Cmd(ENABLE);
    TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
}

void TIMER4_IRQHandler(void)
{
    if(TIM4_GetITStatus(TIM4_IT_UPDATE)) {
        _delay_nms_mm();
        zt_tick();
        TIM4_ClearITPendingBit(TIM4_IT_UPDATE);
    }
}
