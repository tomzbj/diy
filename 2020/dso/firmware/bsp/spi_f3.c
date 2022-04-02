#include <stdlib.h>
#include <stdio.h>
#include "misc.h"

void SPI1_WriteByte(unsigned char data)
{
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_SendData8(SPI1, data);
//    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
//    return SPI_ReceiveData8(SPI1);
}

unsigned char SPI2_WriteByte(unsigned char data)
{
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
    SPI_SendData8(SPI2, data);
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
    return SPI_ReceiveData8(SPI2);
}

unsigned char SPI3_WriteByte(unsigned char data)
{
    while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);
    SPI_SendData8(SPI3, data);
//    ( {  while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET);});
//    return SPI_ReceiveData8(SPI3);
    return 0;
}

static void SPI3_DMA_Config(void)
{
    DMA_InitTypeDef dis;
    DMA_DeInit(DMA2_Channel2);    // spi3 tx
    DMA_StructInit(&dis);
    dis.DMA_PeripheralBaseAddr = (unsigned long)&(SPI3->DR);
    dis.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dis.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dis.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    dis.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    dis.DMA_Mode = DMA_Mode_Normal;
    dis.DMA_Priority = DMA_Priority_VeryHigh;
    dis.DMA_M2M = DMA_M2M_Disable;
    dis.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_Init(DMA2_Channel2, &dis);
}

static void SPI2_DMA_Config(void)
{
    DMA_InitTypeDef dis;
    DMA_DeInit(DMA1_Channel4);    // spi2 rx
    DMA_DeInit(DMA1_Channel5);    // spi2 tx
    DMA_StructInit(&dis);
    dis.DMA_PeripheralBaseAddr = (unsigned long)&(SPI2->DR);
    dis.DMA_DIR = DMA_DIR_PeripheralSRC;
    dis.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dis.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dis.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    dis.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    dis.DMA_Mode = DMA_Mode_Normal;
    dis.DMA_Priority = DMA_Priority_VeryHigh;
    dis.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel4, &dis);
//    DMA_InitStructure.DMA_MemoryBaseAddr = (unsigned long)(&dummy);   // 不指定内存地址
//    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;  // 不指定地址自增
    dis.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_Init(DMA1_Channel5, &dis);
}

void SPI2_Write_DMA(void* buf, int num)
{
    unsigned short dummy = dummy;
    DMA_Cmd(DMA1_Channel5, DISABLE);
    DMA1_Channel5->CNDTR = num;
    DMA1_Channel5->CMAR = (unsigned long)(buf);
    DMA1_Channel5->CCR |= DMA_CCR_MINC;     // 地址自增打开
    DMA_Cmd(DMA1_Channel5, ENABLE);

    SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);
    while(DMA_GetFlagStatus(DMA1_FLAG_TC5) == RESET);
    DMA_ClearFlag(DMA1_FLAG_TC5);

    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE))
        SPI_ReceiveData8(SPI2);
}

void SPI3_Write_DMA(void* buf, int num)
{
    DMA_Cmd(DMA2_Channel2, DISABLE);
    DMA2_Channel2->CNDTR = num;
    DMA2_Channel2->CMAR = (unsigned long)(buf);
    DMA2_Channel2->CCR |= DMA_CCR_MINC;     // 地址自增打开
    DMA_Cmd(DMA2_Channel2, ENABLE);

    SPI_I2S_DMACmd(SPI3, SPI_I2S_DMAReq_Tx, ENABLE);
    while(DMA_GetFlagStatus(DMA2_FLAG_TC2) == RESET);
    DMA_ClearFlag(DMA2_FLAG_TC2);
}

void SPI2_Read_DMA(void* buf, int num)
{
    unsigned char dummy;
    dummy = 0xff;

    DMA_Cmd(DMA1_Channel4, DISABLE);
    DMA_Cmd(DMA1_Channel5, DISABLE);
    DMA1_Channel4->CMAR = (unsigned long)(buf);
    DMA1_Channel4->CNDTR = num;
    DMA1_Channel5->CMAR = (unsigned long)&dummy;
    DMA1_Channel5->CNDTR = num;
    DMA1_Channel5->CCR &= ~DMA_CCR_MINC;    // 地址自增关闭
    DMA_Cmd(DMA1_Channel4, ENABLE);
    DMA_Cmd(DMA1_Channel5, ENABLE);

    SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx | SPI_I2S_DMAReq_Rx, ENABLE);
    while(DMA_GetFlagStatus(DMA1_FLAG_TC4) == RESET);
    DMA_ClearFlag(DMA1_FLAG_TC4);
    DMA_ClearFlag(DMA1_FLAG_TC5);
}

void SPI_Config(void)
{
    SPI_InitTypeDef sis;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

    SPI_StructInit(&sis);
    sis.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    sis.SPI_Mode = SPI_Mode_Master;
    sis.SPI_DataSize = SPI_DataSize_8b;
    sis.SPI_CPOL = SPI_CPOL_High;
    sis.SPI_CPHA = SPI_CPHA_2Edge;
    sis.SPI_NSS = SPI_NSS_Soft;
    sis.SPI_FirstBit = SPI_FirstBit_MSB;
    sis.SPI_CRCPolynomial = 7;

    sis.SPI_Direction = SPI_Direction_1Line_Tx;
    sis.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_Init(SPI3, &sis);
    SPI_RxFIFOThresholdConfig(SPI3, SPI_RxFIFOThreshold_QF);
    SPI_Cmd(SPI3, ENABLE);
    SPI3_DMA_Config();

    sis.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
    SPI_RxFIFOThresholdConfig(SPI1, SPI_RxFIFOThreshold_QF);
    SPI_Init(SPI1, &sis);
    SPI_Cmd(SPI1, ENABLE);
}
