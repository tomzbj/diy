#include "platform.h"

static int key_read(int key_id)
{
    unsigned short keys[1] = {GPIO_Pin_8};
    if((GPIOB->IDR & keys[key_id]) == 0)
        return 1;
    else
        return 0;
}

static void key_action(int key_id, zk_action_t action)
{
    DataConfig_t* pdc = DC_Get();

    if(action == ZK_PRESS) {
//        CHECK_Start();
//        if(pdc->status.mode == DC_MODE_FIND_OFF) {
//        }
//        else if(pdc->status.mode == DC_MODE_FIND_ON) {
//        }
//        else {
//        }
    }
    if(action == ZK_LONGPRESS) {
        if(pdc->status.mode == DC_MODE_NORMAL)
            pdc->status.mode = DC_MODE_FIND_OFF;
        else if(pdc->status.mode == DC_MODE_FIND_OFF)
            pdc->status.mode = DC_MODE_FIND_ON;
        else if(pdc->status.mode == DC_MODE_FIND_ON)
            pdc->status.mode = DC_MODE_NORMAL;
        else
            pdc->status.mode = DC_MODE_NORMAL;
    }
}

void KEY_Config(void)
{
    zk_t zk;
    static int count[1];
    static int state[1];
    zk.read_f = key_read;
    zk.action_f = key_action;
    zk.key_num = 1;
    zk.key_count = count;
    zk.key_state = state;
    zk.longpress_count = 200;
    zk_init(&zk);
}
