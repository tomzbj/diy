#ifndef _VS1003_PORT_H
#define _VS1003_PORT_H

#define VS1003_SPI_LS 1001
#define VS1003_SPI_HS 1002

void VS1003_SetDCS(int dcs);
void VS1003_SetCCS(int xcs);
void VS1003_SetRESET(int xreset);
int VS1003_GetDREQ(void);
void VS1003_Config(void);
void VS1003_SPI_Config(int speed);
unsigned char VS1003_WriteByte(unsigned char byte);

#endif
