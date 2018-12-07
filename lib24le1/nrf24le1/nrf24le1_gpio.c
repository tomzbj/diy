#include "nrf24le1.h"
#include "nrf24le1_gpio.h"

void GPIO_Init(GPIO_TypeDef gpio, GPIO_InitTypeDef* GPIO_InitStructure)
{
    unsigned char i;
    if(GPIO_InitStructure->GPIO_Mode & 0x10) {
        switch(gpio) {
            case GPIO0:
                P0DIR |= GPIO_InitStructure->GPIO_Pin;
                break;
            case GPIO1:
                P1DIR |= GPIO_InitStructure->GPIO_Pin;
                break;
            case GPIO2:
                P2DIR |= GPIO_InitStructure->GPIO_Pin;
                break;
            case GPIO3:
                P3DIR |= GPIO_InitStructure->GPIO_Pin;
                break;
        }
    }
    else {
        switch(gpio) {
            case GPIO0:
                P0DIR &= ~GPIO_InitStructure->GPIO_Pin;
                break;
            case GPIO1:
                P1DIR &= ~GPIO_InitStructure->GPIO_Pin;
                break;
            case GPIO2:
                P2DIR &= ~GPIO_InitStructure->GPIO_Pin;
                break;
            case GPIO3:
                P3DIR &= ~GPIO_InitStructure->GPIO_Pin;
                break;
        }
    }
    for(i = 0; i < 8; i++) {
        if(GPIO_InitStructure->GPIO_Pin & _BV(i)) {
            switch(gpio) {
                case GPIO0:
                    P0CON = GPIO_InitStructure->GPIO_Mode | i;
                    break;
                case GPIO1:
                    P1CON = GPIO_InitStructure->GPIO_Mode | i;
                    break;
                case GPIO2:
                    P2CON = GPIO_InitStructure->GPIO_Mode | i;
                    break;
                case GPIO3:
                    P3CON = GPIO_InitStructure->GPIO_Mode | i;
                    break;
            }
        }
    }
}
