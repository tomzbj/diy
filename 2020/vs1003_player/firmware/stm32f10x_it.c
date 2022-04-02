#include <stdlib.h>
#include <stdbool.h>
#include "misc.h"
#include "stm32f10x_it.h"
#include "ff.h"

#include "usb_istr.h"
#include "usb_int.h"
#include "playback.h"

bool g_status_sleep = false;

void EXTI2_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line2) != RESET) {
        if(g_status_sleep == false) {
            DIR dir;
            FRESULT res = res;

            FILINFO finfo;
            res = f_opendir(&dir, "/");
            _dbg();
            int n = 512, count = 0;
            while(f_readdir(&dir, &finfo) == FR_OK && n--) {
                if(finfo.fname[0]) {
                    count++;
                }
            }
            n = rand() % count;
            f_rewinddir(&dir);
            f_closedir(&dir);
            res = f_opendir(&dir, "/");
            uprintf(USART1, "%d %d\n", count, n);
            while(f_readdir(&dir, &finfo) == FR_OK && n > 0) {
                n--;
                if(n == 1)
                    PLAYBACK_Play(finfo.fname);
            }
            f_closedir(&dir);
        }
        EXTI_ClearITPendingBit(EXTI_Line2);
    }
}

void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) {
        USART_RX_IDLE_IRQHandler(USART1);
        USART_ClearITPendingBit(USART1, USART_IT_IDLE);
    }
}

void SysTick_Handler(void)
{
    _delay_nms_mm();        // executes every 1ms
    PLAYBACK_Handler();
}

void USB_HP_CAN1_TX_IRQHandler(void)
{
    CTR_HP();
}

void USB_LP_CAN1_RX0_IRQHandler(void)
{
    USB_Istr();
}

void PPP_IRQHandler(void)
{
}
void NMI_Handler(void)
{
}
void HardFault_Handler(void)
{
    while(1)
        ;
}
void MemManage_Handler(void)
{
    while(1) {
    }
}
void BusFault_Handler(void)
{
    while(1) {
    }
}
void UsageFault_Handler(void)
{
    while(1) {
    }
}
void SVC_Handler(void)
{
}
void DebugMon_Handler(void)
{
}
void PendSV_Handler(void)
{
}
