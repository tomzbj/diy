#include "misc.h"
#include "led.h"
#include "adc_f0.h"

void LED_Config(void)
{
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void LED_Show(void)
{
    unsigned pattern0[] = {0x00, 0xe0};
    unsigned pattern1[] = {0x00, 0x20};
    unsigned pattern2[] = {0x20, 0x60};
    unsigned pattern3[] = {0x60, 0xe0};
    unsigned pattern4[] = {0xe0, 0xe0};
    static int count = 0;
    BattStatusTypeDef status = ADC_GetBattStatus();

    switch(status) {
        case BATT_ERR:
            GPIOA->ODR = (GPIOA->ODR & 0x1f) | pattern0[count % 2];
            break;
        case BATT_PRE_CHARGE:
            GPIOA->ODR = (GPIOA->ODR & 0x1f) | pattern1[count % 2];
            break;
        case BATT_FAST_CHARGE:
            GPIOA->ODR = (GPIOA->ODR & 0x1f) | pattern2[count % 2];
            break;
        case BATT_TICKLE_CHARGE:
            GPIOA->ODR = (GPIOA->ODR & 0x1f) | pattern3[count % 2];
            break;
        case BATT_FULL:
            GPIOA->ODR = (GPIOA->ODR & 0x1f) | pattern4[count % 2];
            break;
    }
    count++;
}
