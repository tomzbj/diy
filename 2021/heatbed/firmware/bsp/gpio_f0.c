#include "misc.h"

void GPIO_Config(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

    GPIO_InitTypeDef gis;
    GPIO_StructInit(&gis);

    // PA9 & PA10 as USART1
    gis.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    gis.GPIO_Mode = GPIO_Mode_AF;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    gis.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &gis);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

    // PA2 as T15_C1 for PWM output
    gis.GPIO_Pin = GPIO_Pin_2;
    gis.GPIO_Mode = GPIO_Mode_AF;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &gis);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_0);

    // PC14 & PC15 as I2C for LM75
    gis.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
    gis.GPIO_Mode = GPIO_Mode_OUT;
    gis.GPIO_OType = GPIO_OType_OD;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    gis.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &gis);

    // PB8 as T16_C1 for LED
    gis.GPIO_Pin = GPIO_Pin_8;
    gis.GPIO_Mode = GPIO_Mode_AF;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &gis);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_2);

    // PB5~PB7 as i2c for oled
    gis.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    gis.GPIO_Mode = GPIO_Mode_OUT;
    gis.GPIO_OType = GPIO_OType_OD;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    gis.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &gis);

    // PB0~PB1 as input for keys
    gis.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    gis.GPIO_Mode = GPIO_Mode_IN;
    gis.GPIO_OType = GPIO_OType_OD;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    gis.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &gis);
}
