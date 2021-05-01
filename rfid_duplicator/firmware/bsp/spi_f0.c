#include "misc.h"

unsigned char SPI1_ReadWrite(unsigned char data)
{
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    *(unsigned char*)&(SPI1->DR) = data;
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    return *(unsigned char*)&(SPI1->DR);
}

void SPI1_Config(void)
{
    SPI_InitTypeDef sis;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    SPI_StructInit(&sis);
    sis.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    sis.SPI_Mode = SPI_Mode_Master;
    sis.SPI_DataSize = SPI_DataSize_8b;
    sis.SPI_CPOL = SPI_CPOL_Low;
    sis.SPI_CPHA = SPI_CPHA_1Edge;
    sis.SPI_NSS = SPI_NSS_Soft;
    sis.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    sis.SPI_FirstBit = SPI_FirstBit_MSB;
    sis.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &sis);
    SPI_RxFIFOThresholdConfig(SPI1, SPI_RxFIFOThreshold_QF);
    SPI_Cmd(SPI1, ENABLE);
}
