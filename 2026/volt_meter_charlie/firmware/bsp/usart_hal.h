#ifndef _USART_HAL_H
#define _USART_HAL_H

void USART_Config(void);
void USART1_WriteByte(uint8_t c);
uint8_t USART1_ReadByte(void);
void USART1_Write(uint8_t* data, int count);

#endif
