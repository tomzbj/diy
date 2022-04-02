#include "misc.h"
#include "platform.h"

// Sine table, 250 dots, 12bits, MUST STORE IN RAM, or sine wave will be wrong!
const static unsigned short sine_table[] = {
    //
    2048, 2060, 2073, 2085, 2098, 2110, 2123, 2135, 2147, 2160, 2172, 2184,
    2196, 2208, 2220, 2232, 2243, 2255, 2266, 2278, 2289, 2300, 2311, 2321,
    2332, 2342, 2352, 2362, 2372, 2382, 2391, 2401, 2410, 2418, 2427, 2435,
    2443, 2451, 2459, 2466, 2473, 2480, 2487, 2493, 2499, 2505, 2510, 2516,
    2521, 2525, 2530, 2534, 2537, 2541, 2544, 2547, 2550, 2552, 2554, 2556,
    2557, 2558, 2559, 2559, 2559, 2559, 2559, 2558, 2557, 2556, 2554, 2552,
    2550, 2547, 2544, 2541, 2537, 2534, 2530, 2525, 2521, 2516, 2510, 2505,
    2499, 2493, 2487, 2480, 2473, 2466, 2459, 2451, 2443, 2435, 2427, 2418,
    2410, 2401, 2391, 2382, 2372, 2362, 2352, 2342, 2332, 2321, 2311, 2300,
    2289, 2278, 2266, 2255, 2243, 2232, 2220, 2208, 2196, 2184, 2172, 2160,
    2147, 2135, 2123, 2110, 2098, 2085, 2073, 2060, 2047, 2035, 2022, 2010,
    1997, 1985, 1972, 1960, 1948, 1935, 1923, 1911, 1899, 1887, 1875, 1863,
    1852, 1840, 1829, 1817, 1806, 1795, 1784, 1774, 1763, 1753, 1743, 1733,
    1723, 1713, 1704, 1694, 1685, 1677, 1668, 1660, 1652, 1644, 1636, 1629,
    1622, 1615, 1608, 1602, 1596, 1590, 1585, 1579, 1574, 1570, 1565, 1561,
    1558, 1554, 1551, 1548, 1545, 1543, 1541, 1539, 1538, 1537, 1536, 1536,
    1536, 1536, 1536, 1537, 1538, 1539, 1541, 1543, 1545, 1548, 1551, 1554,
    1558, 1561, 1565, 1570, 1574, 1579, 1585, 1590, 1596, 1602, 1608, 1615,
    1622, 1629, 1636, 1644, 1652, 1660, 1668, 1677, 1685, 1694, 1704, 1713,
    1723, 1733, 1743, 1753, 1763, 1774, 1784, 1795, 1806, 1817, 1829, 1840,
    1852, 1863, 1875, 1887, 1899, 1911, 1923, 1935, 1948, 1960, 1972, 1985,
    1997, 2010, 2022, 2035    //
    };

const static int table_size = sizeof(sine_table) / sizeof(sine_table[0]);

void DAC_Config(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

    DAC_InitTypeDef dacis;
    DMA_InitTypeDef dmais;

    DAC_StructInit(&dacis);
    dacis.DAC_WaveGeneration = DAC_WaveGeneration_None;
    dacis.DAC_Buffer_Switch = DAC_BufferSwitch_Disable;
//    dacis.DAC_Buffer_Switch = DAC_BufferSwitch_Enable;
    dacis.DAC_Trigger = DAC_Trigger_T6_TRGO;    // TIM6 as trigger
//    dacis.DAC_Trigger = DAC_Trigger_None;
    DAC_Init(DAC1, DAC_Channel_1, &dacis);
    DAC_Cmd(DAC1, DAC_Channel_1, ENABLE);
    DAC_DMACmd(DAC1, DAC_Channel_1, ENABLE);

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
    DMA_Init(DMA2_Channel3, &dmais);
    DMA_Cmd(DMA2_Channel3, ENABLE);

    printf("[%s: %d] DAC Initialized. %d\n", __FILE__, __LINE__, table_size);
}
