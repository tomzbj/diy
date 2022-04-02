#include "misc.h"
#include "platform.h"

static void hello(void)
{
    printf("Hello, world.\n");
}

void TASKS_Poll(void)
{
    zt_poll();
}

void update_params(void)
{
    /*
    static DataConfig_t old_dc;
    DataConfig_t* pdc = DC_Get();

    if(memcmp(&old_dc, pdc, sizeof(old_dc)) != 0) {
    }
    old_dc = *pdc;
    */
}

void TASKS_Config(void)
{
    zt_bind(hello, 250, 0);
    zt_bind(update_params, 500, 0);
}
