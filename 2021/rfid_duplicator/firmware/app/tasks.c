#include "misc.h"
#include "platform.h"

static void hello(void)
{
    xprintf("Hello, world.\n");
}

static void rfid(void)
{
    DataConfig_t* pdc = DC_Get();
    pdc->data.card_id = RFID_ReadID();
}

void TASKS_Config(void)
{
    zt_bind(hello, 500, 0);
    zt_bind(rfid, 500, 1);
    zt_bind(DISP_Update, 500, 1);
    zt_bind(zk_poll, 1, 1);
}
