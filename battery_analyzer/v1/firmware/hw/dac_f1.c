#include "misc.h"
#include <stdio.h>

void DAC_Config(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    DAC_InitTypeDef DAC_InitStructure;

    GPIO_StructInit(&GPIO_InitStructure);
    // PA4 for sine wave
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIOB->BRR = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;

    DAC_StructInit(&DAC_InitStructure);
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;    // TIM6 as trigger
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);
    DAC_Cmd(DAC_Channel_1, ENABLE);
}

void DAC_CurrentSet(int curr)
{
    unsigned short val;
    GPIOB->BRR = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    if(curr <= 100) {
        val = curr / 1000.0 * 5.0 / 3.3 * 4095 * (12.49 / 2.49) / 1.07;
        DAC_SetChannel1Data(DAC_Align_12b_R, val);
        GPIOB->BSRR = GPIO_Pin_15;
    }
    else if(curr <= 500) {
        val = curr / 1000.0 * 1.0 / 3.3 * 4095 * (12.49 / 2.49) / 1.052;
        DAC_SetChannel1Data(DAC_Align_12b_R, val);
        GPIOB->BSRR = GPIO_Pin_14;
    }
    else {
        val = curr / 1000.0 * 0.2 / 3.3 * 4095 * (12.49 / 2.49) * 1.04;
        DAC_SetChannel1Data(DAC_Align_12b_R, val);
        GPIOB->BSRR = GPIO_Pin_13;
    }
}
