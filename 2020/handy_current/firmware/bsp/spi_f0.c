#include <stdlib.h>
#include <stdio.h>
#include "misc.h"

void SPI2_SetCS(int cs)
{
    if(cs)
        GPIOB->BSRR = GPIO_Pin_0;
    else
        GPIOB->BRR = GPIO_Pin_0;
    _delay_us(10);
}

unsigned char SPI2_ReadWrite(unsigned char data)
{
    ( {  while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);});
    SPI_SendData8(SPI2, data);
    ( {  while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);});
    return SPI_ReceiveData8(SPI2);
}

static void SPI2_DMA_Config(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_InitTypeDef dis;
    DMA_DeInit(DMA1_Channel5);  // spi2 tx
    DMA_StructInit(&dis);
    dis.DMA_PeripheralBaseAddr = (unsigned long)&(SPI2->DR);
    dis.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dis.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dis.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    dis.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    dis.DMA_Mode = DMA_Mode_Normal;
    dis.DMA_Priority = DMA_Priority_VeryHigh;
    dis.DMA_M2M = DMA_M2M_Disable;
    dis.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_Init(DMA1_Channel5, &dis);
}

void SPI2_Write_DMA(unsigned char* buf, int nbytes)
{
    unsigned short dummy = dummy;
    DMA_Cmd(DMA1_Channel5, DISABLE);
    DMA1_Channel5->CNDTR = nbytes;
    DMA1_Channel5->CMAR = (unsigned long)(buf);
    DMA1_Channel5->CCR |= DMA_CCR_MINC;     // 地址自增打开
    DMA_Cmd(DMA1_Channel5, ENABLE);

    SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);
    ( {  while(DMA_GetFlagStatus(DMA1_FLAG_TC5) == RESET);});
    DMA_ClearFlag(DMA1_FLAG_TC5);

    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE))
        SPI_ReceiveData8(SPI2);
}

void SPI2_Config(void)
{
    SPI_InitTypeDef sis;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

    SPI2_SetCS(1);
    SPI_StructInit(&sis);
    sis.SPI_Direction = SPI_Direction_1Line_Tx;
    sis.SPI_Mode = SPI_Mode_Master;
    sis.SPI_DataSize = SPI_DataSize_8b;
    sis.SPI_CPOL = SPI_CPOL_High;
    sis.SPI_CPHA = SPI_CPHA_2Edge;
    sis.SPI_NSS = SPI_NSS_Soft;
    sis.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    sis.SPI_FirstBit = SPI_FirstBit_MSB;
    sis.SPI_CRCPolynomial = 7;
    SPI_Init(SPI2, &sis);
    SPI_RxFIFOThresholdConfig(SPI2, SPI_RxFIFOThreshold_QF);
    SPI_Cmd(SPI2, ENABLE);
    SPI2_DMA_Config();
}
