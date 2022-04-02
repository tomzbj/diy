#include "misc.h"

unsigned char SPI2_ReadWrite(unsigned char data)
{
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
    SPI_SendData8(SPI2, data);
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
    return SPI_ReceiveData8(SPI2);
}

void SPI2_Read_DMA(unsigned char* buf, int nbytes)
{
    DMA_Cmd(DMA1_Channel4, DISABLE);
    DMA_Cmd(DMA1_Channel5, DISABLE);
    DMA1_Channel4->CMAR = (unsigned long)(buf);
    DMA1_Channel4->CNDTR = nbytes;
    DMA1_Channel5->CNDTR = nbytes;
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
    DMA_InitTypeDef dis;

    // SPI2 for spiflash
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    // SPI2 for spiflash
    SPI_StructInit(&sis);
    sis.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
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

    static unsigned char dummy = 0xff;
    DMA_DeInit(DMA1_Channel4);    // SPI2 rx
    DMA_DeInit(DMA1_Channel5);    // SPI2 tx
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
    dis.DMA_MemoryBaseAddr = (unsigned long)(&dummy);
    dis.DMA_MemoryInc = DMA_MemoryInc_Disable;
    dis.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_Init(DMA1_Channel5, &dis);

    printf("[%s: %d] SPI Initialized.\n", __FILE__, __LINE__);
}
