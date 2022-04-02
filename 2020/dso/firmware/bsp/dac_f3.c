#include "misc.h"

void DAC_Config(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

    DAC_InitTypeDef dacis;

    DAC_StructInit(&dacis);
    dacis.DAC_WaveGeneration = DAC_WaveGeneration_None;
    dacis.DAC_Buffer_Switch = DAC_BufferSwitch_Disable;
    dacis.DAC_Trigger = DAC_Trigger_None;    // TIM6 as trigger
    DAC_Init(DAC1, DAC_Channel_1, &dacis);
    DAC_Init(DAC1, DAC_Channel_2, &dacis);
    DAC_Cmd(DAC1, DAC_Channel_1, ENABLE);
    DAC_Cmd(DAC1, DAC_Channel_2, ENABLE);

    DAC_SetChannel1Data(DAC1, DAC_Align_12b_R, 2048);
    DAC_SetChannel2Data(DAC1, DAC_Align_12b_R, 4095);
}
