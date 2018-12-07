#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#include "misc.h"
#include "usart.h"
#include "stm32f10x.h"
#include "cmd.h"

#define MAX_LEN 256

typedef enum {
    FRAME_TYPE_NONE, FRAME_TYPE_CMD,
} frame_type_t;

//USART_Mode_t g_mode = USART_MODE_NORMAL;

//// @brief USART数据帧
struct {
    unsigned char msg[MAX_LEN];
    USART_TypeDef* USARTx;
    int size;
    frame_type_t type;
} g_frame;

unsigned char usart1_rx_buf[MAX_LEN];
unsigned char usart2_rx_buf[2];
unsigned char usart3_rx_buf[2];
unsigned char* usartx_rx_buf;

void USART_WriteData_s(USART_TypeDef* USARTx, const void* data, int num);

void uprintf(USART_TypeDef* USARTn, const char* __restrict fmt, ...)
{
    char buf[256];
    va_list arp;
    va_start(arp, fmt);
    vsprintf(buf, fmt, arp);
    USART_WriteData_s(USARTn, (unsigned char*) buf, strlen(buf));
    va_end(arp);
}

void USARTx_Config(USART_TypeDef* USARTx)
{
    unsigned short txpin = 0, rxpin = 0;
    GPIO_TypeDef* GPIOx = NULL;
    DMA_Channel_TypeDef* DMA_Channelx = NULL;

    GPIO_StructInit(&GPIO_InitStructure);
    USART_StructInit(&USART_InitStructure);

    switch((unsigned long) USARTx) {
        case (unsigned long) USART2:
            txpin = GPIO_Pin_2;
            rxpin = GPIO_Pin_3;
            GPIOx = GPIOA;
            NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
            DMA_Channelx = DMA1_Channel6;
            DMA_InitStructure.DMA_PeripheralBaseAddr =
                (unsigned long) (&USART2->DR);
            DMA_InitStructure.DMA_MemoryBaseAddr =
                (unsigned long) usart2_rx_buf;
            USART_InitStructure.USART_BaudRate = 115200UL; //9600UL;
            break;
        case (unsigned long) USART3:
            txpin = GPIO_Pin_10;
            rxpin = GPIO_Pin_11;
            GPIOx = GPIOB;
            NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
            DMA_Channelx = DMA1_Channel3;
            DMA_InitStructure.DMA_PeripheralBaseAddr =
                (unsigned long) (&USART3->DR);
            DMA_InitStructure.DMA_MemoryBaseAddr =
                (unsigned long) usart3_rx_buf;
            USART_InitStructure.USART_BaudRate = 500000UL;
            break;
        case (unsigned long) USART1:
            txpin = GPIO_Pin_9;
            rxpin = GPIO_Pin_10;
            GPIOx = GPIOA;
            NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
            DMA_Channelx = DMA1_Channel5;
            DMA_InitStructure.DMA_PeripheralBaseAddr =
                (unsigned long) (&USART1->DR);
            DMA_InitStructure.DMA_MemoryBaseAddr =
                (unsigned long) usart1_rx_buf;
            USART_InitStructure.USART_BaudRate = 500000UL;
            break;
    }
    GPIO_InitStructure.GPIO_Pin = txpin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOx, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = rxpin;
    GPIO_Init(GPIOx, &GPIO_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    DMA_DeInit(DMA_Channelx);
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  // 单向，外设源
    DMA_InitStructure.DMA_BufferSize = MAX_LEN;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  // 禁止外设递增
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; // 允许内存递增  
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA_Channelx, &DMA_InitStructure);
    DMA_Cmd(DMA_Channelx, ENABLE);

    USART_DeInit(USARTx);
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl =
        USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USARTx, &USART_InitStructure);

    USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);
    USART_DMACmd(USARTx, USART_DMAReq_Rx, ENABLE);
    USART_Cmd(USARTx, ENABLE);
}

void USART_Config(void)
{
    USARTx_Config(USART1);
//    USARTx_Config(USART2);
//    USARTx_Config(USART3);
}

void USART_WriteData_s(USART_TypeDef* USARTx, const void* data, int num)
{
    while(num--) {
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
            ;
        USART_SendData(USARTx, *((unsigned char*) data));
        data++;
    }
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)
        ;
}

void USART_WriteByte(USART_TypeDef* USARTx, unsigned char byte)
{
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
        ;
    USART_SendData(USARTx, byte);
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)
        ;
}

bool USART_FrameToParse(void)
{
    if(g_frame.type != FRAME_TYPE_NONE)
        return true;
    else
        return false;
}

void USART_ParseFrame(void)
{
    if(g_frame.type == FRAME_TYPE_CMD) {
        FSCMD_Parse((char*) g_frame.msg);
    }
    g_frame.type = FRAME_TYPE_NONE;
}

void USART_RX_IDLE_IRQHandler(USART_TypeDef* USARTx)
{
    DMA_Channel_TypeDef* DMA_Channelx = NULL;
    unsigned char size = size;
    if(USARTx == USART1) {
        usartx_rx_buf = usart1_rx_buf;
        DMA_Channelx = DMA1_Channel5;
    }
    else if(USARTx == USART2) {
        usartx_rx_buf = usart2_rx_buf;
        DMA_Channelx = DMA1_Channel6;
    }
    else if(USARTx == USART3) {
        usartx_rx_buf = usart3_rx_buf;
        DMA_Channelx = DMA1_Channel3;
    }

    DMA_Cmd(DMA_Channelx, DISABLE);    // 关闭 DMA, 防止处理期间有数据
    size = USARTx->SR;
    size = USARTx->DR;
    size = MAX_LEN - DMA_GetCurrDataCounter(DMA_Channelx);
    DMA_SetCurrDataCounter(DMA_Channelx, MAX_LEN);

    if(g_frame.type == FRAME_TYPE_NONE) {
        memcpy((char*) g_frame.msg, usartx_rx_buf, MAX_LEN);
        g_frame.size = size;
        g_frame.USARTx = USARTx;
        if(USARTx == USART1) {
            g_frame.type = FRAME_TYPE_CMD;
        }
    }
    bzero(usart1_rx_buf, sizeof(usart1_rx_buf));
    DMA_Cmd(DMA_Channelx, ENABLE);    // 关闭 DMA, 防止处理期间有数据
}
