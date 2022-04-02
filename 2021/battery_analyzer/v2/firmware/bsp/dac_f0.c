#include "misc.h"
#include <stdio.h>

void DAC_Config(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    DAC_InitTypeDef dis;

    DAC_StructInit(&dis);
    dis.DAC_WaveGeneration = DAC_WaveGeneration_None;
    dis.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
    dis.DAC_Trigger = DAC_Trigger_None;    // TIM6 as trigger
    DAC_Init(DAC_Channel_1, &dis);
    DAC_Cmd(DAC_Channel_1, ENABLE);
    DAC_SetDualChannelData(DAC_Align_12b_R, 0, 0);
}

