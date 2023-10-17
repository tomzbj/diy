#include "misc.h"
#include "platform.h"

Board b;
ZUart u0(512, 128);
ZTask zt(8);

FlashEEP eep(0x08002000, 2048);
DataConfig dc(eep);
ZCli cli(20);

static void parser_f(const void* msg, int size)
{
    cli.parse(msg, size);
}

void Board::run(void)
{
    u0.poll();
    zt.poll();
//    adc.poll();
}

void Board::init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    DWT->CYCCNT = 0;
//    SysTick_Config(rcu_clock_freq_get(CK_SYS) / 1000UL);
    SysTick_Config(16777215UL);//rcu_clock_freq_get(CK_SYS) / 1000UL);

    dc.init();
    u0.bind(parser_f, USART0_ReadByte, USART0_WriteByte);
    USART_Config();
    TIMER_Config();
    VGA_Config();
    CLI_Config();

    TASKS_Config();

    print_log();
    printf("SYSCLK: %lu\n", rcu_clock_freq_get(CK_SYS));
}
