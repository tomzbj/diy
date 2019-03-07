#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "misc.h"
#include "string.h"
#include "adc_f1.h"
#include "usart_f1.h"

#define MAX_CJR_SAMPLES 64

static struct {
    unsigned short cjr_buffer[MAX_CJR_SAMPLES];
    ADC_Result_t result;
} g = { {0}, {0}};

ADC_Result_t* ADC_GetResult(void)
{
    return &(g.result);
}

void ADC_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_AFIO, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    GPIO_StructInit(&GPIO_InitStructure);

    // Internal ADC for CJR
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    DMA_DeInit(DMA1_Channel1);

    // 默认: 外设基址0, 内存基址0, 方向为外设源, 缓存大小0, 缓存自增disable
    // 内存自增disable, 外设数据大小byte, 内存数据大小byte, 模式normal
    // 优先级low, 内存到内存disable
    DMA_StructInit(&DMA_InitStructure);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32) & (ADC1->DR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)(g.cjr_buffer);
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = MAX_CJR_SAMPLES;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
    DMA_ClearITPendingBit(DMA_IT_TC);
    DMA_Cmd(DMA1_Channel1, ENABLE);
    ADC_DeInit(ADC1);

    RCC_ADCCLKConfig(RCC_PCLK2_Div2);
    ADC_StructInit(&ADC_InitStructure);
    ADC_DeInit(ADC1);

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 2;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 2, ADC_SampleTime_239Cycles5);
    ADC_ResetCalibration(ADC1);
    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);

    ( {  while(ADC_GetResetCalibrationStatus(ADC1));});
    ADC_StartCalibration(ADC1);
    ( {  while(ADC_GetCalibrationStatus(ADC1));});
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void ADCDMA_Conv_Complete_IRQHandler(void)
{
    static float sum_ref = 0, sum_volt = 0;
    static int count = 0;
    for(int i = 0; i < MAX_CJR_SAMPLES; i += 2) {
        sum_ref += g.cjr_buffer[i];
        sum_volt += g.cjr_buffer[i + 1];
    }
    count++;
    if(count == 100) {
        g.result.volt = sum_volt / sum_ref * 2.5;
        sum_ref = 0;
        sum_volt = 0;
        count = 0;
    }
}
