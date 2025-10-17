#include "misc.h"
#include "timer_gdf4.h"
//#include "adc_gdf3.h"
#include "rf.h"

extern "C" void TIMER1_IRQHandler(void)
{
  if(SET == timer_interrupt_flag_get(TIMER1, TIMER_INT_UP)) {
    timer_interrupt_flag_clear(TIMER1, TIMER_INT_UP);
    if(1) {
      static int count = 0;
      ++count %= 2133333;
      if(!count)
        printf("TIM1\n");
    }
  }
}

extern "C" void TIMER6_IRQHandler(void)
{
  if(SET == timer_interrupt_flag_get(TIMER6, TIMER_INT_UP)) {
    timer_interrupt_flag_clear(TIMER6, TIMER_INT_UP);
    extern class RF rf;
    rf.play();

    if(0) {
      static int count = 0;
      ++count %= 50000;
      if(count == 0)
        printf("TIM6\n");
    }
  }
}

void TIMER_Config(void)
{
  {
    rcu_periph_clock_enable (RCU_TIMER1);    // timer2 as adc_trigger
    timer_deinit (TIMER1);
    timer_parameter_struct tis;
    tis.prescaler = 0;
    tis.alignedmode = TIMER_COUNTER_EDGE;
    tis.counterdirection = TIMER_COUNTER_UP;
//    tis.period = 134UL;    // 288M / 135 = 2.13Msps, 2.13M * 3 = 6.4Msps
    tis.period = 89UL;    // 192M / 90 = 2.13Msps, 2.13M * 3 = 6.4Msps
    tis.clockdivision = TIMER_CKDIV_DIV1;
    tis.repetitioncounter = 0;
    timer_master_output_trigger_source_select(TIMER1, TIMER_TRI_OUT_SRC_UPDATE);
    timer_init(TIMER1, &tis);
    timer_auto_reload_shadow_enable(TIMER1);
    timer_oc_parameter_struct tos;

    tos.ocpolarity = TIMER_OC_POLARITY_HIGH;
    tos.outputstate = TIMER_CCX_ENABLE;
    timer_channel_output_config(TIMER1, TIMER_CH_1, &tos);
    timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_1, 1);
    timer_channel_output_mode_config(TIMER1, TIMER_CH_1, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER1, TIMER_CH_1, TIMER_OC_SHADOW_DISABLE);

    timer_enable(TIMER1);
//    timer_interrupt_enable(TIMER1, TIMER_INT_UP);
//    nvic_irq_enable(TIMER1_IRQn, 0, 0);
    print_log();
  }

  if(1) {
    rcu_periph_clock_enable (RCU_TIMER6);    // timer5 as dac_trigger
    timer_deinit (TIMER6);
    timer_parameter_struct tis;
    tis.prescaler = 0;
    tis.alignedmode = TIMER_COUNTER_EDGE;
    tis.counterdirection = TIMER_COUNTER_UP;
    tis.period = 3839;    // 192M / 3840 = 50ksps
    tis.clockdivision = TIMER_CKDIV_DIV1;
    tis.repetitioncounter = 0;
//    timer_master_output_trigger_source_select(TIMER6, TIMER_TRI_OUT_SRC_UPDATE);
    timer_init(TIMER6, &tis);
    timer_auto_reload_shadow_enable(TIMER6);
    timer_interrupt_enable(TIMER6, TIMER_INT_UP);
    nvic_irq_enable(TIMER6_IRQn, 2, 2);
    timer_enable(TIMER6);
    print_log();
  }
}
