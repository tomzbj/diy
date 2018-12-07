#ifndef _NRF24LE1_GPIO_H
#define _NRF24LE1_GPIO_H

typedef enum {
    GPIO0, GPIO1, GPIO2, GPIO3
} GPIO_TypeDef;

typedef enum {
    GPIO_Dir_In, GPIO_Dir_Out
} GPIO_DirTypeDef;

typedef enum {
    GPIO_Mode_OutBuffer_Normal = 0x00, GPIO_Mode_OutBuffer_High = 0x60,
    GPIO_Mode_InBuffer_NoPull = 0x10, GPIO_Mode_InBuffer_PullDown = 0x30,
    GPIO_Mode_InBuffer_PullUp = 0x50, GPIO_Mode_InBuffer_Off = 0x70
} GPIO_ModeTypeDef;

typedef enum {
    GPIO_Pin_0 = 0x01, GPIO_Pin_1 = 0x02, GPIO_Pin_2 = 0x04, GPIO_Pin_3 = 0x08,
    GPIO_Pin_4 = 0x10, GPIO_Pin_5 = 0x20, GPIO_Pin_6 = 0x40, GPIO_Pin_7 = 0x80,
    GPIO_Pin_All = 0xff
} GPIO_PinTypeDef;

typedef struct {
    GPIO_PinTypeDef GPIO_Pin;
    GPIO_ModeTypeDef GPIO_Mode;
} GPIO_InitTypeDef;

void GPIO_Init(GPIO_TypeDef gpio, GPIO_InitTypeDef* GPIO_InitStructure);

#endif
