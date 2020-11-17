#include "misc.h"

static struct {
    unsigned short data;
} g;

void I2S_Set(unsigned short data)
{
    g.data = data;
}

void I2S_Write_DMA_Stop(void)
{
    DMA_Cmd(DMA1_Stream4, DISABLE);
    SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, DISABLE);
}

void I2S_Write_DMA(void* buf, int nbytes)
{
    SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, DISABLE);
    DMA_Cmd(DMA1_Stream4, DISABLE);
    DMA1_Stream4->NDTR = nbytes / 2;
    DMA1_Stream4->M0AR = (unsigned long)(buf);
    DMA_Cmd(DMA1_Stream4, ENABLE);
    SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);
//    ( {  while(DMA_GetFlagStatus(DMA1_Stream4, DMA_FLAG_TCIF4) == RESET);});
//    DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_TCIF4);
}

void I2S_Config(void)
{
    I2S_InitTypeDef i2s_is;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

//    RCC_PLLI2SCmd(DISABLE);
    RCC_PLLI2SConfig(271, 2);
    RCC_I2SCLKConfig(RCC_I2S2CLKSource_PLLI2S);
    RCC_PLLI2SCmd(ENABLE);

    while(RCC_GetFlagStatus(RCC_FLAG_PLLI2SRDY) == RESET);
//        RCC_I2SCLKSource_PLLI2S);

    i2s_is.I2S_Standard = I2S_Standard_Phillips;
    i2s_is.I2S_DataFormat = I2S_DataFormat_16bextended;
//    i2s_is.I2S_DataFormat = I2S_DataFormat_16b;
//    i2s_is.I2S_MCLKOutput = I2S_MCLKOutput_Disable;
    i2s_is.I2S_MCLKOutput = I2S_MCLKOutput_Enable;
    i2s_is.I2S_AudioFreq = I2S_AudioFreq_44k;
    i2s_is.I2S_CPOL = I2S_CPOL_High;
    i2s_is.I2S_Mode = I2S_Mode_MasterTx;
    I2S_Init(SPI2, &i2s_is);

    DMA_InitTypeDef dis;

    DMA_DeInit(DMA1_Stream4);
    DMA_StructInit(&dis);
    dis.DMA_Channel = DMA_Channel_0;
    dis.DMA_PeripheralBaseAddr = (unsigned long)&(SPI2->DR);
    dis.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dis.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dis.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    dis.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    dis.DMA_Mode = DMA_Mode_Circular;
    dis.DMA_Priority = DMA_Priority_VeryHigh;
//    dis.DMA_M2M = DMA_M2M_Disable;
//    dis.DMA_DIR = DMA_DIR_PeripheralDST;
//    dis.DMA_DIR = DMA_DIR_PeripheralToMemory;
    dis.DMA_DIR = DMA_DIR_MemoryToPeripheral;
//    dis.DMA_BufferSize = 1;
//    dis.DMA_MemoryBaseAddr = (unsigned long)(&g.data);
    dis.DMA_Memory0BaseAddr = (unsigned long)(&g.data);
//    SYSCFG_DMAChannelRemapConfig(SYSCFG_DMARemap_SPI2, ENABLE);
    dis.DMA_FIFOMode = DMA_FIFOMode_Disable;    //不使用FIFO模式
    dis.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
    dis.DMA_MemoryBurst = DMA_MemoryBurst_Single;    //外设突发单次传输
    dis.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;    //存储器突发单次传输

    DMA_Init(DMA1_Stream4, &dis);
    DMA_ClearITPendingBit(DMA1_Stream4, DMA_IT_TCIF4);
    DMA_ClearITPendingBit(DMA1_Stream4, DMA_IT_HTIF4);
    DMA_ITConfig(DMA1_Stream4, DMA_IT_TC, ENABLE);    //开启传输完成中断
    DMA_ITConfig(DMA1_Stream4, DMA_IT_HT, ENABLE);    //开启传输完成中断

//    DMA_Cmd(DMA1_Stream4, ENABLE);
//    SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);
    I2S_Cmd(SPI2, ENABLE);
//    ( {  while(DMA_GetFlagStatus(DMA1_FLAG_TC5) == RESET);});

    NVIC_InitTypeDef nis;

    nis.NVIC_IRQChannelPreemptionPriority = 1;
    nis.NVIC_IRQChannelSubPriority = 1;
    nis.NVIC_IRQChannelCmd = ENABLE;
    nis.NVIC_IRQChannel = DMA1_Stream4_IRQn;
    NVIC_Init(&nis);
}
