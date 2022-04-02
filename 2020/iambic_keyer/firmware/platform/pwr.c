#include "pwr.h"
#include "misc.h"

void PWR_Sleep(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource1);
    EXTI_StructInit(&EXTI_InitStructure);
    EXTI_ClearITPendingBit(EXTI_Line1);
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // nvic init
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_DisableIRQ(EXTI0_1_IRQn);
    EXTI_ClearITPendingBit(EXTI_Line1);
    NVIC_SystemLPConfig(NVIC_LP_SLEEPDEEP, ENABLE);

    GPIOA->ODR &= ~(GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);
    unsigned long perh, per1;
    perh = RCC_AHBPeriph_DMA1 | RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB
            | RCC_AHBPeriph_GPIOF;
    per1 = RCC_APB1Periph_TIM14;
    RCC_AHBPeriphClockCmd(perh, DISABLE);
    RCC_APB1PeriphClockCmd(per1, DISABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    NVIC_EnableIRQ(EXTI0_1_IRQn);
    SysTick->CTRL &= ~(1 << 0);
    EXTI_ClearITPendingBit(EXTI_Line1);
    EXTI_ClearFlag(EXTI_Line1);
    PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI); // will reset after wake up
    NVIC_DisableIRQ(EXTI0_1_IRQn);
    SysTick->CTRL |= (1 << 0);
    void Init(void);
    Init();
}
