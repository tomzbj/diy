#include "misc.h"
#include "platform.h"

void ADC_Config(void)
{
    NVIC_InitTypeDef nis;
    DMA_InitTypeDef dis;
    ADC_InitTypeDef ais;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

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
    ADC_ChannelConfig(ADC1, ADC_Channel_3, ADC_SampleTime_239_5Cycles);
    ADC_GetCalibrationFactor(ADC1);
    ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);
    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN));
    ADC_StartOfConversion(ADC1);
}
