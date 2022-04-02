#include "zkey.h"

enum {
    STATE_NONE, STATE_PRESSED, STATE_LONGPRESSED
};

static struct {
    zk_t zk;
    unsigned short trig, cont;
} g;

void zk_init(zk_t* zk)
{
    g.zk = *zk;
    if(g.zk.key_num > 16)
        g.zk.key_num = 16;
}

#include "misc.h"
void zk_poll(void)
{
    int i;
    unsigned short key = 0;
    for(i = 0; i < g.zk.key_num; i++) {
        if(g.zk.read_f(i))
            key |= (1 << i);
    }
    g.trig = key & (key ^ g.cont);
    g.cont = key;

    for(i = 0; i < g.zk.key_num; i++) {
        if(g.zk.read_f(i)) {
            if(g.trig & (1 << i)) {
                g.zk.action_f(i, ZK_CLICK);
                g.zk.key_state[i] = STATE_PRESSED;
                g.zk.key_count[i] = 0;
            }
            if(g.cont & (1 << i)) {
                g.zk.key_count[i]++;
                if(g.zk.key_count[i] > g.zk.longpress_count) {
                    g.zk.key_state[i] = STATE_LONGPRESSED;
                }
            }
        }
        else {
            if(g.zk.key_state[i] == STATE_PRESSED) {
                g.zk.action_f(i, ZK_PRESS);
                g.zk.key_state[i] = STATE_NONE;
                g.zk.key_count[i] = 0;
            }
            if(g.zk.key_state[i] == STATE_LONGPRESSED) {
                g.zk.action_f(i, ZK_LONGPRESS);
                g.zk.key_state[i] = STATE_NONE;
                g.zk.key_count[i] = 0;
            }
        }
    }
}
