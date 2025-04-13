#ifndef _USART_H
#define _USART_H

void USART_Config(void);
uint8_t USART0_ReadByte(void);
void USART0_WriteByte(uint8_t c);
void USART0_Write(uint8_t* data, int count);
void USART0_Write_DMA(uint8_t* data, int count);

#endif
