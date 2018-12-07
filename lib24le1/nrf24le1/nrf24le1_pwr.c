#include "nrf24le1.h"

void PWR_SetMode(PWR_ModeTypeDef* mode)
{
    PWRDWN = (PWRDWN & 0xf8) | *mode;
}

void PWR_OpModeCtrl(PWR_OpModeTypeDef* opmode)
{
    OPMCON = *(unsigned char*)(opmode);
}

void PWR_WakeupSourceConfig(PWR_WakeupSourceTypeDef* source)
{
    WUCON = *(unsigned char*)(source);
}

void PWR_WakeupOnPinConfig(PWR_WakeupOnPinTypeDef* pins)
{
    WUOPC0 = *(unsigned char*)(pins);
    WUOPC1 = *(unsigned char*)(pins + 1);
}

void PWR_FailComparatorConfig(PWR_FailComparatorTypeDef* comp)
{
    POFCON = *(unsigned char*)(comp);
}
