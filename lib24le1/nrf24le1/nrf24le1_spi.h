#ifndef _NRF24LE1_SPI_H_
#define _NRF24LE1_SPI_H_ 

typedef enum {
    SPI_BaudRatePrescaler_2 = 0x0, SPI_BaudRatePrescaler_4 = 0x1,
    SPI_BaudRatePrescaler_8 = 0x2, SPI_BaudRatePrescaler_16 = 0x3,
    SPI_BaudRatePrescaler_32 = 0x4, SPI_BaudRatePrescaler_64 = 0x5
} SPI_BaudRatePrescalerTypeDef;

typedef enum {
    SPI_FirstBit_MSB = 0, SPI_FirstBit_LSB = 1
} SPI_FirstBitTypeDef;

typedef enum {
    SPI_CPOL_Low = 0, SPI_CPOL_High = 1
} SPI_CPOLTypeDef;

typedef enum {
    SPI_CPHA_1Edge = 0, SPI_CPHA_2Edge = 1
} SPI_CPHATypeDef;

typedef struct {
    unsigned char SPI_Master:1;
    unsigned char SPI_CPHA:1;
    unsigned char SPI_CPOL:1;
    unsigned char SPI_FirstBit:1;
    unsigned char SPI_BaudRatePrescaler:4;
} SPI_InitTypeDef;

void SPI_Init(SPI_InitTypeDef* SPI_InitStructure);
unsigned char SPI_WriteByte(unsigned char data);
#define SPI_ReadByte() SPI_WriteByte(0xff);

#endif /*MSPI_H_*/
