#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include "misc.h"
#include "usart_f1.h"
#include "stm32f10x.h"

#define MAX_LEN 256

unsigned char usart1_rx_buf[MAX_LEN];
//unsigned char usart2_rx_buf[MAX_LEN];
//unsigned char usart3_rx_buf[MAX_LEN];
unsigned char* usartx_rx_buf;

bool flag_updated = false;

typedef enum {
    FRAME_TYPE_NONE, FRAME_TYPE_BATT
} frame_type_t;

USART_Mode_t g_mode = USART_MODE_NORMAL;

//// @brief USART数据帧
volatile struct {
    char msg[MAX_LEN];
    USART_TypeDef* USARTx;
    int size;
    frame_type_t type;
} g_frame;

static void USART_WriteData_s(USART_TypeDef* USARTx, const void* data, int num);

void BATT_Parse(const char* msg, int size);

void USART_Poll(void)
{
    switch(g_frame.type) {
        case FRAME_TYPE_NONE:
            return;
        case FRAME_TYPE_BATT:
            BATT_Parse((const char*)g_frame.msg, g_frame.size);
            break;
    }
    g_frame.type = FRAME_TYPE_NONE;
}

void uprintf(USART_TypeDef* USARTn, const char* __restrict fmt, ...)
{
    char buf[256];
    va_list arp;
    va_start(arp, fmt);
    vsprintf(buf, fmt, arp);
    USART_WriteData_s(USARTn, (unsigned char*)buf, strlen(buf));
    va_end(arp);
}

void USARTx_Config(USART_TypeDef* USARTx)
{
    unsigned short txpin = 0, rxpin = 0;
    GPIO_TypeDef* GPIOx = NULL;
    DMA_Channel_TypeDef* DMA_Channelx = NULL;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;

    GPIO_StructInit(&GPIO_InitStructure);
    USART_StructInit(&USART_InitStructure);

    switch((unsigned long)USARTx) {
        case (unsigned long)USART2:
            txpin = GPIO_Pin_2;
            rxpin = GPIO_Pin_3;
            GPIOx = GPIOA;
            NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
            DMA_Channelx = DMA1_Channel6;
            DMA_InitStructure.DMA_PeripheralBaseAddr =
                    (unsigned long)(&USART2->DR);
//            DMA_InitStructure.DMA_MemoryBaseAddr = (unsigned long)usart2_rx_buf;
            USART_InitStructure.USART_BaudRate = 500000UL;
            break;
        case (unsigned long)USART3:
            txpin = GPIO_Pin_10;
            rxpin = GPIO_Pin_11;
            GPIOx = GPIOB;
            NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
            DMA_Channelx = DMA1_Channel3;
            DMA_InitStructure.DMA_PeripheralBaseAddr =
                    (unsigned long)(&USART3->DR);
//            DMA_InitStructure.DMA_MemoryBaseAddr = (unsigned long)usart3_rx_buf;
            USART_InitStructure.USART_BaudRate = 9600UL;
            break;
        case (unsigned long)USART1:
            txpin = GPIO_Pin_6;
            rxpin = GPIO_Pin_7;
            GPIOx = GPIOB;
            GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
            NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
            DMA_Channelx = DMA1_Channel5;
            DMA_InitStructure.DMA_PeripheralBaseAddr =
                    (unsigned long)(&USART1->DR);
            DMA_InitStructure.DMA_MemoryBaseAddr = (unsigned long)usart1_rx_buf;
            USART_InitStructure.USART_BaudRate = 500000UL;
            break;
    }
    GPIO_InitStructure.GPIO_Pin = txpin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOx, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = rxpin;
    GPIO_Init(GPIOx, &GPIO_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    DMA_DeInit(DMA_Channelx);
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  // 单向，外设源
    DMA_InitStructure.DMA_BufferSize = MAX_LEN;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;   // 禁止外设递增
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
        ( {  while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);});
        USART_SendData(USARTx, *((unsigned char*)data));
        data++;
    }
    ( {  while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);});
}

void USART_WriteByte(USART_TypeDef* USARTx, unsigned char byte)
{
    ( {  while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);});
    USART_SendData(USARTx, byte);
    ( {  while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);});
}

void USART_RX_IDLE_IRQHandler(USART_TypeDef* USARTx)
{
    DMA_Channel_TypeDef* DMA_Channelx = NULL;
//    volatile unsigned char* usart_rx_buf = NULL;
    unsigned char size;
    if(USARTx == USART1) {
        DMA_Channelx = DMA1_Channel5;
        usartx_rx_buf = usart1_rx_buf;
    }
/*    else if(USARTx == USART2) {
        DMA_Channelx = DMA1_Channel6;
        usartx_rx_buf = usart2_rx_buf;
    }
    else if(USARTx == USART3) {
        DMA_Channelx = DMA1_Channel3;
        usartx_rx_buf = usart3_rx_buf;
    }*/

    DMA_Cmd(DMA_Channelx, DISABLE);    // 关闭 DMA, 防止处理期间有数据
    size = USARTx->SR;
    size = USARTx->DR;
    size = MAX_LEN - DMA_GetCurrDataCounter(DMA_Channelx);
    DMA_SetCurrDataCounter(DMA_Channelx, MAX_LEN);
    if(g_frame.type == FRAME_TYPE_NONE) {
        memcpy((char*)g_frame.msg, usartx_rx_buf, MAX_LEN);
        g_frame.size = size;
        g_frame.USARTx = USARTx;

        if(USARTx == USART1) {
            g_frame.type = FRAME_TYPE_BATT;
        }
        else if(USARTx == USART2) {
        }
        else if(USARTx == USART3) {
        }
    }
    bzero((char*)usartx_rx_buf, sizeof(usart1_rx_buf));
    DMA_Cmd(DMA_Channelx, ENABLE);    // 关闭 DMA, 防止处理期间有数据
}
