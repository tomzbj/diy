#include "misc.h"
#include "platform.h"

static int key_read(int key_id)
{
    unsigned short keys[2] = {GPIO_Pin_11, GPIO_Pin_12};
    if(((GPIOA->IDR) & keys[key_id]) == 0)
        return 1;
    else
        return 0;
}

static void key_action(int key_id, zk_action_t action)
{
    DataConfig_t* pdc = DC_Get();
    int state = pdc->status.disp_state;
    if(state == DC_DISP_STATE_MENU) {
        if(action == ZK_LONGPRESS) {
            switch(key_id) {
                case 0:
                    if(zm_istoplevel())
                        pdc->status.disp_state = DC_DISP_STATE_NORMAL;
                    else
                        zm_op(ZM_OP_BACK);
                    break;
                case 1:
                    zm_op(ZM_OP_RUN);
                    break;
            }
        }
        if(action == ZK_PRESS) {
            switch(key_id) {
                case 0:
                    zm_op(ZM_OP_UP);
                    break;
                case 1:
                    zm_op(ZM_OP_DOWN);
                    break;
            }
        }
    }
    if(state == DC_DISP_STATE_SELECT) {
        if(action == ZK_LONGPRESS) {
            switch(key_id) {
                case 0:
                    pdc->status.disp_state = DC_DISP_STATE_MENU;
                    break;
                case 1:
                    zs_op(ZS_OP_SAVE);
                    break;
            }
        }
        if(action == ZK_PRESS) {
            switch(key_id) {
                case 0:
                    zs_op(ZS_OP_DEC);
                    break;
                case 1:
                    zs_op(ZS_OP_INC);
                    break;
            }
        }
    }
    else if(state == DC_DISP_STATE_NORMAL) {
        if(action == ZK_PRESS) {
            switch(key_id) {
                case 0: {
                    pdc->status.discharge_state = !pdc->status.discharge_state;
                    break;
                }
            }
        }
        if(action == ZK_LONGPRESS) {
            switch(key_id) {
                case 1:
                    if(state == DC_DISP_STATE_NORMAL)
                        pdc->status.disp_state = DC_DISP_STATE_MENU;
                    break;
                case 0:
                    pdc->data.total_capacity = 0;
                    break;
            }
        }
    }
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
