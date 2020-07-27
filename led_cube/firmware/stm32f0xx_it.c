#include <stdio.h>
#include "platform.h"
#include "ztask.h"
//#include "usbd_hid_core.h"

/*
 typedef enum {
 JOY_NONE = 0,
 JOY_SEL = 1,
 JOY_DOWN = 2,
 JOY_LEFT = 3,
 JOY_RIGHT = 4,
 JOY_UP = 5,
 JOY_LEFTKEY = 6,
 JOY_RIGHTKEY = 7
 } JOYState_TypeDef;
 */

static uint8_t* USBD_HID_GetPos(void);

void SysTick_Handler(void)
{
    _delay_nms_mm();            // executes every 1ms
    zt_tick();
//    uint8_t* buf;
//    buf = USBD_HID_GetPos();
//    if((buf[0] != 0) || (buf[1] != 0) || (buf[2] != 0)) {
//    USBD_HID_SendReport(&USB_Device_dev, buf, 4);
//    }
}

void USART3_4_IRQHandler(void)
{
    if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET) {
        USART_RX_IDLE_IRQHandler(USART3);
        USART_ClearITPendingBit(USART3, USART_IT_IDLE);
    }
    if(USART_GetITStatus(USART3, USART_IT_ORE) != RESET) {
        USART_ClearITPendingBit(USART3, USART_IT_ORE);
    }
    if(USART_GetITStatus(USART3, USART_IT_FE) != RESET) {
        USART_ClearITPendingBit(USART3, USART_IT_FE);
    }
}

//#define CURSOR_STEP     5
//__IO uint32_t remote_wakeup = 0;

void USB_IRQHandler(void)
{
    USB_Istr();
}

void EXTI4_15_IRQHandler(void)
{
}

/*
 uint8_t JoyState(void)
 {
 //    if(STM_EVAL_PBGetState(Button_RIGHT)) {
 if((GPIOA->IDR & GPIO_Pin_3) == 0)
 return JOY_RIGHT;
 if((GPIOA->IDR & GPIO_Pin_0) == 0)
 return JOY_LEFT;
 if((GPIOA->IDR & GPIO_Pin_2) == 0)
 return JOY_UP;
 if((GPIOA->IDR & GPIO_Pin_1) == 0)
 return JOY_DOWN;
 if((GPIOA->IDR & GPIO_Pin_4) == 0)
 return JOY_LEFTKEY;
 if((GPIOA->IDR & GPIO_Pin_6) == 0)
 return JOY_RIGHTKEY;
 else
 return 0;
 }

 static uint8_t* USBD_HID_GetPos(void)
 {
 int8_t x = 0, y = 0;
 unsigned char key = 0;
 static uint8_t HID_Buffer[4];
 switch(JoyState()) {
 case JOY_LEFT:
 x -= CURSOR_STEP;
 break;
 case JOY_RIGHT:
 x += CURSOR_STEP;
 break;
 case JOY_UP:
 y -= CURSOR_STEP;
 break;
 case JOY_DOWN:
 y += CURSOR_STEP;
 break;
 default:
 break;
 }
 if((GPIOA->IDR & GPIO_Pin_4) == 0)
 key |= 0x1;
 if((GPIOA->IDR & GPIO_Pin_6) == 0)
 key |= 0x2;
 HID_Buffer[0] = key;
 HID_Buffer[1] = x;
 HID_Buffer[2] = y;
 HID_Buffer[3] = 0;

 return HID_Buffer;
 }
 */

void PPP_IRQHandler(void) { while(1); }
void NMI_Handler(void) { while(1); }
void HardFault_Handler(void) { while(1); }
void SVC_Handler(void) { while(1); }
void PendSV_Handler(void) { while(1); }
