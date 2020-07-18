#ifndef _SPI_F3_H
#define _SPI_F3_H

void SPI_Config(void);
void SPI2_SetCS(int cs);
void SPI2_Write_DMA(void* buf, int num);
void SPI2_Read_DMA(void* buf, int num);
void SPI3_Write_DMA(void* buf, int num);
unsigned char SPI1_WriteByte(unsigned char data);

#endif
