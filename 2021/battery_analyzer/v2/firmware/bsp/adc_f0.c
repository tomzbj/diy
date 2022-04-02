#include "misc.h"
#include "platform.h"

#define ADC_SAMPLES 128
#define ADC_CHANNELS 3

static struct {
    unsigned short buf[ADC_SAMPLES * ADC_CHANNELS];
} g;

void ADC_Config(void)
{
    NVIC_InitTypeDef nis;
    DMA_InitTypeDef dis;
    ADC_InitTypeDef ais;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    nis.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    nis.NVIC_IRQChannelPriority = 2;
    nis.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nis);

    DMA_DeInit(DMA1_Channel1);

    DMA_StructInit(&dis);

    dis.DMA_PeripheralBaseAddr = (unsigned long)&(ADC1->DR);
    dis.DMA_MemoryBaseAddr = (unsigned long)(g.buf);
    dis.DMA_DIR = DMA_DIR_PeripheralSRC;
    dis.DMA_BufferSize = ADC_SAMPLES * ADC_CHANNELS;
    dis.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dis.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dis.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    dis.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    dis.DMA_Mode = DMA_Mode_Circular;
    dis.DMA_Priority = DMA_Priority_High;
    dis.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &dis);
    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
    DMA_ClearITPendingBit(DMA_IT_TC);
    DMA_Cmd(DMA1_Channel1, ENABLE);

    ADC_DeInit(ADC1);
    RCC_ADCCLKConfig(RCC_ADCCLK_PCLK_Div4);
    ADC_StructInit(&ais);
    ADC_DeInit(ADC1);

    ais.ADC_Resolution = ADC_Resolution_12b;
    ais.ADC_ContinuousConvMode = ENABLE;
    ais.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ais.ADC_ScanDirection = ADC_ScanDirection_Upward;
    ais.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_Init(ADC1, &ais);
    ADC_ChannelConfig(ADC1, ADC_Channel_0, ADC_SampleTime_239_5Cycles);
    ADC_ChannelConfig(ADC1, ADC_Channel_1, ADC_SampleTime_239_5Cycles);
//    ADC_ChannelConfig(ADC1, ADC_Channel_2, ADC_SampleTime_239_5Cycles);
    ADC_ChannelConfig(ADC1, ADC_Channel_5, ADC_SampleTime_239_5Cycles);
    ADC_GetCalibrationFactor(ADC1);
    ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);
    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN));
    ADC_StartOfConversion(ADC1);
}

void ADCDMA_Conv_Complete_IRQHandler(void)
{
    static float sum_adc[ADC_CHANNELS];
//    static int count = 0;
    for(int j = 0; j < ADC_CHANNELS; j++) {
        sum_adc[j] = 0;
    }

    for(int i = 0; i < ADC_SAMPLES; i++)
        for(int j = 0; j < ADC_CHANNELS; j++)
            sum_adc[j] += g.buf[i * ADC_CHANNELS + j];
    DataConfig_t* pdc = DC_Get();
    float curr = sum_adc[2] / sum_adc[0] * 2.5 / 11 / 0.15;
    if(curr < 8e-3)
        curr = 0;
    pdc->data.curr = curr;
    pdc->data.vbat1 = sum_adc[1] / sum_adc[0] * 2.5 * 2;
//    pdc->data.vbat2 = sum_adc[2] / sum_adc[0] * 2.5;
}
