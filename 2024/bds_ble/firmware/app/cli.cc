#include "misc.h"
#include "platform.h"

#define ok() printf("ok\n");

static void cli_wr_bds_on(const char** tokens)
{
    if(strcasecmp(tokens[2], "on") == 0) {
        ZPin::write(PA0, 0);
    }
    else if(strcasecmp(tokens[2], "off") == 0) {
        ZPin::write(PA0, 1);
    }
    ok();
}

void CLI_Config(void)
{
    cli.bind("test", [](const char**) {puts("TEST OK");});
    cli.bind("reboot", [](const char**) {puts("REBOOT."); NVIC_SystemReset();});
    cli.bind("freq", "rd", [](const char**) {
        printf("%lu\n",HAL_RCC_GetSysClockFreq());});
    cli.bind("bds", "wr", cli_wr_bds_on);

    auto adc_read_f = [](const char**) {printf("%lu\n", ADC1->DR);};
    auto infi_f = [](const char**) {__disable_irq(); while(1);};
    auto rtc_f = [](const char**) {rtc.show_calendar();};

    cli.bind("adc", "rd", adc_read_f);
    cli.bind("rtc", "rd", rtc_f);
    cli.bind("infi", "wr", infi_f);
}
