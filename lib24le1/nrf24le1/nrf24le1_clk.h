#ifndef _NRF24LE1_CLK_H
#define _NRF24LE1_CLK_H

typedef enum {
    CLK_StartXOSC16_ROSC16 = 0,
    CLK_StartROSC16 = 1,
    CLK_StartXOSC16 = 2
} CLK_StartOSC16TypeDef;

typedef enum {
    CLK_Sysclk_16M = 0,
    CLK_Sysclk_8M = 1,
    CLK_Sysclk_4M = 2,
    CLK_Sysclk_2M = 3,
    CLK_Sysclk_1M = 4,
    CLK_Sysclk_500K = 5,
    CLK_Sysclk_250K = 6,
    CLK_Sysclk_125K = 7
} CLK_SysclkTypeDef;

typedef struct {
    unsigned char CLK_Sysclk:3;
    unsigned char CLK_WakeupIRQ_From_XOSC16_Active:1;
    unsigned char CLK_StartOSC16:2;
    unsigned char CLK_Bypass_Oscillators:1;
    unsigned char CLK_Keep_XOSC16_On_In_Register_Retention_Mode:1;
} CLK_InitTypeDef;

void CLK_Init(CLK_InitTypeDef* CLK_InitStructure);
unsigned long CLK_GetSystemClock(void);

#endif
