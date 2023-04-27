#include "misc.h"
#include "platform.h"

#define ALARM_TIME_INTERVAL 10

extern "C" {
void* __dso_handle = 0;
void SystemInit(void)
{
    nvic_vector_table_set(NVIC_VECTTAB_FLASH, 0x2000);
    __enable_irq();
    rcu_deinit();

    rcu_ahb_clock_config (RCU_AHB_CKSYS_DIV1);
    rcu_apb1_clock_config (RCU_APB1_CKAHB_DIV1);
    rcu_apb2_clock_config (RCU_APB2_CKAHB_DIV1);

    rtc_parameter_struct rpis;
    rtc_alarm_struct alarm;
    rcu_periph_clock_enable (RCU_PMU); /* enable PMU clock */
    pmu_backup_write_enable(); /* allow access to BKP domain */

    rcu_osci_on (RCU_IRC40K); /* enable IRC40K */
    rcu_osci_stab_wait(RCU_IRC40K); /* wait till IRC40K is ready */
    rcu_rtc_clock_config (RCU_RTCSRC_IRC40K); /* select RCU_IRC40K as RTC clock source */
    rcu_periph_clock_enable (RCU_RTC); /* enable RTC clock */

    rpis.rtc_factor_asyn = 0x63;
    rpis.rtc_factor_syn = 0x18F;
    rpis.rtc_year = 0x16;
    rpis.rtc_day_of_week = RTC_SATURDAY;
    rpis.rtc_month = RTC_APR;
    rpis.rtc_date = 0x30;
    rpis.rtc_display_format = RTC_24HOUR;
    rpis.rtc_am_pm = RTC_AM;

    /* configure current time */
    rpis.rtc_hour = 0x00;
    rpis.rtc_minute = 0x00;
    rpis.rtc_second = 0x00;
    rtc_init(&rpis);

    rtc_alarm_disable(); /* disable RTC alarm */
    alarm.rtc_alarm_mask = RTC_ALARM_DATE_MASK | RTC_ALARM_HOUR_MASK
        | RTC_ALARM_MINUTE_MASK;
    alarm.rtc_weekday_or_date = RTC_ALARM_DATE_SELECTED;
    alarm.rtc_alarm_day = 0x31;
    alarm.rtc_am_pm = RTC_AM;

    /* RTC alarm value */
    alarm.rtc_alarm_hour = 0;
    alarm.rtc_alarm_minute = 0;
    alarm.rtc_alarm_second = 3;

    /* configure RTC alarm */
    rtc_alarm_config(&alarm);

    rtc_interrupt_enable (RTC_INT_ALARM);
    rtc_alarm_enable();

    rtc_flag_clear (RTC_FLAG_ALARM0);
}
}

int main(void)
{
    b.init();
    while(1) {
        b.run();
    }
}
