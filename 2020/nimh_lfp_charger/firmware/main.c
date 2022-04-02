#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include "stm32f0xx.h"
#include "usart.h"
#include "pid.h"
#include "adc_f0.h"
#include "timer_f0.h"
#include "led.h"

static void Init(void)
{

    SystemInit();
    SystemCoreClockUpdate();
    SysTick_Config(48000UL);
    NVIC_SetPriority(SysTick_IRQn, 0);
    RCC_AHBPeriphClockCmd(
        RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_DMA1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
    RCC_APB2PeriphClockCmd(
        RCC_APB2Periph_USART1 | RCC_APB2Periph_SYSCFG | RCC_APB2Periph_ADC1,
        ENABLE);

    USART_Config();
    ADC_Config();
    TIMER_Config();
    LED_Config();
}

int main(void)
{
    Init();
    while(1);
    return 0;
}
