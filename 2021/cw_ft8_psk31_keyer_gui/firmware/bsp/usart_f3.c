#include "platform.h"
#include "misc.h"

void USART_Config(void)
{
    NVIC_InitTypeDef nis;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    // USART1 as SPI for XPT2046
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    nis.NVIC_IRQChannel = USART2_IRQn;
    nis.NVIC_IRQChannelPreemptionPriority = 1;
    nis.NVIC_IRQChannelSubPriority = 1;
    nis.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nis);

    USART_InitTypeDef uis;
    USART_Cmd(USART2, DISABLE);
    USART_Cmd(USART1, DISABLE);
    USART_StructInit(&uis);
    uis.USART_WordLength = USART_WordLength_8b;
    uis.USART_StopBits = USART_StopBits_1;
    uis.USART_Parity = USART_Parity_No;
    uis.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    uis.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    uis.USART_BaudRate = 500000UL;
    USART_Init(USART2, &uis);
    USART_Cmd(USART2, ENABLE);

    {
        USART_ClockInitTypeDef ucis;
        ucis.USART_Clock = USART_Clock_Enable;
        ucis.USART_CPOL = USART_CPOL_High;
        ucis.USART_CPHA = USART_CPHA_2Edge;
        ucis.USART_LastBit = USART_LastBit_Enable;
        USART_ClockInit(USART1, &ucis);
    }

    uis.USART_BaudRate = 150000UL;
    USART_MSBFirstCmd(USART1, ENABLE);
    USART_Init(USART1, &uis);
    USART_Cmd(USART1, ENABLE);

    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART2, USART_IT_ORE, ENABLE);
    USART_ITConfig(USART2, USART_IT_FE, ENABLE);

    printf("\n\n");
    printf("[%s: %d] USART Initialized.\n", __FILE__, __LINE__);
}

unsigned char USART_ReadByte(int source)
{
    return USART2->RDR;
}

void USART_WriteByte(unsigned char c, int source)
{
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    USART2->TDR = c;
}

unsigned char SPI_USART1_ReadWrite(unsigned char data)
{
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART1->TDR = data;
    while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
    return USART1->RDR;
}
