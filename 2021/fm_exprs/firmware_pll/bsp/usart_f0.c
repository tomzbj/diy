#include "platform.h"
#include "misc.h"

void USART_WriteByte(unsigned char c, int source)
{
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART1->TDR = c;
}

unsigned char USART_ReadByte(int source)
{
    return USART1->RDR;
}

void USART_Config(void)
{
    NVIC_InitTypeDef nis;
    USART_InitTypeDef uis;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    USART_DeInit(USART1);
    USART_StructInit(&uis);
    uis.USART_WordLength = USART_WordLength_8b;
    uis.USART_StopBits = USART_StopBits_1;
    uis.USART_Parity = USART_Parity_No;
    uis.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    uis.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    uis.USART_BaudRate = 500000UL;
//    USART_SWAPPinCmd(USART1, DISABLE);
    USART_Init(USART1, &uis);
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
    USART_ITConfig(USART1, USART_IT_ORE, ENABLE);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART1, USART_IT_FE, ENABLE);
//    USART_ITConfig(USART1, USART_IT_TC, ENABLE);
    USART_Cmd(USART1, ENABLE);

    nis.NVIC_IRQChannelPriority = 1;
    nis.NVIC_IRQChannelCmd = ENABLE;
    nis.NVIC_IRQChannel = USART1_IRQn;    //USARTn_IRQChannel;
    NVIC_Init(&nis);
}
