#include "usart_f3.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#include "misc.h"
#include "cli.h"

#define MAX_LEN 128
#define CLI_BINARY_HEADER 0x96

//volatile unsigned char usart1_rx_buf[MAX_LEN];
//volatile unsigned char usart2_rx_buf[MAX_LEN];
static unsigned char USART1_rx_buf[MAX_LEN];

enum {
    FRAME_TYPE_NONE, FRAME_TYPE_CLI, FRAME_TYPE_CLI_BIN
};

static struct {
    unsigned char msg[MAX_LEN];
    int size;
    unsigned char type;
} g;

static void USART_WriteData_s(USART_TypeDef* USARTx, const void* data, int num);

void USART_ReConfig(USART_TypeDef* USARTx, unsigned long baudrate)
{
    USART_InitTypeDef uis;
//    USART_DeInit(USARTx);
    USART_Cmd(USARTx, DISABLE);
    USART_StructInit(&uis);
    uis.USART_WordLength = USART_WordLength_8b;
    uis.USART_StopBits = USART_StopBits_1;
    uis.USART_Parity = USART_Parity_No;
    uis.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    uis.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    uis.USART_BaudRate = baudrate;
    USART_Init(USARTx, &uis);
    if(USARTx == USART1)
        USART_SWAPPinCmd(USARTx, ENABLE);
    USART_Cmd(USARTx, ENABLE);
}

void USART_Config(void)
{
    NVIC_InitTypeDef nis;
    DMA_InitTypeDef dis;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    nis.NVIC_IRQChannel = USART1_IRQn;
    nis.NVIC_IRQChannelPreemptionPriority = 1;
    nis.NVIC_IRQChannelSubPriority = 1;
    nis.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nis);

    DMA_DeInit(DMA1_Channel5);
    dis.DMA_PeripheralBaseAddr = (unsigned long)(&USART1->RDR);
    dis.DMA_MemoryBaseAddr = (unsigned long)USART1_rx_buf;
    dis.DMA_DIR = DMA_DIR_PeripheralSRC;    // 单向，外设源
    dis.DMA_BufferSize = MAX_LEN;
    dis.DMA_PeripheralInc = DMA_PeripheralInc_Disable;    // 禁止外设递增
    dis.DMA_MemoryInc = DMA_MemoryInc_Enable;    // 允许内存递增
    dis.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    dis.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    dis.DMA_Mode = DMA_Mode_Normal;
    dis.DMA_Priority = DMA_Priority_VeryHigh;
    dis.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel5, &dis);
    DMA_Cmd(DMA1_Channel5, ENABLE);

    USART_ReConfig(USART1, 500000UL);

    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
    USART_ITConfig(USART1, USART_IT_ORE, ENABLE);
    USART_ITConfig(USART1, USART_IT_FE, ENABLE);
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
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

void USART_Poll(void)
{
    switch(g.type) {
        case FRAME_TYPE_NONE:
            return;
        case FRAME_TYPE_CLI:
            CLI_Parse(g.msg, g.size, CLI_TYPE_ASCII);
            break;
        case FRAME_TYPE_CLI_BIN:
            CLI_Parse(g.msg, g.size, CLI_TYPE_BINARY);
            break;
        default:
            break;
    }
    g.type = FRAME_TYPE_NONE;
}

void USART_RX_IDLE_IRQHandler(USART_TypeDef* USARTx)
{
    DMA_Channel_TypeDef* DMA_Channelx = NULL;
    unsigned char* usartx_rx_buf;
    int size = size;
    if(USARTx == USART1) {
        usartx_rx_buf = USART1_rx_buf;
        DMA_Channelx = DMA1_Channel5;
    }

    DMA_Cmd(DMA_Channelx, DISABLE);    // 关闭 DMA, 防止处理期间有数据
    size = USARTx->ISR;
    size = USARTx->RDR;
    size = MAX_LEN - DMA_GetCurrDataCounter(DMA_Channelx);
    DMA_SetCurrDataCounter(DMA_Channelx, MAX_LEN);

    if(g.type == FRAME_TYPE_NONE) {
        memcpy(g.msg, usartx_rx_buf, size);
        g.size = size;

        if(0) {
        }
        else if(g.msg[0] == CLI_BINARY_HEADER) {
            g.type = FRAME_TYPE_CLI_BIN;
        }
        else if(memcmp(g.msg, "##", 2) == 0 || g.msg[0] == '?') {
            g.type = FRAME_TYPE_CLI;
        }
    }

    DMA_Cmd(DMA_Channelx, ENABLE);    // 关闭 DMA, 防止处理期间有数据
}
