#include "misc.h"

void EXTI_Config(void)
{
    NVIC_InitTypeDef nis;

    nis.NVIC_IRQChannel = EXTI9_5_IRQn;
    nis.NVIC_IRQChannelPreemptionPriority = 2;
    nis.NVIC_IRQChannelSubPriority = 1;
    nis.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nis);

    EXTI_InitTypeDef eis;
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource5);
    EXTI_StructInit(&eis);
    EXTI_ClearITPendingBit(EXTI_Line5);
    eis.EXTI_Line = EXTI_Line5;
    eis.EXTI_Mode = EXTI_Mode_Interrupt;
    eis.EXTI_Trigger = EXTI_Trigger_Rising;
    eis.EXTI_LineCmd = ENABLE;
    EXTI_Init(&eis);
    NVIC_EnableIRQ(EXTI9_5_IRQn);
}
