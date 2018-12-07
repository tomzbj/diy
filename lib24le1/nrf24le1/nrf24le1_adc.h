#ifndef _NRF24LE1_ADC_H_
#define _NRF24LE1_ADC_H_

#include "nrf24le1.h"

typedef enum {
    ADC_Channel_0 = 0, ADC_Channel_1 = 1, ADC_Channel_2 = 2, ADC_Channel_3 = 3,
    ADC_Channel_4 = 4, ADC_Channel_5 = 5, ADC_Channel_6 = 6, ADC_Channel_7 = 7,
    ADC_Channel_8 = 8, ADC_Channel_9 = 9, ADC_Channel_10 = 10,
    ADC_Channel_11 = 11, ADC_Channel_12 = 12, ADC_Channel_13 = 13,
    ADC_Channel_1_3_VDD = 14, ADC_Channel_2_3_VDD = 15
} ADC_ChannelTypeDef;

typedef enum {
    ADC_Reference_1V2 = 0, ADC_Reference_VDD = 1, ADC_Reference_AIN3 = 2,
    ADC_Reference_AIN9 = 3
} ADC_ReferenceTypeDef;

typedef enum {
    ADC_DifferentialMode_Off= 0, ADC_DifferentialMode_AIN2_Inverted = 1,
    ADC_DifferentialMode_AIN6_Inverted = 2
} ADC_DifferentialModeTypeDef;

typedef enum {
    ADC_DataRate_2k = 0, ADC_DataRate_4k = 1, ADC_DataRate_8k = 2,
    ADC_DataRate_16k = 3
} ADC_DataRateTypeDef;

typedef enum {
    ADC_InputAcquisitionTime_0_75us = 0,
    ADC_InputAcquisitionTime_3us = 1,
    ADC_InputAcquisitionTime_12us = 2,
    ADC_InputAcquisitionTime_36us = 3
} ADC_InputAcquisitionTimeTypeDef;

typedef enum {
    ADC_Resolution_6bits = 0,
    ADC_Resolution_8bits = 1,
    ADC_Resolution_10bits = 2,
    ADC_Resolution_12bits = 3
} ADC_ResolutionTypeDef;

typedef enum {
    ADC_DataAlign_Left = 0, ADC_DataAlign_Right = 1
} ADC_DataAlignTypeDef;

typedef struct {
    // ADCCON1
    unsigned char ADC_Reference:2;
    unsigned char ADC_Channel:4;
    unsigned char reserved1:2;
    // ADCCON2
    unsigned char ADC_InputAcquisitionTime:2;
    unsigned char ADC_DataRate:3;
    unsigned char ADC_ContinueousConvMode:1;
    unsigned char ADC_DifferentialMode:2;
    // ADCCON3
    unsigned char reserved2:5;
    unsigned char ADC_DataAlign:1;
    unsigned char ADC_Resolution:2;
} ADC_InitTypeDef;

void ADC_Init(ADC_InitTypeDef* ADC_InitStructure);
void ADC_SelectChannel(ADC_ChannelTypeDef channel);
void ADC_StartConversion(void);
unsigned short ADC_ReadData(void);

#endif
