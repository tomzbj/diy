#include "misc.h"

void GPIO_Config(void)
{
    unsigned short pins;

    // PA4~7 as COM0~COM2, SEG0 for LCD
    pins = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIOA, pins, GPIO_Mode_Out_PP_High_Slow);

    // PE0~5 as SEG1~SEG6 for LCD
    pins = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4
        | GPIO_Pin_5;
    GPIO_Init(GPIOE, pins, GPIO_Mode_Out_PP_High_Slow);    // SEG1~6

    // PD0~1 as SEG7 & COM3 for LCD
    pins = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_Init(GPIOD, pins, GPIO_Mode_Out_PP_High_Slow);    // SEG7, COM3

    // PB & PC as input
    pins = 0xff;
    GPIO_Init(GPIOB, pins, GPIO_Mode_In_FL_No_IT);
    GPIO_Init(GPIOC, pins, GPIO_Mode_In_FL_No_IT);
}
