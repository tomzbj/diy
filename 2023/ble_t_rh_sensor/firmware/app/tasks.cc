#include "misc.h"
#include "platform.h"

static void hello(void)
{
    printf("%d %lu\n", HXTAL_VALUE, rcu_clock_freq_get(CK_SYS));
}

static void led(void)
{
    static int count = 5;
    count++;
    count %= 10;
    if(count <= 1)
        GPIO_BOP (GPIOA) = GPIO_PIN_2;
    else
        GPIO_BC(GPIOA) = GPIO_PIN_2;
}

void TASKS_Config(void)
{
    zt.bind(hello, 250, 0);
    zt.bind(led, 10, 0);
//    zt.bind(SHT_Read, 250, 1);
//    zt.bind(RF_Poll, 250, 1);

    _dbg();
}
