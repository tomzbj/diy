#ifndef _SPI_F0_H
#define _SPI_F0_H

void SPI2_Config(void);
void SPI2_SetCS(int cs);
unsigned char SPI2_ReadWrite(unsigned char data);
void SPI2_Write_DMA(unsigned char* buf, int num);
void SPI2_Read_DMA(unsigned char* buf, int num);

#endif
