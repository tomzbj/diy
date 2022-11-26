#include "misc.h"
#include "platform.h"

void TIMER_Config(void)
{
    rcu_periph_clock_enable(RCU_TIMER13);

    timer_parameter_struct tis;
    timer_oc_parameter_struct tocis;

    // T2_C0 as lcd brightness, 10Hz
    timer_deinit(TIMER13);
    tis.prescaler = 20999;
    tis.alignedmode = TIMER_COUNTER_EDGE;
    tis.counterdirection = TIMER_COUNTER_UP;
    tis.period = 9999UL;
    tis.clockdivision = TIMER_CKDIV_DIV4;   // 84M / 4 / 21000 / 1000 = 1Hz
    tis.repetitioncounter = 0;
    timer_init(TIMER13, &tis);

    tocis.ocpolarity = TIMER_OC_POLARITY_HIGH;
    tocis.outputstate = TIMER_CCX_ENABLE;
    tocis.ocidlestate = TIMER_OC_IDLE_STATE_LOW;

    timer_channel_output_config(TIMER13, TIMER_CH_0, &tocis);
    timer_channel_output_pulse_value_config(TIMER13, TIMER_CH_0, 5000);    //tis.period / 2);
    timer_channel_output_mode_config(TIMER13, TIMER_CH_0, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER13, TIMER_CH_0,
        TIMER_OC_SHADOW_DISABLE);

    timer_auto_reload_shadow_enable(TIMER13);
    timer_enable(TIMER13);

    print_log();
}
