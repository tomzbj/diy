#include "misc.h"
#include "platform.h"

enum {
    KEY_TOP = 0, KEY_BOTTOM = 1
};

static int key_read(int key_id)
{
    unsigned short keys[2] = {GPIO_Pin_10, GPIO_Pin_9};
    if(((GPIOA->IDR) & keys[key_id]) == 0)
        return 1;
    else
        return 0;
}

static void key_action(int key_id, zk_action_t action)
{
    /*
    DataConfig_t* pdc = DC_Get();

    if(pdc->status.mode == DC_MODE_NORMAL) {
        if(action == ZK_LONGPRESS) {
            if(key_id == KEY_TOP) {     // entering fast setting mode
//                pdc->status.mode = DC_MODE_FAST_SETTING;
            }
        }
        else if(action == ZK_PRESS) {   // do nothing
        }
    }
    */
}

void KEY_Config(void)
{
    zk_t zk;
    static int count[2];
    static int state[2];
    zk.read_f = key_read;
    zk.action_f = key_action;
    zk.key_num = 2;
    zk.key_count = count;
    zk.key_state = state;
    zk.longpress_count = 300;
    zk_init(&zk);
}
