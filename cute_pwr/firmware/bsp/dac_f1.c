#include "misc.h"
#include "platform.h"

void DAC_Config(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

    DAC_InitTypeDef dacis;

    DAC_StructInit(&dacis);
    dacis.DAC_WaveGeneration = DAC_WaveGeneration_None;
    dacis.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
    dacis.DAC_Trigger = DAC_Trigger_None;
    DAC_Init(DAC_Channel_1, &dacis);
    DAC_Init(DAC_Channel_2, &dacis);
    DAC_Cmd(DAC_Channel_1, ENABLE);
    DAC_Cmd(DAC_Channel_2, ENABLE);
}
