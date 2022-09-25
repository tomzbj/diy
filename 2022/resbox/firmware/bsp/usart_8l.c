#include "misc.h"
#include "platform.h"

char putchar(char c)
{
    USART_SendData8(USART1, c);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    return c;
}

void USART_WriteByte(unsigned char c, int source)
{
    USART1->DR = c;
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}

unsigned char USART_ReadByte(int source)
{
    return USART1->DR;
}

void USART_Config(void)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE);
//    SYSCFG_REMAPPinConfig(REMAP_Pin_USART1TxRxPortA, ENABLE);
    USART_Init(USART1, 115200UL, USART_WordLength_8b, USART_StopBits_1,
        USART_Parity_No, USART_Mode_Tx | USART_Mode_Rx);
    USART_Cmd(USART1, ENABLE);
//    xdev_out(putchar);
//    while(1) { xprintf("Hello, world.\n"); _delay_ms(500); }
}
