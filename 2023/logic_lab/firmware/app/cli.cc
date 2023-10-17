#include "misc.h"
#include "platform.h"

#define ok() printf("ok\n");

void CLI_Config(void)
{
    cli.bind("test", [](const char**) {puts("TEST OK");});
    cli.bind("reboot", [](const char**) {puts("REBOOT."); NVIC_SystemReset();});

    cli.bind("freq", "rd", [](const char**) {
        printf("%lu\n", rcu_clock_freq_get(CK_SYS));});

    cli.bind("zt_start", "wr", [](const char* tokens[]) {
        zt.start(strtol(tokens[2], NULL, 10)); ok();});
    cli.bind("zt_stop", "wr", [](const char* tokens[]) {
        zt.stop(strtol(tokens[2], NULL, 10)); ok();});
}
