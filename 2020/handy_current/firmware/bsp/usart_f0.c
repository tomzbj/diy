#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "usart_f0.h"
#include "misc.h"
#include "stm32f0xx.h"
#include "cli.h"

#define MAX_LEN 1152

unsigned char usart1_rx_buf[MAX_LEN];
unsigned char* g_usartx_rx_buf;

enum {
    FRAME_TYPE_NONE, FRAME_TYPE_CLI
};

static struct {
    char msg[MAX_LEN];
    USART_TypeDef* USARTx;
    int size;
    unsigned char type;
} g;

void uprintf(USART_TypeDef* USARTn, const char* __restrict fmt, ...)
{
    char buf[256];
    va_list arp;
    va_start(arp, fmt);
    vsprintf(buf, fmt, arp);
    USART_WriteData(USARTn, (unsigned char*)buf, strlen(buf));
    va_end(arp);
}

void USART_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    // nvic init
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;   //USARTn_IRQChannel;
    NVIC_Init(&NVIC_InitStructure);

    // DMA init
    DMA_DeInit(DMA1_Channel3);  // DMA1_Channel3用于USART1_RX
    DMA_InitStructure.DMA_PeripheralBaseAddr = (unsigned long)(&USART1->RDR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (unsigned long)usart1_rx_buf;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  // 单向，外设源
    DMA_InitStructure.DMA_BufferSize = MAX_LEN;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  // 禁止外设递增
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; // 允许内存递增  
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel3, &DMA_InitStructure);
    DMA_Cmd(DMA1_Channel3, ENABLE);

    // usart init
    USART_DeInit(USART1);
    USART_StructInit(&USART_InitStructure);
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl =
            USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_BaudRate = 500000UL;
    USART_Init(USART1, &USART_InitStructure);
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
    USART_ITConfig(USART1, USART_IT_ORE, ENABLE);
    USART_ITConfig(USART1, USART_IT_FE, ENABLE);
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);  // 采用 DMA 方式接收
    USART_SWAPPinCmd(USART1, ENABLE);
    USART_Cmd(USART1, ENABLE);
}

/// @brief 向USART写数据.
/// @param USARTx 要写数据的USART端口.
/// @param data 要写入的数据缓冲区指针.
/// @param num 要写入的字节数.
/// @retval None
void USART_WriteData(USART_TypeDef* USARTx, const void* data, int num)
{
    while(num--) {
        ( {  while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);});
        USART_SendData(USARTx, *((unsigned char *)data));
        data++;
    }
    ( {  while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);});
}

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

/// @brief 向USART写一个字节数据.
/// @param USARTx 要写数据的USART端口.
/// @param byte 要写的字节.
/// @retval None
void USART_WriteByte(USART_TypeDef* USARTx, unsigned char byte)
{
    ( {  while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);});
    USART_SendData(USARTx, byte);
    ( {  while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);});
}

/// @brief USART空闲中断处理程序, 用于接收数据. 
/// @param USARTx 要处理的USART端口.
/// @retval None
void USART_RX_IDLE_IRQHandler(USART_TypeDef* USARTx)
{
    typeof(DMA1_Channel3) dma_channel = NULL;
    unsigned short size = size;
    const char* usartx_rx_buf = NULL;
    if(USARTx == USART1) {
        dma_channel = DMA1_Channel3;
        usartx_rx_buf = (const char*)usart1_rx_buf;
    }
    DMA_Cmd(dma_channel, DISABLE);
    size = USARTx->ISR;
    size = USARTx->RDR;
    size = MAX_LEN - DMA_GetCurrDataCounter(dma_channel);
    DMA_SetCurrDataCounter(dma_channel, MAX_LEN);

    if(g.type == FRAME_TYPE_NONE) {   // 判断上一帧是否已处理完, 未处理完则直接忽略
//        _dbg();
        memcpy((char*)g.msg, usartx_rx_buf, MAX_LEN);
        g.size = size;
        g.USARTx = USARTx;
        if(memcmp(g.msg, "##", 2) == 0 || g.msg[0] == '?')
            g.type = FRAME_TYPE_CLI;
    }
    bzero((char*)usartx_rx_buf, sizeof(usart1_rx_buf));
    DMA_Cmd(dma_channel, ENABLE);
}
