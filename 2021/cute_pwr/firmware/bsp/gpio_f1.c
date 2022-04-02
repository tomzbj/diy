#include "misc.h"

void GPIO_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitTypeDef gis;
    GPIO_StructInit(&gis);

    // PB6 & PB7 for USART1
    gis.GPIO_Pin = GPIO_Pin_6;
    gis.GPIO_Mode = GPIO_Mode_AF_PP;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gis);
    gis.GPIO_Mode = GPIO_Mode_IPU;
    gis.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOB, &gis);
    GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);

    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    // PB3 & PB5 as spi1 for lcd
    gis.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5;
    gis.GPIO_Mode = GPIO_Mode_AF_PP;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gis);
    GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);

    // PC13~15 as output for lcd cs/rs/reset
    gis.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    gis.GPIO_Mode = GPIO_Mode_Out_PP;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &gis);

    // PA3 as t2_c4 for lcd backlight
    gis.GPIO_Pin = GPIO_Pin_3;
    gis.GPIO_Mode = GPIO_Mode_AF_PP;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gis);

    // PA11 as t1_c4 for lcd backlight
    gis.GPIO_Pin = GPIO_Pin_11;
    gis.GPIO_Mode = GPIO_Mode_AF_PP;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gis);

    // PA6-7, PB0-1 as adc
    gis.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    gis.GPIO_Mode = GPIO_Mode_AIN;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gis);
    gis.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_Init(GPIOB, &gis);

    // PA4-5 as dac
    gis.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_Init(GPIOA, &gis);
}
