#include "misc.h"
#include "platform.h"

static void hello(void)
{
//    printf("%d %lu\n", HXTAL_VALUE, rcu_clock_freq_get(CK_SYS));
    GPIOA->ODR ^= GPIO_Pin_7;
}

static void cw_task(void)
{
    static int count;
    const DataConfig_t* pdc = DC_Get();
    count++;
    if(count >= 1200 / pdc->cfg.cw_wpm) {
        count = 0;
        cw_poll();
    }
}

void TASKS_Config(void)
{
    zt_bind(hello, 250, 1);
    zt_bind(cw_task, 1, 1);
//    zt_bind(DISP_Poll, 250, 1);
    zt_bind((void*)GUI_Exec, 100, 1);
    zt_bind((void*)GUI_TOUCH_Exec, 10, 1);

    printf("[%s: %d] TASKS Initialized.\n", __FILE__, __LINE__);
}
