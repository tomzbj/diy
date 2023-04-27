#include "board.h"
#include "misc.h"
#include "platform.h"

board b;
zuart u0(128, 128);
ztask zt(6);

flash_eeprom eep(0x08002000, 2048);
data_config dc(eep);

void board::sleep(void)
{
    rcu_periph_clock_disable (RCU_GPIOA);
    rcu_periph_clock_disable (RCU_GPIOB);
    rcu_periph_clock_disable (RCU_USART0); /* disable USART clock */
    rcu_periph_clock_disable (RCU_SPI0);
//        pmu_to_sleepmode (WFI_CMD);
    rcu_periph_clock_enable (RCU_PMU);
    pmu_to_standbymode (WFE_CMD);
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_USART0); /* enable USART clock */
    rcu_periph_clock_enable(RCU_SPI0);
}

void board::init(void)
{
    SysTick_Config(rcu_clock_freq_get(CK_SYS) / 100UL);
    GPIO_Config();
    USART_Config();
    u0.bind(CLI_Parse, USART0_ReadByte, USART0_WriteByte);
    printf("[%s: %d] USART Initialized.\n", __FILE__, __LINE__);
    {
        rtc_parameter_struct rps;
        rtc_current_time_get(&rps);
        printf("%d %d %d\n", rps.rtc_hour, rps.rtc_minute, rps.rtc_second);
    }
    SPI_Config();
    RF_Config();
    TASKS_Config();

    printf("SYSCLK: %lu\n", rcu_clock_freq_get(CK_SYS));
    printf("All peripherals initialized.\n");
}

void board::run(void)
{
    SHT_Read();
    RF_Poll();
    GPIO_BC (GPIOA) = GPIO_PIN_2;
    sleep();
}
