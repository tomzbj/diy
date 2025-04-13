#include "misc.h"
#include "timer_gdf4.h"
#include "charlie.h"

extern "C" void TIMER6_IRQHandler(void)
{
  if(SET == timer_interrupt_flag_get(TIMER6, TIMER_INT_UP)) {
    extern class CHARLIE charlie;
    charlie.poll();
    timer_interrupt_flag_clear(TIMER6, TIMER_INT_UP);
  }
}

void TIMER_Config(void)
{
  timer_parameter_struct tis;

  // timer5 as dac_trigger
  rcu_periph_clock_enable (RCU_TIMER6);
  timer_deinit (TIMER6);
//  timer_parameter_struct tis;
  tis.prescaler = 0;
  tis.alignedmode = TIMER_COUNTER_EDGE;
  tis.counterdirection = TIMER_COUNTER_UP;
  tis.period = 599UL;    // 120M / 1000 = 120ksps
  tis.clockdivision = TIMER_CKDIV_DIV1;
  tis.repetitioncounter = 0;
  timer_init(TIMER6, &tis);

  timer_auto_reload_shadow_enable(TIMER6);
  timer_master_output_trigger_source_select(TIMER6, TIMER_TRI_OUT_SRC_UPDATE);

  timer_interrupt_enable(TIMER6, TIMER_INT_UP);
  timer_enable(TIMER6);
  nvic_irq_enable(TIMER6_IRQn, 0, 0);
  print_log();
}
