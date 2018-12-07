#ifndef _SPIFLASH_CONF_H
#define _SPIFLASH_CONF_H

void SPIFLASH_SetCS(int cs);
unsigned char SPIFLASH_WriteByte(unsigned char data);
void SPIFLASH_Config(void);

#endif
