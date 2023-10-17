#include "misc.h"
#include "platform.h"

static void hello(void)
{
    printf("%d %lu\n", HXTAL_VALUE, rcu_clock_freq_get(CK_SYS));
}

void TASKS_Config(void)
{
    zt.bind(hello, 250, 0);
    zt.bind(GATES_Task, 10, 1);
//    zt.bind(DISP_Update, 500, 1);

    _dbg();
}
