#include "misc.h"
#include "platform.h"

void* __dso_handle = 0;

void SystemInit(void)
{
  __enable_irq();
  rcu_deinit();
  rcu_osci_on (RCU_HXTAL);
  rcu_osci_stab_wait(RCU_HXTAL);
  rcu_periph_clock_enable (RCU_PMU);
  pmu_ldo_output_select (PMU_LDOVS_HIGH);
  rcu_ahb_clock_config (RCU_AHB_CKSYS_DIV1);
  rcu_apb1_clock_config (RCU_APB1_CKAHB_DIV4);     // APB1 84M
  rcu_apb2_clock_config (RCU_APB2_CKAHB_DIV2);     // APB2 168M
  rcu_pll_config(RCU_PLLSRC_HXTAL, 16, 336, 2, 7);
  rcu_osci_on (RCU_PLL_CK);
  rcu_osci_stab_wait(RCU_PLL_CK);
  pmu_highdriver_mode_enable();
  while(pmu_flag_get(PMU_FLAG_HDRF) == RESET);
  pmu_highdriver_switch_select (PMU_HIGHDR_SWITCH_EN);
  while(pmu_flag_get(PMU_FLAG_HDSRF) == RESET);
  rcu_system_clock_source_config (RCU_CKSYSSRC_PLLP);
  while(rcu_system_clock_source_get() != RCU_SCSS_PLLP);
}

int main(void)
{
  void BSP_Init(void);
  BSP_Init();
  TASKS_Config();
  while(1);     //never reach here
}
