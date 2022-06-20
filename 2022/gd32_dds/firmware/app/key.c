#include "misc.h"
#include "platform.h"

enum {
    KEY_TOP = 0, KEY_BOTTOM = 1
};
static int key_read(int key_id)
{
    unsigned short keys[2] = {GPIO_PIN_1, GPIO_PIN_0};
    if((GPIO_ISTAT(GPIOB) & keys[key_id]) == 0)
        return 1;
    else
        return 0;
}

static void key_action(int key_id, zk_action_t action)
{
    DataConfig_t* pdc = DC_Get();
    if(pdc->state.disp_state == DC_STATE_MENU) {
        if(action == ZK_LONGPRESS) {
            switch(key_id) {
                case KEY_TOP:
                    if(zm_istoplevel()) {
                        pdc->state.disp_state = DC_STATE_NORMAL;
                        DDS_Start();
                        break;
                    }
                    else
                        zm_op(ZM_OP_BACK);
                    break;
                case KEY_BOTTOM:
                    zm_op(ZM_OP_RUN);
                    break;
            }
        }
        if(action == ZK_PRESS) {
            switch(key_id) {
                case KEY_TOP:
                    zm_op(ZM_OP_UP);
                    break;
                case KEY_BOTTOM:
                    zm_op(ZM_OP_DOWN);
                    break;
            }
        }
    }
    else if(pdc->state.disp_state == DC_STATE_EDIT) {
        if(action == ZK_LONGPRESS) {
            switch(key_id) {
                case KEY_BOTTOM:
                    zm_edit_op(ZE_OP_INC);
                    break;
                case KEY_TOP:
                    zm_edit_op(ZE_OP_DEC);
                    break;
            }
        }
        if(action == ZK_PRESS) {
            switch(key_id) {
                case KEY_TOP:
                    zm_edit_op(ZE_OP_LEFT);
                    break;
                case KEY_BOTTOM:
                    zm_edit_op(ZE_OP_RIGHT);
                    break;
            }
        }
    }
    else if(pdc->state.disp_state == DC_STATE_SELECT) {
        if(action == ZK_LONGPRESS) {
            switch(key_id) {
                case KEY_TOP:
                    pdc->state.disp_state = DC_STATE_MENU;
                    break;
                case KEY_BOTTOM:
                    zm_select_op(ZS_OP_SAVE);
                    break;
            }
        }
        if(action == ZK_PRESS) {
            switch(key_id) {
                case KEY_TOP:
                    zm_select_op(ZS_OP_DEC);
                    break;
                case KEY_BOTTOM:
                    zm_select_op(ZS_OP_INC);
                    break;
            }
        }
    }
    else if(pdc->state.disp_state == DC_STATE_NORMAL) {
        DDS_Stop();
        if(action == ZK_LONGPRESS) {
            switch(key_id) {
                case KEY_BOTTOM:
                    pdc->state.disp_state = DC_STATE_MENU;
                    break;
                case KEY_TOP:
                    // 切换波形
                    pdc->state.waveform++;
                    if(pdc->state.waveform > DC_WAVEFORM_MAX)
                        pdc->state.waveform = 0;
                    DDS_Start();
                    break;
            }
        }
        if(action == ZK_PRESS) {
            switch(key_id) {
                case KEY_TOP:
                    // 切换输出幅度
                    pdc->state.ampl++;
                    if(pdc->state.ampl > DC_AMPL_MAX)
                        pdc->state.ampl = 0;
                    DDS_Start();
                    break;
                case KEY_BOTTOM:
                    // 切换预设频率
                    pdc->state.curr_preset++;
                    pdc->state.curr_preset %= 4;
                    DDS_Start();
                    break;
            }
            DDS_SetFreq(pdc->cfg.freq[pdc->state.curr_preset]);
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
    print_log();
}
