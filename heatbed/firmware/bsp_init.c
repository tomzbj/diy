#include "platform.h"

static void setsda(int state)
{
    (state) ? (GPIOC->BSRR = GPIO_Pin_15) : (GPIOC->BRR = GPIO_Pin_15);
    _delay_us(3);
}

static void setscl(int state)
{
    (state) ? (GPIOC->BSRR = GPIO_Pin_14) : (GPIOC->BRR = GPIO_Pin_14);
    _delay_us(3);
}

static int getsda(void)
{
    _delay_us(3);
    return (GPIOC->IDR & GPIO_Pin_15);
}

void BSP_Init(void)
{
    SystemInit();
    SysTick_Config(48000UL);
    FLASH_EEPROM_Config(0x08001c00, 1024);
    DC_Config();
    GPIO_Config();
    TIMER_Config();
    USART_Config();
    DISP_Config();
    KEY_Config();
    {
        zi2c_t i2c;
        i2c.setsda_f = setsda;
        i2c.setscl_f = setscl;
        i2c.getsda_f = getsda;
        i2c.addr = 0x48;
        zi2c_init(&i2c);
        LM75_Init(&i2c);
    }

    TASKS_Config();
}
