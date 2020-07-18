#include "misc.h"
#include "platform.h"

static void hello(void)
{
    printf("Hello, world.\n");
}

static void test(void)
{
//    GPIOB->ODR ^= GPIO_Pin_5;
}
void TASKS_Poll(void)
{
    zt_poll();
}

void TASKS_Config(void)
{
    zt_bind(zk_poll, 1, 1);
    zt_bind(test, 1, 0);
    zt_bind(hello, 250, 0);
    zt_bind(OSC_UpdateParams, 500, 1);
    zt_bind(UI_DrawTrace, 500, 1);
//    zt_bind(DISP_Poll, 100, 0);
//    zt_bind(CURLOOP_Update, 100, 1);
}
