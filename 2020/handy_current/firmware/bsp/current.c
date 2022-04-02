#include "current.h"
#include "misc.h"
#include "conf.h"

const float s125[] = {0.5, 1.0, 2.0};
const float s123[] = {0.25, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.5, 2.0};
const float sr5[] = {0.25, 0.4, 0.63, 1.0, 1.6};
const float sr10[] = {0.25, 0.32, 0.4, 0.5, 0.63, 0.8, 1.0, 1.3, 1.6, 2.0};

static struct {
    float val;
    int channel;
    const float *p_pref;
    int size_pref;
} g;

float CURRENT_GetVal(void)
{
    return g.val;
}

void CURRENT_SetVal(float val)
{
    g.val = val;
}

void CURRENT_Adjust(int dir)
{
    GlobalConfig_t *conf = GlobalConfigRead();

    switch(conf->current_steps) {
        case CURRENT_PREF_S125:
            g.p_pref = s125;
            g.size_pref = sizeof(s125) / sizeof(s125[0]);
            break;
        case CURRENT_PREF_S123:
            g.p_pref = s123;
            g.size_pref = sizeof(s123) / sizeof(s123[0]);
            break;
        case CURRENT_PREF_R5:
            g.p_pref = sr5;
            g.size_pref = sizeof(sr5) / sizeof(sr5[0]);
            break;
        case CURRENT_PREF_R10:
            g.p_pref = sr10;
            g.size_pref = sizeof(sr10) / sizeof(sr10[0]);
            break;
        default:
            g.p_pref = sr10;
            g.size_pref = sizeof(sr10) / sizeof(sr10[0]);
            break;
    }

    if(dir == CURRENT_DIR_UP) {
        if(g.val >= g.p_pref[g.size_pref - 1]) {

            if(g.channel == 3)
                return;
            else {
                CURRENT_SetChannel(g.channel + 1);
                g.val = g.p_pref[0];
            }
        }
        else {
            for(int i = 0; i < g.size_pref; i++) {
                if(g.p_pref[i] > g.val) {
                    g.val = g.p_pref[i];
                    break;
                }
            }
        }
    }
    else if(dir == CURRENT_DIR_DOWN) {
        if(g.val <= g.p_pref[0]) {
            if(g.channel == 0)
                return;
            else {
                g.val = g.p_pref[g.size_pref - 1];
                CURRENT_SetChannel(g.channel - 1);
            }
        }
        else {
            for(int i = g.size_pref - 1; i >= 0; i--) {
                if(g.p_pref[i] < g.val) {
                    g.val = g.p_pref[i];
                    break;
                }
            }
        }
    }
    DAC_SetDualChannelData(DAC_Align_12b_R, 2048, g.val / 3.30 * 4096.0);
}

void CURRENT_Config(void)
{
    CURRENT_SetVal(0);
    CURRENT_SetChannel(0);
}

int CURRENT_GetChannel(void)
{
    return g.channel;
}

void CURRENT_SetChannel(int channel)
{
    g.channel = channel;
    switch(channel) {
        case 3:
            GPIOA->BRR = GPIO_Pin_0;
            GPIOA->BRR = GPIO_Pin_1;
            break;
        case 2:
            GPIOA->BSRR = GPIO_Pin_0;
            GPIOA->BRR = GPIO_Pin_1;
            break;
        case 1:
            GPIOA->BRR = GPIO_Pin_0;
            GPIOA->BSRR = GPIO_Pin_1;
            break;
        case 0:
            GPIOA->BSRR = GPIO_Pin_0;
            GPIOA->BSRR = GPIO_Pin_1;
            break;
    }
}
