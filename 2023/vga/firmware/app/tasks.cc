#include "misc.h"
#include "platform.h"

static void hello(void)
{
    printf("Hello, world.\n");
}

static void led(void)
{
    static int count = 0;
    count++;
    if(count % 500 < 10)
        GPIO_BOP (GPIOA) = GPIO_PIN_0;
    else
        GPIO_BC(GPIOA) = GPIO_PIN_0;

//    if(count % 500 == 0) printf("%lu %lu\n", TIMER_CNT(TIMER5), DMA_CH2CNT(DMA1));
}

static void update_params(void)
{
    static typeof(dc.cfg) old_cfg;
    if(memcmp(&(old_cfg), &(dc.cfg), sizeof(old_cfg)) != 0) {
    }
    old_cfg = dc.cfg;
}


void TASKS_Config(void)
{
    zt.bind(led, 1, 1);
    zt.bind(hello, 500, 0);
//    zt.bind(RF_RSSI, 250, 1);
    zt.bind(update_params, 500, 0);

    print_log();
}
