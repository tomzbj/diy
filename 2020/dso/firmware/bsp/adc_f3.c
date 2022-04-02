#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "misc.h"
#include "platform.h"

void ADC_Config(void* adc_buf)
{
    DMA_InitTypeDef dis;
    ADC_InitTypeDef ais;
    ADC_CommonInitTypeDef acis;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12, ENABLE);

    DMA_DeInit(DMA1_Channel1);
    DMA_StructInit(&dis);

    dis.DMA_PeripheralBaseAddr = (unsigned long)&(ADC1->DR);
    dis.DMA_MemoryBaseAddr = (unsigned long)adc_buf;
    dis.DMA_DIR = DMA_DIR_PeripheralSRC;
    dis.DMA_BufferSize = DC_ADC_BUF_SIZE;
    dis.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dis.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dis.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    dis.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    dis.DMA_Mode = DMA_Mode_Circular;
    dis.DMA_Priority = DMA_Priority_VeryHigh;
    dis.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &dis);

    ADC_DeInit(ADC1);
    RCC_ADCCLKConfig(RCC_ADC12PLLCLK_Div1);
    ADC_StructInit(&ais);

    unsigned long calibration_value = calibration_value;
    ADC_VoltageRegulatorCmd(ADC1, ENABLE); /* Calibration procedure */
    _delay_ms_loop(10);
    ADC_SelectCalibrationMode(ADC1, ADC_CalibrationMode_Single);
    ADC_StartCalibration(ADC1);

    while(ADC_GetCalibrationStatus(ADC1) != RESET);
    calibration_value = ADC_GetCalibrationValue(ADC1);

    acis.ADC_Mode = ADC_Mode_Independent;
    acis.ADC_Clock = ADC_Clock_SynClkModeDiv1;
    acis.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    acis.ADC_DMAMode = ADC_DMAMode_Circular;
    acis.ADC_TwoSamplingDelay = 0;
    ADC_CommonInit(ADC1, &acis);

    ais.ADC_Resolution = ADC_Resolution_8b;
    ais.ADC_ContinuousConvMode = ADC_ContinuousConvMode_Disable;
    ais.ADC_ExternalTrigConvEvent = ADC_ExternalTrigConvEvent_13;
    ais.ADC_ExternalTrigEventEdge = ADC_ExternalTrigEventEdge_RisingEdge;
    ais.ADC_DataAlign = ADC_DataAlign_Right;
    ais.ADC_OverrunMode = ADC_OverrunMode_Disable;
    ais.ADC_AutoInjMode = ADC_AutoInjec_Disable;
    ais.ADC_NbrOfRegChannel = 1;

    ADC_Init(ADC1, &ais);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_1Cycles5);    // A1_C3 for OPA1_OUT
    ADC_Cmd(ADC1, ENABLE);
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_RDY));

    ADC_DMAConfig(ADC1, ADC_DMAMode_Circular);
    DMA_Cmd(DMA1_Channel1, ENABLE);
    ADC_DMACmd(ADC1, ENABLE);
    ADC_StartConversion(ADC1);
}
