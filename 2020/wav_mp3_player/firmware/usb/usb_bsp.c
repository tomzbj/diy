#include "usb_bsp.h"

void USB_OTG_BSP_Init(USB_OTG_CORE_HANDLE * pdev)
{
    GPIO_InitTypeDef gis;
//    #ifdef USE_USB_OTG_FS
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    gis.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12;
    gis.GPIO_Speed = GPIO_Speed_100MHz;
    gis.GPIO_Mode = GPIO_Mode_AF;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &gis);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_OTG1_FS);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_OTG1_FS);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_OTG1_FS);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS, ENABLE);
}
void USB_OTG_BSP_EnableInterrupt(USB_OTG_CORE_HANDLE * pdev)
{
    NVIC_InitTypeDef nvis;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
#ifdef USE_USB_OTG_HS
    nvis.NVIC_IRQChannel = OTG_HS_IRQn;
#else
    nvis.NVIC_IRQChannel = OTG_FS_IRQn;
#endif
    nvis.NVIC_IRQChannelPreemptionPriority = 1;
    nvis.NVIC_IRQChannelSubPriority = 3;
    nvis.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvis);
#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    nvis.NVIC_IRQChannel = OTG_HS_EP1_OUT_IRQn;
    nvis.NVIC_IRQChannelPreemptionPriority = 1;
    nvis.NVIC_IRQChannelSubPriority = 2;
    nvis.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvis);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    nvis.NVIC_IRQChannel = OTG_HS_EP1_IN_IRQn;
    nvis.NVIC_IRQChannelPreemptionPriority = 1;
    nvis.NVIC_IRQChannelSubPriority = 1;
    nvis.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvis);
#endif
}
#if defined (USE_STM322xG_EVAL)
#define count_us   40
#elif defined(USE_STM324xG_EVAL) || defined(USE_STM324x9I_EVAL)
#define count_us   55
#else                           
#define count_us   12
#endif
void USB_OTG_BSP_uDelay(const uint32_t usec)
{
    uint32_t count = count_us * usec;
    do
    {
        if (--count == 0)
        {
            return;
        }
    }
    while (1);
}
void USB_OTG_BSP_mDelay(const uint32_t msec)
{
    USB_OTG_BSP_uDelay(msec * 1000);
}

