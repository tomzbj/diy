#include "misc.h"

void GPIO_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitTypeDef gis;
    GPIO_StructInit(&gis);

    // PA9 & PA10 for USART1
    gis.GPIO_Pin = GPIO_Pin_9;
    gis.GPIO_Mode = GPIO_Mode_AF_PP;
    gis.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOA, &gis);
    gis.GPIO_Mode = GPIO_Mode_IPU;
    gis.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA, &gis);

    // PA0~5 as ADC
    gis.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3
        | GPIO_Pin_4 | GPIO_Pin_5;
    gis.GPIO_Mode = GPIO_Mode_AIN;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gis);

    // PB10~15 as ADC en
    gis.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13
        | GPIO_Pin_14 | GPIO_Pin_15;
    gis.GPIO_Mode = GPIO_Mode_Out_PP;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gis);

    // PB6~9 as motor ctrl
    gis.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    gis.GPIO_Mode = GPIO_Mode_AF_PP;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gis);
}
