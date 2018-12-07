#include "nrf24le1.h"

void SPI_Init(SPI_InitTypeDef* SPI_InitStructure)
{
    SPIMCON0 = *(unsigned char*) SPI_InitStructure;
}

unsigned char SPI_WriteByte(unsigned char data)
{
    unsigned char reg;

    while(SPIMSTAT & _BV(2))
        reg = SPIMDAT;

    SPIMDAT = data;
    while((SPIMSTAT & _BV(2)) == 0);

    return SPIMDAT;
}
