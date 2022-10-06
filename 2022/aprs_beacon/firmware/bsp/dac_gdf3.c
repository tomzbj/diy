#include "misc.h"

void DAC_Config(void)
{
    rcu_periph_clock_enable(RCU_DAC);
    dac_deinit();
    dac_output_buffer_enable();
//    dac_trigger_enable();
//    dac_trigger_source_config(DAC_TRIGGER_T5_TRGO);
    dac_enable();
}
