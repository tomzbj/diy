#include "misc.h"
#include "platform.h"
#include <math.h>

#define LVDT_SINE_FREQ    10000

// Sine table, 240 dots, 12bits, MUST STORE IN RAM, or sine wave will be wrong!
static unsigned short g_sine_table[] = {
    2048, 2089, 2131, 2173, 2215, 2256, 2298, 2339, 2380, 2421, 2462, 2502,
    2542, 2582, 2621, 2660, 2698, 2736, 2774, 2811, 2848, 2883, 2919, 2954,
    2988, 3022, 3054, 3087, 3118, 3149, 3179, 3208, 3237, 3264, 3291, 3317,
    3342, 3366, 3389, 3412, 3433, 3454, 3473, 3492, 3509, 3526, 3541, 3556,
    3569, 3582, 3593, 3603, 3613, 3621, 3628, 3634, 3639, 3643, 3645, 3647,
    3647, 3647, 3645, 3643, 3639, 3634, 3628, 3621, 3613, 3603, 3593, 3582,
    3569, 3556, 3541, 3526, 3509, 3492, 3473, 3454, 3433, 3412, 3389, 3366,
    3342, 3317, 3291, 3264, 3237, 3208, 3179, 3149, 3118, 3087, 3054, 3022,
    2988, 2954, 2919, 2883, 2847, 2811, 2774, 2736, 2698, 2660, 2621, 2582,
    2542, 2502, 2462, 2421, 2380, 2339, 2298, 2256, 2215, 2173, 2131, 2089,
    2047, 2006, 1964, 1922, 1880, 1839, 1797, 1756, 1715, 1674, 1633, 1593,
    1553, 1513, 1474, 1435, 1397, 1359, 1321, 1284, 1247, 1212, 1176, 1141,
    1107, 1073, 1041, 1008, 977, 946, 916, 887, 858, 831, 804, 778,
    753, 729, 706, 683, 662, 641, 622, 603, 586, 569, 554, 539,
    526, 513, 502, 492, 482, 474, 467, 461, 456, 452, 450, 448,
    448, 448, 450, 452, 456, 461, 467, 474, 482, 492, 502, 513,
    526, 539, 554, 569, 586, 603, 622, 641, 662, 683, 706, 729,
    753, 778, 804, 831, 858, 887, 916, 946, 977, 1008, 1041, 1073,
    1107, 1141, 1176, 1212, 1248, 1284, 1321, 1359, 1397, 1435, 1474, 1513,
    1553, 1593, 1633, 1674, 1715, 1756, 1797, 1839, 1880, 1922, 1964, 2006
//
    };

const int g_sine_table_size = sizeof(g_sine_table) / sizeof(g_sine_table[0]);

void DAC_SetAmplitude(unsigned short amp)
{
    for(int i = 0; i < g_sine_table_size; i++) {
        g_sine_table[i] = (int)(sin(i * 1.0 / g_sine_table_size * 2 * 3.141593)
            * amp + 2048);
    }
//    for(int i = 0; i < g_sine_table_size; i++) { printf("%u\n", g_sine_table[i]); }
}

void DAC_Config(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
    DAC_InitTypeDef dacis;
    DMA_InitTypeDef dmais;
    DAC_StructInit(&dacis);
    dacis.DAC_WaveGeneration = DAC_WaveGeneration_None;
    dacis.DAC_Buffer_Switch = DAC_BufferSwitch_Enable;
    dacis.DAC_Trigger = DAC_Trigger_T6_TRGO; // TIM6 as trigger
    DAC_Init(DAC1, DAC_Channel_1, &dacis);
    DAC_Cmd(DAC1, DAC_Channel_1, ENABLE);
    DAC_DMACmd(DAC1, DAC_Channel_1, ENABLE);
    DMA_StructInit(&dmais);
    dmais.DMA_DIR = DMA_DIR_PeripheralDST;
    dmais.DMA_BufferSize = g_sine_table_size;
    dmais.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dmais.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dmais.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    dmais.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    dmais.DMA_Priority = DMA_Priority_High;
    dmais.DMA_M2M = DMA_M2M_Disable;
    dmais.DMA_Mode = DMA_Mode_Circular;
    dmais.DMA_MemoryBaseAddr = (unsigned long)g_sine_table;
    dmais.DMA_PeripheralBaseAddr = (unsigned long)&(DAC->DHR12R1);
    DMA_Init(DMA2_Channel3, &dmais);
    DMA_Cmd(DMA2_Channel3, ENABLE);

    const unsigned long sysclk = 72000000UL;
    unsigned short period = sysclk / g_sine_table_size / LVDT_SINE_FREQ - 1;
    TIM_SetAutoreload(TIM6, period);
}
