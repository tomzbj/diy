#include "misc.h"
#include "platform.h"

enum {
    KEY_TOP = 0, KEY_BOTTOM = 1
};

#define NUM_STEPS 3
static struct {
    float steps[NUM_STEPS];
    int curr_step;
} g;

static int key_read(int key_id)
{
    unsigned short keys[2] = {GPIO_Pin_0, GPIO_Pin_1};
    if(((GPIOB->IDR) & keys[key_id]) == 0)
        return 1;
    else
        return 0;
}

static void key_action(int key_id, zk_action_t action)
{
    DataConfig_t* pdc = DC_Get();
    if(action == ZK_LONGPRESS) {
        if(key_id == KEY_TOP) {
            g.curr_step++;
            g.curr_step %= NUM_STEPS;
            pdc->status.t_step = g.steps[g.curr_step];
        }
    }
    else if(action == ZK_PRESS) {
        if(key_id == KEY_TOP) {
            pdc->status.t_sp += g.steps[g.curr_step];
            if(pdc->status.t_sp >= 80.0)
                pdc->status.t_sp = 80.0;
        }
        else if(key_id == KEY_BOTTOM) {
            pdc->status.t_sp -= g.steps[g.curr_step];
            if(pdc->status.t_sp <= 30.0)
                pdc->status.t_sp = 30.0;
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

    memcpy(g.steps, (typeof(g.steps)) {10, 5, 1}, sizeof(g.steps));
    g.curr_step = 1;
    DataConfig_t* pdc = DC_Get();
    pdc->status.t_step = g.steps[g.curr_step];
}
