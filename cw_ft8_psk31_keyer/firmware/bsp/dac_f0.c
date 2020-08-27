#include "misc.h"
#include "platform.h"

// Sine table, 250 dots, 12bits, MUST STORE IN RAM, or sine wave will be wrong!
const static unsigned short sine_table[] = {
    //
    2048, 2098, 2148, 2198, 2248, 2298, 2348, 2397, 2447, 2496, 2545, 2593,
    2642, 2690, 2737, 2784, 2831, 2877, 2923, 2968, 3012, 3056, 3100, 3143,
    3185, 3226, 3267, 3307, 3346, 3385, 3422, 3459, 3495, 3530, 3564, 3598,
    3630, 3661, 3692, 3721, 3750, 3777, 3803, 3829, 3853, 3876, 3898, 3919,
    3939, 3957, 3975, 3991, 4006, 4020, 4033, 4045, 4055, 4064, 4072, 4079,
    4085, 4089, 4092, 4094, 4095, 4094, 4092, 4089, 4085, 4079, 4072, 4064,
    4055, 4045, 4033, 4020, 4006, 3991, 3975, 3957, 3939, 3919, 3898, 3876,
    3853, 3829, 3803, 3777, 3750, 3721, 3692, 3661, 3630, 3598, 3564, 3530,
    3495, 3459, 3422, 3385, 3346, 3307, 3267, 3226, 3185, 3143, 3100, 3056,
    3012, 2968, 2923, 2877, 2831, 2784, 2737, 2690, 2642, 2593, 2545, 2496,
    2447, 2397, 2348, 2298, 2248, 2198, 2148, 2098, 2048, 1997, 1947, 1897,
    1847, 1797, 1747, 1698, 1648, 1599, 1550, 1502, 1453, 1405, 1358, 1311,
    1264, 1218, 1172, 1127, 1083, 1039, 995, 952, 910, 869, 828, 788, 749, 710,
    673, 636, 600, 565, 531, 497, 465, 434, 403, 374, 345, 318, 292, 266, 242,
    219, 197, 176, 156, 138, 120, 104, 89, 75, 62, 50, 40, 31, 23, 16, 10, 6, 3,
    1, 1, 1, 3, 6, 10, 16, 23, 31, 40, 50, 62, 75, 89, 104, 120, 138, 156, 176,
    197, 219, 242, 266, 292, 318, 345, 374, 403, 434, 465, 497, 531, 565, 600,
    636, 673, 710, 749, 788, 828, 869, 910, 952, 995, 1039, 1083, 1127, 1172,
    1218, 1264, 1311, 1358, 1405, 1453, 1502, 1550, 1599, 1648, 1698, 1747,
    1797, 1847, 1897, 1947, 1997
};
const static int table_size = sizeof(sine_table) / sizeof(sine_table[0]);

void DAC_Config(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DAC_InitTypeDef dacis;
    DMA_InitTypeDef dmais;

    DAC_StructInit(&dacis);
    dacis.DAC_WaveGeneration = DAC_WaveGeneration_None;
    dacis.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
    dacis.DAC_Trigger = DAC_Trigger_T6_TRGO;    // TIM6 as trigger
//    dacis.DAC_Trigger = DAC_Trigger_None;
    DAC_Init(DAC_Channel_1, &dacis);
    DAC_Cmd(DAC_Channel_1, ENABLE);
    DAC_DMACmd(DAC_Channel_1, ENABLE);
    dacis.DAC_Trigger = DAC_Trigger_None;
    DAC_Init(DAC_Channel_2, &dacis);
    DAC_Cmd(DAC_Channel_2, ENABLE);

    DMA_StructInit(&dmais);
    dmais.DMA_DIR = DMA_DIR_PeripheralDST;
    dmais.DMA_BufferSize = table_size;
    dmais.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dmais.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dmais.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    dmais.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    dmais.DMA_Priority = DMA_Priority_High;
    dmais.DMA_M2M = DMA_M2M_Disable;
    dmais.DMA_Mode = DMA_Mode_Circular;

    dmais.DMA_PeripheralBaseAddr = (unsigned long)&(DAC->DHR12R1);
    dmais.DMA_MemoryBaseAddr = (unsigned long)sine_table;
    DMA_Init(DMA1_Channel3, &dmais);
    DMA_Cmd(DMA1_Channel3, ENABLE);
}
