#include "misc.h"
#include "stm32f10x.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include "spiflash.h"

#include "usb_init.h"
#include "usb_pwr.h"
#include "vs1003.h"
#include "hw_config.h"
#include "ff.h"
#include "playback.h"
#include "pwr_f1.h"

void Init(void)
{
    Set_System();
    SysTick_Config(72000UL);

    typeof(RCC_APB2Periph_GPIOA) per1, per2, perh;

    per1 = RCC_APB1Periph_SPI2;
    per2 = RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC
            | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1 | RCC_APB2Periph_SPI1;
    perh = RCC_AHBPeriph_DMA1;
    RCC_APB1PeriphClockCmd(per1, ENABLE);
    RCC_APB2PeriphClockCmd(per2, ENABLE);
    RCC_AHBPeriphClockCmd(perh, ENABLE);
    USB_Init();
    USART_Config();
    SPIFLASH_Config();
    VS1003_Config();

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);
    EXTI_StructInit(&EXTI_InitStructure);
    EXTI_ClearITPendingBit(EXTI_Line2);
    EXTI_InitStructure.EXTI_Line = EXTI_Line2;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // nvic init
    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    extern FATFS g_fs;
    FRESULT res = res; /* 文件操作结果 */
    res = f_mount(&g_fs, "0:", 1);
}

int main(void)
{
    Init();
    while(1) {
        static int count = 0;
        if(USART_FrameToParse()) {
            USART_ParseFrame();
        }
        extern volatile unsigned char bDeviceState;
//        if(PLAYBACK_GetStatus() == false && bDeviceState == UNCONNECTED) {
        if(PLAYBACK_GetStatus() == false) {
            count++;
            if(count > 1000000UL) {
                PWR_Sleep();
                count = 0;
            }
        }
    }
}
