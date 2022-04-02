#include "misc.h"
#include "platform.h"

static void hello(void)
{
    xprintf("Hello, world.\n");
}

static void update_params(void)
{
    static DataConfig_t old_dc;
    DataConfig_t* pdc = DC_Get();
    if(memcmp(&old_dc, pdc, sizeof(old_dc)) != 0) {
        VFO_SetFreq(pdc->data.freq * 1000UL);
//        xprintf("%lu\n", pdc->data.freq);
    }
    old_dc = *pdc;
}

void TASKS_Config(void)
{
    zt_bind(hello, 500, 0);
    zt_bind(update_params, 500, 1);
//    zt_bind(DISP_Update, 250, 1);
    zt_bind(zk_poll, 1, 1);
}
