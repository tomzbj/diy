#include "usb_bsp.h"

void USB_BSP_Init(USB_CORE_HANDLE *pdev)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
    RCC_USBCLKConfig(RCC_USBCLK_PLLCLK); 
}

void USB_BSP_EnableInterrupt(USB_CORE_HANDLE *pdev)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USB_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
