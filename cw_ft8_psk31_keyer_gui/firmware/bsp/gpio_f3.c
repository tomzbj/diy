#include "misc.h"

void GPIO_Config(void)
{
    GPIO_InitTypeDef gis;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

    // PA2~3 for USART2
    gis.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    gis.GPIO_Mode = GPIO_Mode_AF;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gis);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_7);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_7);

    // PB9 as LCD Backlight pwm
    gis.GPIO_Pin = GPIO_Pin_9;
    gis.GPIO_Mode = GPIO_Mode_AF;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gis);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_1);

    // PB3-5 as SPI1 for LCD
    gis.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
    gis.GPIO_Mode = GPIO_Mode_AF;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gis);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_5);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_5);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_5);

    // PB6-8 as LCD CS, RS, RESET
    gis.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
    gis.GPIO_Mode = GPIO_Mode_OUT;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gis);

    // PA7 for LED
    gis.GPIO_Pin = GPIO_Pin_7;
    gis.GPIO_Mode = GPIO_Mode_OUT;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gis);

    // PB13-15 as SPI2 for W25
    gis.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    gis.GPIO_Mode = GPIO_Mode_AF;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gis);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_5);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_5);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_5);

    // PB12 as CS for W25
    gis.GPIO_Pin = GPIO_Pin_12;
    gis.GPIO_Mode = GPIO_Mode_OUT;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gis);

    // PA8~10 for USART1
    gis.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
    gis.GPIO_Mode = GPIO_Mode_AF;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gis);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_7);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_7);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_7);

    // PA11 as CS for XPT2046
    gis.GPIO_Pin = GPIO_Pin_11;
    gis.GPIO_Mode = GPIO_Mode_OUT;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gis);

    // PA4  as dac
    gis.GPIO_Pin = GPIO_Pin_4;
    gis.GPIO_Mode = GPIO_Mode_AN;
    gis.GPIO_Speed = GPIO_Speed_2MHz;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &gis);

    // PB11 as ECHO EN
    gis.GPIO_Pin = GPIO_Pin_11;
    gis.GPIO_Mode = GPIO_Mode_OUT;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gis);
    GPIOB->BSRR = GPIO_Pin_11;

    // PB0&1 as i2c for MCP4018_ECHO, PB2&10 as i2c for MCP4018_OUTPUT
    gis.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_10;
    gis.GPIO_Mode = GPIO_Mode_OUT;
    gis.GPIO_OType = GPIO_OType_OD;
    gis.GPIO_PuPd = GPIO_PuPd_UP;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gis);
    /*
     // PA2 & PA6 as adc
     gis.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_6;
     gis.GPIO_Mode = GPIO_Mode_AN;
     gis.GPIO_Speed = GPIO_Speed_2MHz;
     gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
     GPIO_Init(GPIOA, &gis);

     */
}
