#include "platform.h"
#include "misc.h"

const typeof(USART2) usart[1] = {USART2};

void USART_WriteByte(unsigned char c, int source)
{
    if(source >= 1)
        return;
    while(USART_GetFlagStatus(usart[source], USART_FLAG_TXE) == RESET);
    usart[source]->TDR = c;
}

unsigned char USART_ReadByte(int source)
{
    return (source >= 1) ? 0 : (usart[source]->RDR);
//    if(source >= 2) return 0; return usart[source]->RDR;
}

void USART_SetBaudrate(unsigned long baudrate)
{
    __disable_irq();
    USART_Cmd(USART2, DISABLE);
    USART2->BRR = 48000000UL / baudrate;
    USART_Cmd(USART2, ENABLE);
    __enable_irq();
}

void USART_Config(void)
{
    NVIC_InitTypeDef nis;
    USART_InitTypeDef uis;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    USART_DeInit(USART2);
    USART_StructInit(&uis);
    uis.USART_WordLength = USART_WordLength_8b;
    uis.USART_StopBits = USART_StopBits_1;
    uis.USART_Parity = USART_Parity_No;
    uis.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    uis.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    uis.USART_BaudRate = 500000UL;
    USART_Init(USART2, &uis);

    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
    USART_ITConfig(USART2, USART_IT_ORE, ENABLE);
    USART_ITConfig(USART2, USART_IT_FE, ENABLE);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART2, USART_IT_NE, ENABLE);
    USART_Cmd(USART2, ENABLE);

    nis.NVIC_IRQChannelPriority = 0;
    nis.NVIC_IRQChannelCmd = ENABLE;
    nis.NVIC_IRQChannel = USART2_IRQn;    //USARTn_IRQChannel;
    NVIC_Init(&nis);
//    while(1) { USART_WriteByte('K', 0); _delay_ms_loop(100); }
//    xdev_out(USART_WriteByte);

    printf("\n");
    printf("[%s: %d] USART Initialized.\n", __FILE__, __LINE__);
}
