#include "misc.h"
#include "platform.h"

#define MAX_SAMPLES 64
#define NUM_CHANNELS 6
static unsigned short adc_buf[MAX_SAMPLES * NUM_CHANNELS];

void ADC_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    ADC_InitTypeDef ais;
    DMA_InitTypeDef dis;

    NVIC_InitTypeDef nvis;
    nvis.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    nvis.NVIC_IRQChannelPreemptionPriority = 1;
    nvis.NVIC_IRQChannelSubPriority = 1;
    nvis.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvis);

    DMA_DeInit(DMA1_Channel1);

    DMA_StructInit(&dis);
    dis.DMA_PeripheralBaseAddr = (unsigned long)&(ADC1->DR);
    dis.DMA_MemoryBaseAddr = (unsigned long)(adc_buf);
    dis.DMA_DIR = DMA_DIR_PeripheralSRC;
    dis.DMA_BufferSize = MAX_SAMPLES * NUM_CHANNELS;
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

    RCC_ADCCLKConfig(RCC_PCLK2_Div2);
    ADC_StructInit(&ais);
    ADC_DeInit(ADC1);
    ais.ADC_Mode = ADC_Mode_Independent;
    ais.ADC_ScanConvMode = ENABLE;
    ais.ADC_ContinuousConvMode = ENABLE;
    ais.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ais.ADC_DataAlign = ADC_DataAlign_Right;
    ais.ADC_NbrOfChannel = NUM_CHANNELS;
    ADC_Init(ADC1, &ais);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 6, ADC_SampleTime_239Cycles5);
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
    DataConfig_t* pdc = DC_Get();
    float sum_pos = 0;

    for(int j = 0; j < MAX_SAMPLES; j++) {
        int min = 99999;
        int n = -1;
        for(int i = 0; i < NUM_CHANNELS; i++) {
            if(adc_buf[i] < min) {
                min = adc_buf[i];
                n = i;
            }
        }
        sum_pos += n;
    }
    pdc->data.pos = sum_pos / MAX_SAMPLES;
}

