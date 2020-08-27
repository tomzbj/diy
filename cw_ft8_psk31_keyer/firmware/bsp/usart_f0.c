#include "platform.h"

#define MAX_LEN 128
#define USART_TOTAL_NUM 1

typedef struct {
    const USART_TypeDef* usartx;
    const typeof(USART1_IRQn) usartx_irqn;
    unsigned char msg[MAX_LEN];
    unsigned char rxbuf[MAX_LEN];
    unsigned char* rxptr;
    int size, updated;
} usartx_t;

static struct {
    usartx_t usart[USART_TOTAL_NUM];
    void (*parser_f)(const void* msg, int size, int source);
} g = {
    .usart = {
        [0] = {.usartx = USART1, .usartx_irqn = USART1_IRQn},
    }
};

void USART_BindParser(void (*parser_f)(const void* msg, int size, int source))
{
    g.parser_f = parser_f;
}

static void __usart_config(int usart_id, unsigned long baudrate)
{
    USART_TypeDef* usartx = (USART_TypeDef*)g.usart[usart_id].usartx;

    g.usart[usart_id].rxptr = g.usart[usart_id].rxbuf;
    g.usart[usart_id].size = 0;
    g.usart[usart_id].updated = 0;

    NVIC_InitTypeDef nis;
    nis.NVIC_IRQChannel = g.usart[usart_id].usartx_irqn;
    nis.NVIC_IRQChannelPriority = 1;
    nis.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nis);

    USART_InitTypeDef uis;
    USART_Cmd(usartx, DISABLE);
    USART_StructInit(&uis);
    uis.USART_WordLength = USART_WordLength_8b;
    uis.USART_StopBits = USART_StopBits_1;
    uis.USART_Parity = USART_Parity_No;
    uis.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    uis.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    uis.USART_BaudRate = baudrate;
    USART_Init(usartx, &uis);
    USART_Cmd(usartx, ENABLE);
    USART_ITConfig(usartx, USART_IT_IDLE, ENABLE);
    USART_ITConfig(usartx, USART_IT_RXNE, ENABLE);
    USART_ITConfig(usartx, USART_IT_ORE, ENABLE);
    USART_ITConfig(usartx, USART_IT_FE, ENABLE);
}

void USART_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    __usart_config(0, 500000UL);
}

void USART_WriteData(USART_TypeDef* USARTx, const void* data, int num)
{
    while(num--) {
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
        USART_SendData(USARTx, *((unsigned char*)data));
        data++;
    }
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
}

void USART_WriteByte(USART_TypeDef* USARTx, unsigned char byte)
{
    USART_WriteData(USARTx, &byte, 1);
}

void USART_Poll(void)
{
    for(int i = 0; i < USART_TOTAL_NUM; i++) {
        usartx_t* p = &(g.usart[i]);
        if(p->updated) {
            g.parser_f(p->msg, p->size, i);
            p->updated = 0;
        }
    }
}

void USART_RXNE_IRQHandler(USART_TypeDef* USARTx)
{
    for(int i = 0; i < USART_TOTAL_NUM; i++) {
        usartx_t* p = &(g.usart[i]);
        if(USARTx == p->usartx) {
            if(p->rxptr - p->rxbuf >= MAX_LEN)
                break;
            *(p->rxptr) = p->usartx->RDR;
            p->rxptr++;
            break;
        }
    }
}

void USART_RX_IDLE_IRQHandler(USART_TypeDef* USARTx)
{
    for(int i = 0; i < USART_TOTAL_NUM; i++) {
        usartx_t* p = &(g.usart[i]);
        if(USARTx == p->usartx) {
            p->size = p->rxptr - p->rxbuf;
            memcpy(p->msg, p->rxbuf, p->size);
            p->rxptr = p->rxbuf;
            p->updated = 1;
            break;
        }
    }
}
