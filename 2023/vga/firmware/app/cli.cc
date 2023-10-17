#include "misc.h"
#include "platform.h"

#define MSG_LEN 256
#define ok() printf("ok\n");

static void cli_rd_sbrk_sp(const char* tokens[])
{
    register unsigned char* sp asm("sp");
    auto a1 = sp;
    auto a2 = malloc(1);
    printf("%p %p %d\n", a1, a2, (int)a1 - (int)a2);
    free(a2);
}

static void cli_wr_debug(const char* tokens[])
{
    if(strcasecmp(tokens[2], "on") == 0)
        dc.state.debug = 1;
    else if(strcasecmp(tokens[2], "off") == 0)
        dc.state.debug = 0;
    ok();
}

void CLI_Config(void)
{
    /* @formatter:on */
    cli.bind("test", [](const char**) {puts("TEST OK");});
    cli.bind("reboot", [](const char**) {puts("REBOOT."); NVIC_SystemReset();});

    cli.bind("sbrk_sp", "rd", cli_rd_sbrk_sp);
    cli.bind("freq", "rd", [](const char**) {    //
            printf("%lu\n", rcu_clock_freq_get(CK_SYS));
        });

    cli.bind("zt_start", "wr", [](    //
        const char* tokens[]) {zt.start(strtol(tokens[2], NULL, 10)); ok();});
    cli.bind("zt_stop", "wr", [](    //
        const char* tokens[]) {zt.stop(strtol(tokens[2], NULL, 10)); ok();});

    cli.bind("debug", "wr", cli_wr_debug);

    /* @formatter:on */
}
