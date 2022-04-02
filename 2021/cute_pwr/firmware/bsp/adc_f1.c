#include "misc.h"
#include "platform.h"

#define ADC_SAMPLES 64
#define ADC_CHANNELS 4

static struct {
    unsigned short buf[ADC_SAMPLES * ADC_CHANNELS];
} g;

void ADC_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    NVIC_InitTypeDef nis;
    DMA_InitTypeDef dis;
    ADC_InitTypeDef ais;

    nis.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    nis.NVIC_IRQChannelPreemptionPriority = 1;
    nis.NVIC_IRQChannelSubPriority = 1;
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

    RCC_ADCCLKConfig(RCC_PCLK2_Div8);
    ADC_StructInit(&ais);
    ADC_DeInit(ADC1);

    ais.ADC_Mode = ADC_Mode_Independent;
    ais.ADC_ScanConvMode = ENABLE;
    ais.ADC_ContinuousConvMode = ENABLE;
    ais.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ais.ADC_DataAlign = ADC_DataAlign_Right;
    ais.ADC_NbrOfChannel = 4;
    ADC_Init(ADC1, &ais);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 2, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 3, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 4, ADC_SampleTime_239Cycles5);
    ADC_ResetCalibration(ADC1);
    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);

    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void ADCDMA_Conv_Complete_IRQHandler(void)
{
    static unsigned long sum_adc[ADC_CHANNELS];
//    static int count = 0;
    for(int j = 0; j < ADC_CHANNELS; j++)
        sum_adc[j] = 0;

    for(int i = 0; i < ADC_SAMPLES; i++)
        for(int j = 0; j < ADC_CHANNELS; j++)
            sum_adc[j] += g.buf[i * ADC_CHANNELS + j];

    DataConfig_t* pdc = DC_Get();
    pdc->data.vout = sum_adc[0] * 1.0 / sum_adc[2] * 1.242 / 15.0 * 66.0;
    pdc->data.iout = sum_adc[1] * 1.0 / sum_adc[2] * 1.242 / 50.0 / 0.1;
    pdc->data.vbat = sum_adc[3] * 1.0 / sum_adc[2] * 1.242 * 2.0;
//    if(curr < 8e-3) curr = 0;
//    pdc->data.curr = curr;
//    pdc->data.vbat1 = sum_adc[1] / sum_adc[0] * 2.5 * 2;
}
