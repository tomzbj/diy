#include "misc.h"
#include "platform.h"

#define USART_USB USART1
#define MAX_LEN 128

void USART_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    USART_InitTypeDef uis;
    NVIC_InitTypeDef nis;

    USART_StructInit(&uis);

    // PB2 as USART1_TXEN
    nis.NVIC_IRQChannel = USART1_IRQn;

    nis.NVIC_IRQChannelPreemptionPriority = 0;
    nis.NVIC_IRQChannelSubPriority = 1;
    nis.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nis);

    USART_DeInit(USART1);
    uis.USART_WordLength = USART_WordLength_8b;
    uis.USART_BaudRate = 500000UL;
    uis.USART_StopBits = USART_StopBits_1;
    uis.USART_Parity = USART_Parity_No;
    uis.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    uis.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART1, &uis);

    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
    USART_ITConfig(USART1, USART_IT_ORE, ENABLE);
    USART_ITConfig(USART1, USART_IT_FE, ENABLE);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);
}

unsigned char USART_ReadByte(int usart_id)
{
    return USART1->DR;
}

void USART_WriteByte(unsigned char data, int usart_id)
{
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, data);
}
