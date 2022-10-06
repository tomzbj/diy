#include "misc.h"
#include "platform.h"

void TIMER_Config(void)
{
    rcu_periph_clock_enable(RCU_TIMER13);

    timer_parameter_struct tis;

    timer_deinit(TIMER13);
    tis.prescaler = 0;
    tis.alignedmode = TIMER_COUNTER_EDGE;
    tis.counterdirection = TIMER_COUNTER_UP;
    tis.period = 2191;
    tis.clockdivision = TIMER_CKDIV_DIV1;
    tis.repetitioncounter = 0;
    timer_init(TIMER13, &tis);

    timer_auto_reload_shadow_enable(TIMER13);
    timer_enable(TIMER13);

    timer_interrupt_flag_clear(TIMER13, TIMER_INT_UP);
    timer_interrupt_enable(TIMER13, TIMER_INT_UP);
    nvic_irq_enable(TIMER13_IRQn, 0, 0);

    print_log();
}

