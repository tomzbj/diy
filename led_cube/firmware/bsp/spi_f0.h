#ifndef _SPI_F0_H
#define _SPI_F0_H

void SPI2_Read_DMA(void* buf, int nbytes);
void SPI1_Write_DMA(void* buf, int nbytes);
void SPI_Config(void);

#endif
