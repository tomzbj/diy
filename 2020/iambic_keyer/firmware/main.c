#include <stdio.h>
#include "stm32f0xx.h"
#include "platform.h"
#include "iambic.h"

void Init(void)
{
    SystemInit();
    SysTick_Config(8000UL);
    TIMER_Config();
    KEY_Config();
    IAMBIC_Config();
}

int main(void)
{
    Init();
    while(1) {
        IAMBIC_Loop();
    }
    return 0;
}
