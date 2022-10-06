#include "misc.h"
#include "platform.h"

static void hello(void)
{
//    printf("%d %lu\n", HXTAL_VALUE, rcu_clock_freq_get(CK_SYS));
    register unsigned char* sp asm("sp");
    printf("%08lX\n", (unsigned long)sp);
}

void TASKS_Config(void)
{
    zt_bind(hello, 250, 0);

    print_log();
}
