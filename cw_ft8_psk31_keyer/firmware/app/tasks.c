#include "platform.h"

static void hello(void)
{
    printf("Hello, world.\n");
}

static void cw_task(void)
{
    static int count;
    const DataConfig_t* pdc = DC_Get();
    count++;
    if(count >= 1200 / pdc->cfg.cw_wpm) {
        count = 0;
        cw_poll();
    }
}

static void params_update(void)
{
    static DataConfig_t old_dc;
    DataConfig_t* pdc = DC_Get();
    if(memcmp(&old_dc, pdc, sizeof(old_dc)) != 0) {
        unsigned long tmp = (DAC->CR & ~0x3CUL);
        if(pdc->cfg.tx_mode == DC_TX_MODE_CW
            || pdc->cfg.tx_mode == DC_TX_MODE_FT8) {
            tmp |= DAC_Trigger_T6_TRGO;
        }
        else if(pdc->cfg.tx_mode == DC_TX_MODE_BPSK) {
            tmp |= DAC_Trigger_None;
        }
        DAC->CR = tmp;
    }
    old_dc = *pdc;
}

void TASKS_Config(void)
{
    zt_bind(hello, 250, 0);
    zt_bind(zk_poll, 1, 1);
    zt_bind(cw_task, 1, 1);
    zt_bind(params_update, 500, 1);
}

void TASKS_Poll(void)
{
    zt_poll();
}
