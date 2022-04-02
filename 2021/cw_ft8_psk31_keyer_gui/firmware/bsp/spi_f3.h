#ifndef _SPI_F3_H
#define _SPI_F3_H

void SPI_Config(void);

unsigned char SPI1_ReadWrite(unsigned char data);
void SPI1_Write_DMA(const void* buf, int num);

unsigned char SPI2_ReadWrite(unsigned char data);
void SPI2_Write_DMA(void* buf, int num);
void SPI2_Read_DMA(void* buf, int num);

#endif
