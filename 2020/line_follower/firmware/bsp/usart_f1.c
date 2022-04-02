#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#include "misc.h"
#include "platform.h"

#define USART_USB USART1
#define MAX_LEN 1280

enum {
    FRAME_TYPE_NONE, FRAME_TYPE_CLI,
};

static struct {
    char msg[MAX_LEN];
    unsigned char u3_rxbuf[MAX_LEN];
    int size, type;
} g = { {0}, {0}, 0, FRAME_TYPE_NONE};

void USART_Poll(void)
{
    switch(g.type) {
        case FRAME_TYPE_NONE:
            return;
        case FRAME_TYPE_CLI:
            CLI_Parse(g.msg, g.size);
            break;
        default:
            break;
    }
    g.type = FRAME_TYPE_NONE;
}

void USART_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    USART_InitTypeDef uis;
    NVIC_InitTypeDef nis;
    DMA_InitTypeDef dis;

    DMA_Channel_TypeDef* DMA_Channelx = NULL;
    DMA_Channelx = DMA1_Channel5;

    USART_StructInit(&uis);

    nis.NVIC_IRQChannel = USART1_IRQn;
    dis.DMA_PeripheralBaseAddr = (unsigned long)(&USART1->DR);
    dis.DMA_MemoryBaseAddr = (unsigned long)(g.u3_rxbuf);

    nis.NVIC_IRQChannelPreemptionPriority = 0;
    nis.NVIC_IRQChannelSubPriority = 1;
    nis.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nis);

    DMA_DeInit(DMA_Channelx);
    dis.DMA_DIR = DMA_DIR_PeripheralSRC;    // 单向，外设源
    dis.DMA_BufferSize = MAX_LEN;
    dis.DMA_PeripheralInc = DMA_PeripheralInc_Disable;    // 禁止外设递增
    dis.DMA_MemoryInc = DMA_MemoryInc_Enable;    // 允许内存递增
    dis.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    dis.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    dis.DMA_Mode = DMA_Mode_Circular;
    dis.DMA_Priority = DMA_Priority_VeryHigh;
    dis.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA_Channelx, &dis);
    DMA_Cmd(DMA_Channelx, ENABLE);

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
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
    USART_Cmd(USART1, ENABLE);
//    while(1) { USART_WriteByte(USART1, 'K'); }
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
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
    USART_SendData(USARTx, byte);
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
}

void USART_RX_IDLE_IRQHandler(USART_TypeDef* USARTx)
{
    DMA_Channel_TypeDef* DMA_Channelx = NULL;
    unsigned char* usartx_rx_buf;
    int size;
    if(0) {
    }
    else if(USARTx == USART1) {
        DMA_Channelx = DMA1_Channel5;
        usartx_rx_buf = g.u3_rxbuf;
    }

    DMA_Cmd(DMA_Channelx, DISABLE);    // 关闭 DMA, 防止处理期间有数据
    size = USARTx->SR;
    size = USARTx->DR;
    size = MAX_LEN - DMA_GetCurrDataCounter(DMA_Channelx);
    DMA_SetCurrDataCounter(DMA_Channelx, MAX_LEN);

    if(g.type == FRAME_TYPE_NONE) {
        memcpy((char*)g.msg, usartx_rx_buf, MAX_LEN);
        g.size = size;

        if(USARTx == USART_USB) {
            if(0) {
            }
            else if(usartx_rx_buf[0] == '?') {
                g.type = FRAME_TYPE_CLI;
            }
            else if(usartx_rx_buf[0] == '#' && usartx_rx_buf[1] == '#') {
                g.type = FRAME_TYPE_CLI;
            }
        }
    }
    bzero((char*)usartx_rx_buf, sizeof(g.u3_rxbuf));
    DMA_Cmd(DMA_Channelx, ENABLE);    // 关闭 DMA, 防止处理期间有数据
}
