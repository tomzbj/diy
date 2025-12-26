#ifndef _USART2_HAL_H
#define _USART2_HAL_H

void USART2_Config(void);
void USART2_WriteByte(uint8_t c);
uint8_t USART2_ReadByte(void);
void USART2_Write(uint8_t* data, int count);

#endif
