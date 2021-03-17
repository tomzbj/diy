#include "misc.h"
#include "platform.h"

static void hello(void)
{
//    xprintf("Hello, world.\n");
    RCC_ClocksTypeDef clocks;
    RCC_GetClocksFreq(&clocks);
    xprintf("%ld\n", clocks.SYSCLK_Frequency);
}

static void adc(void)
{
    DataConfig_t* pdc = DC_Get();
    printf("%.3f %.3f\n", pdc->data.curr, pdc->data.vbat1);
}

static void update_params(void)
{
    static DataConfig_t old_dc;

    DataConfig_t* pdc = DC_Get();
    if(memcmp(&(old_dc.cfg), &(pdc->cfg), sizeof(old_dc.cfg)) != 0
        || memcmp(&(old_dc.status), &(pdc->status), sizeof(old_dc.status))
            != 0) {
        if(pdc->status.discharge_state == DC_DISCHARGE_STATE_ON) {
            DAC_SetChannel1Data(DAC_Align_12b_R,
                (unsigned short)(pdc->cfg.discharge_curr / 1000.0 * 0.15 * 11
                    / 3.3 * 4094));
        }
        else
            DAC_SetChannel1Data(DAC_Align_12b_R, 0);
    }
    old_dc = *pdc;
}

static void capacity(void)
{
    DataConfig_t* pdc = DC_Get();
    pdc->data.total_capacity += (pdc->data.curr / 3600.0);
    if(pdc->data.vbat1 < pdc->cfg.end_volt / 1000.0)
        pdc->status.discharge_state = DC_DISCHARGE_STATE_OFF;
    printf("%.3fV %.1fmA %.1fmAh\n", pdc->data.vbat1, pdc->data.curr, pdc->data.total_capacity);
}

void TASKS_Poll(void)
{
    zt_poll();
}

void TASKS_Config(void)
{
    zt_bind(hello, 500, 0);
    zt_bind(zk_poll, 1, 1);
    zt_bind(update_params, 10, 1);
    zt_bind(DISP_Update, 250, 1);
    zt_bind(adc, 500, 0);
    zt_bind(capacity, 1000, 1);
}
