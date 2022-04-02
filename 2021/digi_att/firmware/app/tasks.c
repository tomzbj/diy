#include "platform.h"
#include "misc.h"

static void hello(void)
{
    printf("Hello, world.\n");
}

static void att(void)
{
    static int old_att;
    DataConfig_t* pdc = DC_Get();
    if(old_att != pdc->cfg.att) {
        zss_set_num(pdc->cfg.att);
        int rel = pdc->cfg.att / 3;
        for(int i = 0; i < 4; i++) {
            if((1 << i) & rel)
                REL_Set(i, 1);
            else
                REL_Set(i, 0);
        }
    }
    old_att = pdc->cfg.att;
}

void TASKS_Config(void)
{
    zt_bind(hello, 500, 1);
    zt_bind(att, 500, 1);
    zt_bind(zss_update, 5, 1);
    zt_bind(zk_poll, 10, 1);
}
