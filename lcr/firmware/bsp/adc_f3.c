#include "misc.h"
#include "platform.h"

//#define ADC_CDR_ADDRESS    ((uint32_t)0x5000030C)

void ADC_Config(void)
{
    ADC_InitTypeDef ais;
    ADC_CommonInitTypeDef acis;
    DMA_InitTypeDef dis;
    volatile unsigned long tmp = tmp;
    DataConfig_t* pdc = DC_Get();

    RCC_ADCCLKConfig(RCC_ADC12PLLCLK_Div1);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    dis.DMA_PeripheralBaseAddr = (unsigned long)&(ADC1_2->CDR);
    dis.DMA_MemoryBaseAddr = (unsigned long)(pdc->data.adc_buf); //(uint32_t)adc_buf;
    dis.DMA_DIR = DMA_DIR_PeripheralSRC;
    dis.DMA_BufferSize = DC_NUM_OF_SAMPLES;
    dis.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dis.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dis.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    dis.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
    dis.DMA_Mode = DMA_Mode_Circular;
    dis.DMA_Priority = DMA_Priority_Medium;
    dis.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &dis);

    ADC_StructInit(&ais);
    ADC_VoltageRegulatorCmd(ADC1, ENABLE);
    ADC_VoltageRegulatorCmd(ADC2, ENABLE);
    _delay_us(100);
    ADC_SelectCalibrationMode(ADC1, ADC_CalibrationMode_Single);
    ADC_StartCalibration(ADC1);
    ADC_SelectCalibrationMode(ADC2, ADC_CalibrationMode_Single);
    ADC_StartCalibration(ADC2);
    while(ADC_GetCalibrationStatus(ADC1) != RESET);
    tmp = ADC_GetCalibrationValue(ADC1);
    while(ADC_GetCalibrationStatus(ADC2) != RESET);
    tmp = ADC_GetCalibrationValue(ADC2);
    acis.ADC_Mode = ADC_Mode_RegSimul; //ADC_Mode_Interleave;
    acis.ADC_Clock = ADC_Clock_AsynClkMode;
    acis.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
    acis.ADC_DMAMode = ADC_DMAMode_Circular;
    acis.ADC_TwoSamplingDelay = 10;
    ADC_CommonInit(ADC1, &acis);
    ais.ADC_ContinuousConvMode = ADC_ContinuousConvMode_Disable;
    ais.ADC_Resolution = ADC_Resolution_12b;
    ais.ADC_ExternalTrigConvEvent = ADC_ExternalTrigConvEvent_14;
    ais.ADC_ExternalTrigEventEdge = ADC_ExternalTrigEventEdge_RisingEdge;
    ais.ADC_DataAlign = ADC_DataAlign_Right;
    ais.ADC_OverrunMode = ADC_OverrunMode_Disable;
    ais.ADC_AutoInjMode = ADC_AutoInjec_Disable;
    ais.ADC_NbrOfRegChannel = 1;
    ADC_Init(ADC1, &ais);
    ADC_Init(ADC2, &ais);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_19Cycles5);
    ADC_RegularChannelConfig(ADC2, ADC_Channel_3, 1, ADC_SampleTime_19Cycles5);
    ADC_DMAConfig(ADC1, ADC_DMAMode_Circular);
    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);
    ADC_Cmd(ADC2, ENABLE);
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_RDY));
    while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_RDY));
    DMA_Cmd(DMA1_Channel1, DISABLE);
    ADC_StartConversion(ADC1);
}

