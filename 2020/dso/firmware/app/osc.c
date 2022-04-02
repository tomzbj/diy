#include "platform.h"

const int sens[] = {1, 2, 5, 10, 20, 50, 100, 200, 500};

void OSC_ParamsUp(void)
{
    int label = UI_GetActiveLabel();
    DataConfig_t* pdc = DC_Get();
    switch(label) {
        case UI_LABEL_TRIG_MODE:
            if(pdc->cfg.trig_mode == DC_TRIG_MODE_MAX)
                pdc->cfg.trig_mode = 0;
            else
                pdc->cfg.trig_mode++;
            break;
        case UI_LABEL_TRIG_LEVEL:
            if(pdc->cfg.trig_level < 3300)
                pdc->cfg.trig_level += 100;
            break;
        case UI_LABEL_TRIG_EDGE:
            if(pdc->cfg.trig_edge == DC_TRIG_EDGE_MAX)
                pdc->cfg.trig_edge = 0;
            else
                pdc->cfg.trig_edge++;
            break;
        case UI_LABEL_COUPLING:
            if(pdc->cfg.coupling == DC_COUPLING_MAX)
                pdc->cfg.coupling = 0;
            else
                pdc->cfg.coupling++;
            break;
        case UI_LABEL_SEN:
            if(pdc->cfg.sen != DC_SEN_MAX)
                pdc->cfg.sen++;
            break;
        case UI_LABEL_TIMEBASE:
            if(pdc->cfg.timebase < DC_TIMEBASE_MAX)
                pdc->cfg.timebase++;
            break;
    }
    DC_Update();
}

void OSC_ParamsDown(void)
{
    int label = UI_GetActiveLabel();
    DataConfig_t* pdc = DC_Get();
    switch(label) {
        case UI_LABEL_TRIG_MODE:
            if(pdc->cfg.trig_mode == 0)
                pdc->cfg.trig_mode = DC_TRIG_MODE_MAX;
            else
                pdc->cfg.trig_mode--;
            break;
        case UI_LABEL_TRIG_LEVEL:
            if(pdc->cfg.trig_level >= 100)
                pdc->cfg.trig_level -= 100;
            break;
        case UI_LABEL_TRIG_EDGE:
            if(pdc->cfg.trig_edge == 0)
                pdc->cfg.trig_edge = DC_TRIG_EDGE_MAX;
            else
                pdc->cfg.trig_edge--;
            break;
        case UI_LABEL_COUPLING:
            if(pdc->cfg.coupling == 0)
                pdc->cfg.coupling = DC_COUPLING_MAX;
            else
                pdc->cfg.coupling--;
            break;
        case UI_LABEL_SEN:
            if(pdc->cfg.sen != 0)
                pdc->cfg.sen--;
            break;
        case UI_LABEL_TIMEBASE:
            if(pdc->cfg.timebase != 0)
                pdc->cfg.timebase--;
            break;
    }
    DC_Update();
}

static void apply_mix_settings(void)
{
    unsigned char hc595_state = 0;
    DataConfig_t* pdc = DC_Get();

    if(pdc->cfg.coupling == DC_COUPLING_DC)
        ;    // do nothing
    else if(pdc->cfg.coupling == DC_COUPLING_AC)
        hc595_state |= 0x40;
    else if(pdc->cfg.coupling == DC_COUPLING_GND)
        hc595_state |= 0x80;

    int sen2 = sens[pdc->cfg.sen];
    int sen1 = 1;
    while(sen2 >= 10) {
        sen2 /= 10;
        sen1 *= 10;
    }
    if(sen1 == 100)
        hc595_state |= 0x20;
    else if(sen1 == 10)
        hc595_state |= 0x10;
    else if(sen1 == 1)
        ;
    if(sen2 == 5)
        hc595_state |= 0x08;
    if(sen2 == 2)
        hc595_state |= 0x04;
    if(sen2 == 1)
        ;
    HC595_WriteByte(hc595_state);
}

static void apply_timebase_settings(void)
{
    DataConfig_t* pdc = DC_Get();
    const unsigned short arrs[] = {
        8,
        16,
        40,
        80,
        160,
        400,
        800,
        1600,
        4000,
        8000,
        16000,
        40000};
    if(pdc->cfg.timebase > DC_TIMEBASE_MAX)
        pdc->cfg.timebase = DC_TIMEBASE_MAX;
    TIM_SetAutoreload(TIM6, arrs[pdc->cfg.timebase] - 1);
}

void OSC_UpdateParams(void)
{
    DataConfig_t* pdc = DC_Get();
    static typeof(pdc->cfg) old_cfg;
    char buf[16];

    if(old_cfg.coupling != pdc->cfg.coupling) {
        const char* str_coupling[] = {"DC", "AC", "GND"};
        if(pdc->cfg.coupling > DC_COUPLING_MAX)
            pdc->cfg.coupling = DC_COUPLING_MAX;
        UI_SetLabelText(UI_LABEL_COUPLING, str_coupling[pdc->cfg.coupling]);
        apply_mix_settings();
    }
    if(old_cfg.sen != pdc->cfg.sen) {
        const int sen[] = {1, 2, 5, 10, 20, 50, 100, 200, 500};
        sprintf(buf, "%.2fV", sen[pdc->cfg.sen] / 20.0);
        UI_SetLabelText(UI_LABEL_SEN, buf);
        apply_mix_settings();
    }
    if(old_cfg.timebase != pdc->cfg.timebase) {
        const char* timebases[] = {
            "5us",
            "10us",
            "25us",
            "50us",
            "100us",
            "250us",
            "500us",
            "1ms",
            "2.5ms",
            "5ms",
            "10ms",
            "25ms"};
        if(pdc->cfg.timebase > DC_TIMEBASE_MAX)
            pdc->cfg.timebase = DC_TIMEBASE_MAX;
        UI_SetLabelText(UI_LABEL_TIMEBASE, timebases[pdc->cfg.timebase]);
        apply_timebase_settings();
    }
    if(old_cfg.trig_level != pdc->cfg.trig_level) {
        if(pdc->cfg.trig_level > 3300)
            pdc->cfg.trig_level = 3300;
        if(pdc->cfg.trig_level < 0)
            pdc->cfg.trig_level = 0;
        sprintf(buf, "%.1fV", pdc->cfg.trig_level / 1000.0);
        UI_SetLabelText(UI_LABEL_TRIG_LEVEL, buf);
    }
    if(old_cfg.trig_mode != pdc->cfg.trig_mode) {
        const char* str_trig_mode[] = {"AUTO", "NORMAL", "SINGLE"};
        if(pdc->cfg.trig_mode > DC_TRIG_MODE_MAX)
            pdc->cfg.trig_mode = DC_TRIG_MODE_MAX;
        UI_SetLabelText(UI_LABEL_TRIG_MODE, str_trig_mode[pdc->cfg.trig_mode]);
    }
    if(old_cfg.trig_edge != pdc->cfg.trig_edge) {
        const char* str_trig_edge[] = {"FALLING", "RISING", "BOTH"};
        if(pdc->cfg.trig_edge > DC_TRIG_EDGE_MAX)
            pdc->cfg.trig_edge = DC_TRIG_EDGE_MAX;
        UI_SetLabelText(UI_LABEL_TRIG_EDGE, str_trig_edge[pdc->cfg.trig_edge]);
    }
    old_cfg = pdc->cfg;
}
