#include "misc.h"
#include "platform.h"
#include <pthread.h>

void* __dso_handle = 0;
void SystemInit(void)
{
    nvic_vector_table_set(NVIC_VECTTAB_FLASH, 0x2800);
    __enable_irq();
    rcu_deinit();
    rcu_osci_on (RCU_HXTAL);
    rcu_osci_stab_wait(RCU_HXTAL);
    rcu_periph_clock_enable (RCU_PMU);
    pmu_ldo_output_select (PMU_LDOVS_HIGH);
    rcu_ahb_clock_config (RCU_AHB_CKSYS_DIV1);
    rcu_apb1_clock_config (RCU_APB1_CKAHB_DIV2);
    rcu_apb2_clock_config (RCU_APB2_CKAHB_DIV1);
    rcu_pllpresel_config (RCU_PLLPRESRC_HXTAL);
    rcu_predv0_config (RCU_PREDV0_DIV2);
    rcu_pll_config(RCU_PLLSRC_HXTAL_IRC48M, RCU_PLL_MUL25);    // 200M
    rcu_osci_on (RCU_PLL_CK);
    rcu_osci_stab_wait(RCU_PLL_CK);
    pmu_highdriver_mode_enable();
    while(pmu_flag_get(PMU_FLAG_HDRF) == RESET);
    pmu_highdriver_switch_select (PMU_HIGHDR_SWITCH_EN);
    while(pmu_flag_get(PMU_FLAG_HDSRF) == RESET);
    rcu_system_clock_source_config (RCU_CKSYSSRC_PLL);
    while(rcu_system_clock_source_get() != RCU_SCSS_PLL);
}

int main(void)
{
    b.init();
    while(1) {
        b.run();
    }
}
