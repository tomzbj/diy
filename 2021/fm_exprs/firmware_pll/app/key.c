#include "misc.h"
#include "platform.h"

static int key_read(int key_id)
{
    unsigned short keys[2] = {GPIO_Pin_1, GPIO_Pin_0};
    if((GPIOA->IDR & keys[key_id]) == 0)
        return 1;
    else
        return 0;
}

static void key_action(int key_id, zk_action_t action)
{
    DataConfig_t* pdc = DC_Get();
    if(action == ZK_PRESS) {
        switch(key_id) {
            case 0:
                if(pdc->data.freq < 108000UL)
                    pdc->data.freq += 50;
                break;
            case 1:
                if(pdc->data.freq > 88000UL)
                    pdc->data.freq -= 50;
                break;
        }
    }
    else if(action == ZK_LONGPRESS) {
        switch(key_id) {
            case 0:
                if(pdc->data.freq < 108000UL)
                    pdc->data.freq += 1000;
                break;
            case 1:
                if(pdc->data.freq > 88000UL)
                    pdc->data.freq -= 1000;
                break;
        }
    }
    DISP_Update();
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
