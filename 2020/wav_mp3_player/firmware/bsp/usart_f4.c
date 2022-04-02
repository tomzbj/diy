#include <string.h>
#include "misc.h"

#define MAX_LEN 256

static struct {
    unsigned char u1_rx_buf[MAX_LEN];
    unsigned char msg[MAX_LEN];
    int flag_updated;
    int size;
    void (*parser_f)(const unsigned char* msg, int size);
} g;

void USART_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

    USART_InitTypeDef uis;
    NVIC_InitTypeDef nis;
    DMA_InitTypeDef dis;

    nis.NVIC_IRQChannelPreemptionPriority = 1;
    nis.NVIC_IRQChannelSubPriority = 1;
    nis.NVIC_IRQChannelCmd = ENABLE;
    nis.NVIC_IRQChannel = USART1_IRQn;
    NVIC_Init(&nis);

    DMA_DeInit(DMA2_Stream5);
    dis.DMA_Channel = DMA_Channel_4;
    dis.DMA_FIFOMode = DMA_FIFOMode_Disable;
    dis.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
    dis.DMA_Memory0BaseAddr = (unsigned long) g.u1_rx_buf;
    dis.DMA_DIR = DMA_DIR_PeripheralToMemory;    // 单向，外设源
    dis.DMA_BufferSize = MAX_LEN;
    dis.DMA_PeripheralBaseAddr = (unsigned long) (&USART1->DR);
    dis.DMA_PeripheralInc = DMA_PeripheralInc_Disable;    // 禁止外设递增
    dis.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    dis.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    dis.DMA_MemoryInc = DMA_MemoryInc_Enable;    // 允许内存递增
    dis.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    dis.DMA_MemoryBurst = DMA_MemoryBurst_Single;

    dis.DMA_Mode = DMA_Mode_Circular;
    dis.DMA_Priority = DMA_Priority_VeryHigh;
//    dis.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA2_Stream5, &dis);
    DMA_Cmd(DMA2_Stream5, ENABLE);
    USART_StructInit(&uis);
    USART_DeInit(USART1);
    uis.USART_BaudRate = 500000UL;
    uis.USART_WordLength = USART_WordLength_8b;
    uis.USART_StopBits = USART_StopBits_1;
    uis.USART_Parity = USART_Parity_No;
    uis.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    uis.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART1, &uis);
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
    USART_ITConfig(USART1, USART_IT_ORE, ENABLE);
    USART_ITConfig(USART1, USART_IT_FE, ENABLE);
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
    USART_Cmd(USART1, ENABLE);
}

void USART_BindParser(void (*parser_f)(const unsigned char* msg, int size))
{
    g.parser_f = parser_f;
}

void USART_Poll(void)
{
    if(g.flag_updated) {
        g.parser_f(g.msg, g.size);
        g.flag_updated = 0;
    }
}

void USART_WriteByte(USART_TypeDef* USARTx, unsigned char byte)
{
    ( {  while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);});
    USART_SendData(USARTx, byte);
    ( {  while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);});
}

void USART_RX_IDLE_IRQHandler(USART_TypeDef* USARTx)
{
    DMA_Stream_TypeDef* DMA_Streamx = NULL;
    unsigned char* usartx_rx_buf = NULL;
    int size = size;
    if(USARTx == USART1) {
        DMA_Streamx = DMA2_Stream5;
        usartx_rx_buf = g.u1_rx_buf;
    }

    DMA_Cmd(DMA_Streamx, DISABLE);    // 关闭 DMA, 防止处理期间有数据
    size = USARTx->DR;
    size = USARTx->SR;
    size = MAX_LEN - DMA_GetCurrDataCounter(DMA_Streamx);
    DMA_SetCurrDataCounter(DMA_Streamx, MAX_LEN);

    // do parse by frame metadata, not by usart?
    if(!g.flag_updated) {
        memcpy(g.msg, usartx_rx_buf, MAX_LEN);
        g.size = size;
        g.flag_updated = 1;
    }
    bzero(usartx_rx_buf, MAX_LEN);
    DMA_Cmd(DMA_Streamx, ENABLE);    // 关闭 DMA, 防止处理期间有数据
}
