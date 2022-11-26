#include "misc.h"
#include "platform.h"

void TASKS_Config(void)
{
    zt_bind(DISP_Update, 250, 1);
    print_log();
}
