#include "stm32f10x.h"
#include "hw_config.h"
#include "usb_lib.h"

#include "stm32f10x.h"
#include "stm32f10x_flash.h"
#include "misc.h"

#define JOY_UP       GPIO_Pin_2 // PE3 
#define JOY_DOWN     GPIO_Pin_4 // PE2 
#define JOY_LEFT     GPIO_Pin_5 // PE5 
#define JOY_RIGHT    GPIO_Pin_3 // PE4 
#define JOY_LEFT_BUTTON    GPIO_Pin_13  // PC.13 
#define JOY_RIGHT_BUTTON    GPIO_Pin_12 // PC.12 

ErrorStatus HSEStartUpStatus;

void Set_System(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | // Enable GPIOD and GPIOE clock 
        RCC_APB2Periph_GPIOE, ENABLE);
    RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5); // Select USBCLK source  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE); // Enable USB clock 

    // PA8 used as USB pull-up
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void Enter_LowPowerMode(void)
{
}

void Leave_LowPowerMode(void)
{
}

void USB_Cable_Config(FunctionalState NewState)
{
    if(NewState != DISABLE) {
        GPIO_ResetBits(GPIOA, GPIO_Pin_8);
    }
    else {
        GPIO_SetBits(GPIOA, GPIO_Pin_8);
    }
}

unsigned char JoyState(void)
{
    return 0;
}

void Joystick_Send(unsigned char Keys)
{
}

