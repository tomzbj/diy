/// @file usart.c
/// @brief 串口通讯程序.
/// @author Zhang Hao, stavrosatic@gmail.com
/// @version R0.1
/// @date 2017-10-13
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
/*
 #include <stdlib.h>
 */
#include "usart.h"
#include "misc.h"
#include "stm32f0xx.h"

#define MAX_LEN 256
unsigned char usart1_rx_buf[MAX_LEN];
unsigned char* g_usartx_rx_buf;

enum {
    FRAME_TYPE_NONE, FRAME_TYPE_CLI, FRAME_TYPE_MODBUS_ASCII,
    FRAME_TYPE_MODBUS_RTU, FRAME_TYPE_FILEOPS
};

struct {
    char msg[256];
    USART_TypeDef* USARTx;
    int size;
    unsigned char type;
} g_frame;

/*volatile bool flag_cli_to_parse = false;
 volatile bool flag_modbus_ascii_to_parse = false;
 volatile bool flag_modbus_rtu_to_parse = false;
 */

/// @brief 向指定USART端口输出的printf.
/// @param USARTn 指定的USART端口.
/// @param fmt 输出格式.
/// @param ... 要输出的数据.
/// @retval None
void uprintf(USART_TypeDef* USARTn, const char* __restrict fmt, ...)
{
    char buf[256];
    va_list arp;
    va_start(arp, fmt);
    vsprintf(buf, fmt, arp);
    USART_WriteData(USARTn, (unsigned char*)buf, strlen(buf));
    va_end(arp);
}

/// @brief 配置USART参数. 
/// @param None
/// @retval None
void USART_Config(void)
{
    // PA2 as USART2_TX, PA3 as USART2_RX, PA9 as USART1_TX, PA10 as USART1_RX
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

    // nvic init
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;   //USARTn_IRQChannel;
    NVIC_Init(&NVIC_InitStructure);

    // DMA init
    DMA_DeInit(DMA1_Channel3);  // DMA1_Channel3用于USART1_RX 
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART1->RDR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)usart1_rx_buf;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  // 单向，外设源
    DMA_InitStructure.DMA_BufferSize = MAX_LEN;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;   // 禁止外设递增
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

    // 默认: 波特率9600, 字宽8位, 停止位1, 极性no, 模式tx/rx, 流控none
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
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);  // 采用 DMA 方式接收  
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
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
            ;
        USART_SendData(USARTx, *((u8 *)data));
        data++;
    }
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
//    if(g_frame.type == FRAME_TYPE_CLI)
//        CliParse(g_frame.USARTx, g_frame.msg, g_frame.size);
    /*    else if(g_frame.type == FRAME_TYPE_MODBUS_ASCII)
     ModbusAsciiParse(g_frame.USARTx, g_frame.msg, g_frame.size);
     else if(g_frame.type == FRAME_TYPE_MODBUS_RTU)
     ModbusRtuParse(g_frame.USARTx, g_frame.msg, g_frame.size);
     else if(g_frame.type == FRAME_TYPE_FILEOPS)
     FileopsParse(g_frame.USARTx, g_frame.msg, g_frame.size);
     */
    g_frame.type = FRAME_TYPE_NONE;
}

/// @brief 向USART写一个字节数据. 
/// @param USARTx 要写数据的USART端口.
/// @param byte 要写的字节.
/// @retval None
void USART_WriteByte(USART_TypeDef* USARTx, u8 byte)
{
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
        ;
    USART_SendData(USARTx, byte);
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)
        ;
}

/*
 /// @brief 读取USART数据帧.
 /// @param None
 /// @retval None
 USART_Frame_t* USART_GetFrame(void)
 {
 return (USART_Frame_t*)&g_frame;
 }
 */

/// @brief USART空闲中断处理程序, 用于接收数据. 
/// @param USARTx 要处理的USART端口.
/// @retval None
void USART_RX_IDLE_IRQHandler(USART_TypeDef* USARTx)
{
    typeof(DMA1_Channel5) dma_channel = NULL;
    unsigned char size = size;
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

    if(g_frame.type == FRAME_TYPE_NONE) {   // 判断上一帧是否已处理完, 未处理完则直接忽略
        memcpy((char*)g_frame.msg, usartx_rx_buf, MAX_LEN);
        g_frame.size = size;
        g_frame.USARTx = USARTx;

        /*        if(usartx_rx_buf[0] == ':' &&
         ModbusAsciiCheckIntegrity(usartx_rx_buf, size) == MODBUS_CHECK_OK) {
         g_frame.type = FRAME_TYPE_MODBUS_ASCII;
         }
         else if(usartx_rx_buf[0] == '?' || usartx_rx_buf[0] == '#') {
         g_frame.type = FRAME_TYPE_CLI;
         }
         else if(ModbusRtuCheckIntegrity(usartx_rx_buf, size) == MODBUS_CHECK_OK) {
         g_frame.type = FRAME_TYPE_MODBUS_RTU;
         }
         else {
         g_frame.type = FRAME_TYPE_FILEOPS;
         }
         */
    }
    bzero((char*)usartx_rx_buf, sizeof(usart1_rx_buf));
    DMA_Cmd(dma_channel, ENABLE);
}
