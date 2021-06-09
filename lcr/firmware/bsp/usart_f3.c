#include "platform.h"
#include "misc.h"

void USART_Config(void)
{
    NVIC_InitTypeDef nis;

    RCC_APB1PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    nis.NVIC_IRQChannel = USART1_IRQn;
    nis.NVIC_IRQChannelPreemptionPriority = 1;
    nis.NVIC_IRQChannelSubPriority = 1;
    nis.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nis);

    USART_InitTypeDef uis;
    USART_Cmd(USART1, DISABLE);
    USART_StructInit(&uis);
    uis.USART_WordLength = USART_WordLength_8b;
    uis.USART_StopBits = USART_StopBits_1;
    uis.USART_Parity = USART_Parity_No;
    uis.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    uis.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    uis.USART_BaudRate = 500000UL;
    USART_Init(USART1, &uis);
    USART_Cmd(USART1, ENABLE);

    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART1, USART_IT_ORE, ENABLE);
    USART_ITConfig(USART1, USART_IT_FE, ENABLE);
}

unsigned char USART_ReadByte(int source)
{
    return USART1->RDR;
}
void USART_WriteByte(unsigned char c, int source)
{
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART1->TDR = c;
//    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}
