#include "misc.h"
#include "platform.h"

void* __dso_handle = 0;

void SystemInit(void)
{
  SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2)); // enable fpu

  __enable_irq();
  rcu_deinit();
  rcu_osci_on (RCU_HXTAL);
  rcu_osci_stab_wait(RCU_HXTAL);
  rcu_periph_clock_enable (RCU_PMU);
  pmu_ldo_output_select (PMU_LDOVS_HIGH);
  rcu_ahb_clock_config (RCU_AHB_CKSYS_DIV1);
  rcu_apb1_clock_config (RCU_APB1_CKAHB_DIV2);     // APB1 60M
  rcu_apb2_clock_config (RCU_APB2_CKAHB_DIV1);     // APB2 120M
  rcu_pll_config(RCU_PLLSRC_HXTAL, 8, 192, 2, 4);
  rcu_osci_on (RCU_PLL_CK);
  rcu_osci_stab_wait(RCU_PLL_CK);
  pmu_highdriver_mode_enable();
  while(pmu_flag_get(PMU_FLAG_HDRF) == RESET);
  pmu_highdriver_switch_select (PMU_HIGHDR_SWITCH_EN);
  while(pmu_flag_get(PMU_FLAG_HDSRF) == RESET);
  rcu_system_clock_source_config (RCU_CKSYSSRC_PLLP);
  while(rcu_system_clock_source_get() != RCU_SCSS_PLLP);
  rcu_timer_clock_prescaler_config (RCU_TIMER_PSC_MUL2);    // TIMER CLK x4, =240M
}

int main(void)
{
  void BSP_Init(void);
  BSP_Init();
  while(1) {
    _delay_us(1);
    events.poll();
    extern class RF rf;
    rf.poll();
    if(u0.idle()) {
//_dbg();
      char buf[1024];
      memset(buf, 0, sizeof(buf));
      char c = u0.peek();
      int len = u0.read(buf, sizeof(buf));
      if(c == '#')
        cli.parse(buf, len);
    }
  }
}
