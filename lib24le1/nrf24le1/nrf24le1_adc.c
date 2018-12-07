#include "nrf24le1.h"

void ADC_Init(ADC_InitTypeDef* ADC_InitStructure)
{
    ADCCON1 = *(unsigned char*)ADC_InitStructure;
    ADCCON2 = *((unsigned char*)ADC_InitStructure + 1);
    ADCCON3 = *((unsigned char*)ADC_InitStructure + 2);
}

void ADC_SelectChannel(ADC_ChannelTypeDef channel)
{
    ADCCON1 = (ADCCON1 & 0xc3) | (channel << 2);
}

void ADC_StartConversion(void)
{
    ADCCON1 |= _BV(7);
    nop();
    nop();
    nop();
    nop();
}

unsigned short ADC_ReadData(void)
{
    while(ADCCON1 & _BV(6));        // Wait if ADC busy
    return ADCDAT;
}
