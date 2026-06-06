#ifndef _SPI_HAL_H
#define _SPI_HAL_H

#include "misc.h"

void SPI3_Config(void);
void SPI3_Write(const void* msg, int size);
void SPI3_Write_DMA(const void* msg, int size);

#endif

