#include "platform.h"

#define MAX_LEN 160

unsigned char usart1_rx_buf[MAX_LEN];
unsigned char* g_usartx_rx_buf;

enum {
    FRAME_TYPE_NONE, FRAME_TYPE_CLI, FRAME_TYPE_MODBUS, FRAME_TYPE_ZMB_RTU,
    FRAME_TYPE_FILEOPS, FRAME_TYPE_FLASHER, FRAME_TYPE_IAP
};

static struct {
    unsigned char msg[MAX_LEN];
    USART_TypeDef* USARTx;
    int size;
    unsigned char type;
} g;

void uputc(char c)
{
    ( {  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);});
    USART_SendData(USART1, c);
    ( {  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);});
}

void USART_Config(void)
{
    NVIC_InitTypeDef nis;
    DMA_InitTypeDef dis;
    USART_InitTypeDef uis;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    // nvic init
    nis.NVIC_IRQChannelPriority = 0;
    nis.NVIC_IRQChannelCmd = ENABLE;
    nis.NVIC_IRQChannel = USART1_IRQn;   //USARTn_IRQChannel;
    NVIC_Init(&nis);

    // DMA init
    DMA_DeInit(DMA1_Channel3);  // DMA1_Channel3用于USART1_RX
    dis.DMA_PeripheralBaseAddr = (unsigned long)(&USART1->RDR);
    dis.DMA_MemoryBaseAddr = (unsigned long)usart1_rx_buf;
    dis.DMA_DIR = DMA_DIR_PeripheralSRC;  // 单向，外设源
    dis.DMA_BufferSize = MAX_LEN;
    dis.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  // 禁止外设递增
    dis.DMA_MemoryInc = DMA_MemoryInc_Enable; // 允许内存递增
    dis.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    dis.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    dis.DMA_Mode = DMA_Mode_Normal;
    dis.DMA_Priority = DMA_Priority_VeryHigh;
    dis.DMA_M2M = DMA_M2M_Disable;
    dis.DMA_PeripheralBaseAddr = (unsigned long)(&USART1->RDR);
    dis.DMA_MemoryBaseAddr = (unsigned long)usart1_rx_buf;
    DMA_Init(DMA1_Channel3, &dis);
    DMA_Cmd(DMA1_Channel3, ENABLE);

    // usart init
    USART_DeInit(USART1);
    USART_StructInit(&uis);
    uis.USART_WordLength = USART_WordLength_8b;
    uis.USART_StopBits = USART_StopBits_1;
    uis.USART_Parity = USART_Parity_No;
    uis.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    uis.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    uis.USART_BaudRate = 500000UL;
//    USART_SWAPPinCmd(USART1, DISABLE);
    USART_Init(USART1, &uis);
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
    USART_ITConfig(USART1, USART_IT_ORE, ENABLE);
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);  // 采用 DMA 方式接收
    USART_Cmd(USART1, ENABLE);
//    xdev_out(uputc);
}

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

void USART_RX_IDLE_IRQHandler(USART_TypeDef* USARTx)
{
    typeof(DMA1_Channel6) dma_channel = NULL;
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
        memcpy((char*)g.msg, usartx_rx_buf, MAX_LEN);
        g.size = size;
        g.USARTx = USARTx;
        if(0) {
        }
        else if(g.msg[0] == '?' || g.msg[0] == '#') {
            g.type = FRAME_TYPE_CLI;
        }
    }
    bzero((char*)usartx_rx_buf, sizeof(usart1_rx_buf));
    DMA_Cmd(dma_channel, ENABLE);
}
