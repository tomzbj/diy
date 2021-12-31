#include "misc.h"
#include "platform.h"

static int key_read(int key_id)
{
    if((PINB & (1 << PB4)) == 0)
        return 1;
    else
        return 0;
}

static void key_action(int key_id, zk_action_t action)
{
    DataConfig_t* pdc = DC_Get();
    if(action == ZK_LONGPRESS) {
        if(pdc->cfg.att > 0)
            pdc->cfg.att -= 3;
        else
            pdc->cfg.att = 45;
    }
    if(action == ZK_PRESS) {
        if(pdc->cfg.att < 45)
            pdc->cfg.att += 3;
        else
            pdc->cfg.att = 0;
    }
}

void KEY_Config(void)
{
    DDRB &= ~_BV(PB4);
    PORTB |= _BV(PB4);
    zk_t zk;
    static int count[1];
    static int state[1];
    zk.read_f = key_read;
    zk.action_f = key_action;
    zk.key_num = 1;
    zk.key_count = count;
    zk.key_state = state;
    zk.longpress_count = 20;

    zk_init(&zk);
    printf("[%s: %d] KEY Initialized.\n", __FILE__, __LINE__);
}
