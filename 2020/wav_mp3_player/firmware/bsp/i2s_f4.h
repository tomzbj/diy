#ifndef _I2S_F0_H
#define _I2S_F0_H

void I2S_Config(void);
void I2S_Set(unsigned short data);
void I2S_Write_DMA(void* buf, int nbytes);
void I2S_Write_DMA_Stop(void);

#endif
