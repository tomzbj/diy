#include "misc.h"
#include "iambic.h"
#include "pwr.h"

#define IsClick(key) ((key & 0xf) == 0xc)
#define IsLongPress(key) ((key & 0xffffff) == 0xfffffe)

typedef enum {
    KEY_NONE, KEY1_CLICK, KEY2_CLICK, KEY1_LONGPRESS, KEY2_LONGPRESS,
    KEY3_CLICK, KEY3_LONGPRESS
} KEY_Status_t;

void KEY_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    // PB11~12 as in for keys 
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void KEY_Parse(KEY_Status_t status)
{
    switch(status) {
        case KEY1_CLICK:
            IAMBIC_ToggleWPM();
            break;
        case KEY1_LONGPRESS:
            PWR_Sleep();
            break;
        default:
            break;
    }
}

void KEY_Read(void)
{
    static unsigned long key1;
    KEY_Status_t status = KEY_NONE;
    key1 <<= 1;

    if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1))
        key1 |= 1;

    if(IsLongPress(key1))
        status = KEY1_LONGPRESS;
    else if(IsClick(key1))
        status = KEY1_CLICK;
    KEY_Parse(status);
}

