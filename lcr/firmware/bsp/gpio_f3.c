#include "misc.h"

void GPIO_Config(void)
{
    GPIO_InitTypeDef gis;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

    // PB6~7 for USART1
    gis.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    gis.GPIO_Mode = GPIO_Mode_AF;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gis);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_7);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_7);

    // PA4  as dac
    gis.GPIO_Pin = GPIO_Pin_4;
    gis.GPIO_Mode = GPIO_Mode_AN;
    gis.GPIO_Speed = GPIO_Speed_2MHz;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &gis);

    // PA2 & PA6 as adc
    gis.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_6;
    gis.GPIO_Mode = GPIO_Mode_AN;
    gis.GPIO_Speed = GPIO_Speed_2MHz;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &gis);

    // PB12~13 as DG409 ctrl
    gis.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
    gis.GPIO_Mode = GPIO_Mode_OUT;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gis);
}
