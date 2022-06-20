#include "misc.h"
#include "platform.h"

static void hello(void)
{
    register unsigned char* sp asm("sp");
    printf("%08lX\n", (unsigned long)sp);
}

void TASKS_Config(void)
{
    zt_bind(hello, 250, 0);
    zt_bind(DISP_Update, 250, 1);
//    zt_bind(led, 10, 1);
    zt_bind(zk_poll, 1, 1);

    print_log();
}
