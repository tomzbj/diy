#include <stdio.h>
#include "stm32f0xx_it.h"
#include "platform.h"
#include "iambic.h"

void SysTick_Handler(void)
{
    static int count = 0;
    _delay_nms_mm();            // executes every 1ms
    count++;
    if(count % 20 == 0) {
        KEY_Read();
    }
    if(count % (1000 / IAMBIC_GetCurrWPM()) == 0) {
        IAMBIC_SetFlag();
    }
}

void EXTI0_1_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line1) != RESET) {
        EXTI_ClearITPendingBit(EXTI_Line1);
        EXTI_ClearFlag(EXTI_Line1);
    }
}

void PPP_IRQHandler(void)
{
    ( {  while(1);});
}
void NMI_Handler(void)
{
    ( {  while(1);});
}
void HardFault_Handler(void)
{
    ( {  while(1);});
}
void SVC_Handler(void)
{
    ( {  while(1);});
}
void PendSV_Handler(void)
{
    ( {  while(1);});
}
