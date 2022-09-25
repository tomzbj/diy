#include "misc.h"
#include "platform.h"

static void BSP_Init(void)
{
    disableInterrupts();
    CLK_DeInit();
    CLK_LSICmd(ENABLE);    //
    CLK_HSICmd(DISABLE);    //
    while(CLK_GetFlagStatus(CLK_FLAG_LSIRDY) == RESET);

    CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_LSI);
    CLK_SYSCLKSourceSwitchCmd(ENABLE);
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_2);
    GPIO_Config();
//    TIMER_Config();

    DISP_Config();

    enableInterrupts();
    RTC_WakeUpCmd(DISABLE);                  // 使能自动唤醒
    RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16); // 32.768k / 16 = 2.048k
    RTC_SetWakeUpCounter(2);               // 唤醒频率2048/204=10Hz, 周期100ms
    RTC_ITConfig(RTC_IT_WUT, ENABLE);      // 开启中断
    RTC_WakeUpCmd(ENABLE);                  // 使能自动唤醒
}

int main(void)
{
    BSP_Init();
    while(1) {
        int i, r = 0;
        const int rb[] = {2000, 5000, 500, 50, 5, 2, 20, 200};
        const int rc[] = {1, 10, 1, 10, 100, 100, 1000, 1000};
        for(i = 0; i <= 7; i++) {
            if(GPIOB->IDR & (1 << i))
                r += rb[i];
        }
        for(i = 0; i <= 7; i++) {
            if(GPIOC->IDR & (1 << i))
                r += rc[i];
        }
        LCD_WriteInt(r);
//        wfi();
        halt();
    }
}
