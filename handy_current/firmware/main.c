#include <stdio.h>
#include <string.h>
#include "misc.h"
#include "disp.h"
#include "key.h"
#include "current.h"
#include "flash_eeprom.h"
#include "gpio_f0.h"
#include "usart_f0.h"
#include "dac_f0.h"
#include "spi_f0.h"
#include "timer_f0.h"

#define APP_BASE 0x08002800
#define PAGE_SIZE 2048

void SystemInit(void)
{
    memcpy((void*)(0x20000000), (void*)APP_BASE, 256);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_MemoryRemapConfig(SYSCFG_MemoryRemap_SRAM);
    __enable_irq();

    RCC_DeInit();
    FLASH_SetLatency(FLASH_Latency_1);
    RCC_HCLKConfig(RCC_SYSCLK_Div1);    // AHB CLK = SYSCLK
    RCC_PCLKConfig(RCC_HCLK_Div1);    // APB1 CLK = SYSCLK

    RCC_PLLConfig(RCC_PLLSource_HSI, RCC_PLLMul_6); // 8M * 2 = 16M
    RCC_PLLCmd(ENABLE);
    ( {  while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);});
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    ( {  while(RCC_GetSYSCLKSource() != 0x08);});
}

int main(void)
{
    SystemInit();
    SysTick_Config(8000UL);
    NVIC_SetPriority(SysTick_IRQn, 1);
    GPIO_Config();
    USART_Config();
    KEY_Config();
    SPI2_Config();
    TIMER2_Config();
    FLASH_EEPROM_Config(APP_BASE - PAGE_SIZE, PAGE_SIZE);
    DISP_Config();
//    ( {  while(1) {USART_Poll();}});
    _delay_ms_loop(100);
    DAC_Config();
    CURRENT_Config();

    DISP_SetUpdate();
    DISP_Update();

    while(1) {
        USART_Poll();
        DISP_Update();
    }
    return 0;
}
