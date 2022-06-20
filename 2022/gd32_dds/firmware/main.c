#include "misc.h"
#include "platform.h"

void SystemInit(void)
{
    nvic_vector_table_set(NVIC_VECTTAB_FLASH, 0x2000);
    __enable_irq();
    rcu_deinit();
    rcu_osci_on(RCU_HXTAL);
    rcu_osci_stab_wait(RCU_HXTAL);
    rcu_ahb_clock_config(RCU_AHB_CKSYS_DIV1);
    rcu_apb1_clock_config(RCU_APB1_CKAHB_DIV1);
    rcu_apb2_clock_config(RCU_APB2_CKAHB_DIV1);
    rcu_hxtal_prediv_config(RCU_PLL_PREDV2);
    rcu_pll_config(RCU_PLLSRC_HXTAL_IRC48M, RCU_PLL_MUL30);
//    rcu_pll_config(RCU_PLLSRC_IRC8M_DIV2, RCU_PLL_MUL21);
    rcu_osci_on(RCU_PLL_CK);
    rcu_osci_stab_wait(RCU_PLL_CK);
    rcu_system_clock_source_config(RCU_CKSYSSRC_PLL);
    while(rcu_system_clock_source_get() != RCU_SCSS_PLL);
}

int main(void)
{
    void BSP_Init(void);
    BSP_Init();
    while(1) {
        zu_poll();
        zt_poll();
//        pmu_to_sleepmode(WFI_CMD);
//        ADC_Poll();
    }
}
