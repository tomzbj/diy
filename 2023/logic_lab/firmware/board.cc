#include "misc.h"
#include "platform.h"

Board b;
ZUart u0(128, 128);
ZTask zt(6);
ZCli cli(20);
FlashEEP eep(0x08001c00, 1024);
DataConfig dc(eep);

static void parser_f(const void* msg, int size)
{
    cli.parse(msg, size);
}

void Board::init(void)
{
    SysTick_Config(rcu_clock_freq_get(CK_SYS) / 1000UL);
    GPIO_Config();
    USART_Config();
    u0.bind(parser_f, USART0_ReadByte, USART0_WriteByte);
    printf("[%s: %d] USART Initialized.\n", __FILE__, __LINE__);
    CLI_Config();
    GATES_Config();
    TASKS_Config();

    printf("SYSCLK: %lu\n", rcu_clock_freq_get(CK_SYS));
    printf("All peripherals initialized.\n");
}

void Board::run(void)
{
    u0.poll();
    zt.poll();
}
