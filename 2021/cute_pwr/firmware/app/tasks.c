#include "misc.h"
#include "platform.h"

static void hello(void)
{
//    printf("Hello, world.\n");
//    DataConfig_t* pdc = DC_Get();
//    printf("%.3f %.3f %.3f\n", pdc->data.vout, pdc->data.iout, pdc->data.vbat);
}

void update_params(void)
{
    static DataConfig_t old_dc;
    DataConfig_t* pdc = DC_Get();

    if(memcmp(&old_dc, pdc, sizeof(old_dc)) != 0) {
        if(old_dc.status.v_dcdc != pdc->status.v_dcdc) {
            float duty = (pdc->status.v_dcdc - 1.25 + 0.36) / (150.0 / 37.0)
                / 3.3;
            if(duty > 1.0)
                duty = 1.0;
            if(duty < 0.0)
                duty = 0.0;
            TIM_SetCompare4(TIM1, TIM1->ARR * duty);
        }
        if(old_dc.status.v_sp != pdc->status.v_sp) {
            float dac_perc = pdc->status.v_sp / 66.0 * 15.0 / 3.3;
            if(dac_perc < 0.0)
                dac_perc = 0.0;
            if(dac_perc > 1.0)
                dac_perc = 1.0;
            DAC_SetChannel1Data(DAC_Align_12b_R, dac_perc * 4095.0);
        }
        if(old_dc.status.i_sp != pdc->status.i_sp) {
            float dac_perc = pdc->status.i_sp * 0.1 * 50 / 3.3;
            if(dac_perc < 0.0)
                dac_perc = 0.0;
            if(dac_perc > 1.0)
                dac_perc = 1.0;
            DAC_SetChannel2Data(DAC_Align_12b_R, dac_perc * 4095.0);
        }
    }
    pdc->status.v_dcdc = pdc->data.vout + 1;
    old_dc = *pdc;
}

void TASKS_Config(void)
{
//    zt_bind(zk_poll, 1, 1);
    zt_bind(hello, 500, 0);
    zt_bind(update_params, 500, 1);
    zt_bind((void*)GUI_Exec, 100, 1);
}
