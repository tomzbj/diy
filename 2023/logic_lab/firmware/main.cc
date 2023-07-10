#include "misc.h"
#include "platform.h"

extern "C" {
void* __dso_handle = 0;
void SystemInit(void)
{
    nvic_vector_table_set(NVIC_VECTTAB_FLASH, 0x2000);
    __enable_irq();
    rcu_deinit();
    rcu_periph_clock_enable (RCU_PMU);
    pmu_ldo_output_select (PMU_LDOVS_HIGH);
    fmc_wscnt_set (WS_WSCNT_0);
    rcu_ahb_clock_config (RCU_AHB_CKSYS_DIV1);
    rcu_apb1_clock_config (RCU_APB1_CKAHB_DIV1);
    rcu_apb2_clock_config (RCU_APB2_CKAHB_DIV1);

    rcu_hxtal_prediv_config (RCU_PLL_PREDV2);
    rcu_pll_config(RCU_PLLSRC_IRC8M_DIV2, RCU_PLL_MUL4);
    rcu_osci_on (RCU_PLL_CK);
    rcu_osci_stab_wait(RCU_PLL_CK);
    rcu_system_clock_source_config (RCU_CKSYSSRC_PLL);
    while(rcu_system_clock_source_get() != RCU_SCSS_PLL);
}
}

int main(void)
{
    b.init();
    while(1) {
        b.run();
    }
}
