#include "platform.h"
#include "math.h"
#include <string.h>
#include <stdbool.h>

#define LABEL_NUM 10
#define LABEL_TEXT_LEN 10

typedef signed short s16_t;

typedef struct {
    const s16_t x, y, w, h, xgrids, ygrids;
    UG_COLOR c_bg, c_grid, c_fg;
    s16_t lx, ly, lx2, ly2;
} trace_t;

typedef struct {
    const char* name;
    char text[LABEL_TEXT_LEN + 2];
    s16_t x, y, w, h;
//    bool updated;
} label_t;

static struct {
    const s16_t w, h;
    label_t labels[LABEL_NUM];
    trace_t trace;
    UG_COLOR c_label_bg, c_active_label_bg;
    unsigned char active_label;
} g = {
//
    .w = 320,
    .h = 240,    //
    .trace = {    //
        .x = 0, .y = 32, .w = 250, .h = 176, .xgrids = 10, .ygrids = 8,    // grid size: 20x22
            .c_bg = C_BLACK,
            .c_grid = C_DIM_GRAY    //
        },
    .labels = {    //
        [0] = {.name = "MODE", .text = "AUTO", .x = 0, .y = 0},    //
            [1] = {.name = "TR.LV.", .text = "0.5V", .x = 80, .y = 0},    //
            [2] = {.name = "TR.EDGE", .text = "EDGE", .x = 160, .y = 0},    //
            [3] = {.name = "COUP.", .text = "DC", .x = 0, .y = 208},    //
            [4] = {.name = "SEN.", .text = "1.0V", .x = 80, .y = 208},    //
            [5] = {.name = "T.BASE", .text = "50us", .x = 160, .y = 208}    //
        },    //
    .c_label_bg = C_BLACK,
    .c_active_label_bg = C_DARK_RED    //
    };

static void moveto(s16_t x, s16_t y, s16_t y2)
{
    g.trace.lx = x;
    g.trace.ly = y;
    g.trace.lx2 = x;
    g.trace.ly2 = y2;
}

static void lineto(s16_t x, s16_t y)
{
    UG_DrawLine(g.trace.lx, g.trace.ly, x, y, g.trace.c_fg);
}

static UG_COLOR get_grid_color(s16_t x, s16_t y)
{
    x -= g.trace.x;
    y -= g.trace.y;

    if((x) % (g.trace.w / g.trace.xgrids) == 0)
        return g.trace.c_grid;
    else if((y) % (g.trace.h / g.trace.ygrids) == 0)
        return g.trace.c_grid;
    else if(x == 1 || x == g.trace.w - 1 || x == g.trace.w - 2)
        return g.trace.c_grid;
    else if(x == g.trace.w / 2 - 1 || x == g.trace.w / 2 + 1)
        return g.trace.c_grid;
    else if(y == 1 || y == g.trace.h - 1 || y == g.trace.h - 2)
        return g.trace.c_grid;
    else if(y == g.trace.h / 2 - 1 || y == g.trace.h / 2 + 1)
        return g.trace.c_grid;
    return g.trace.c_bg;
}

static void draw_grid(void)
{
    UG_FillFrame(g.trace.x, g.trace.y, g.trace.x + g.trace.w,
        g.trace.y + g.trace.h, g.trace.c_bg);
    for(int y = 0; y < g.trace.h; y++) {
        for(int x = 0; x < g.trace.w; x++) {
            UG_COLOR c = get_grid_color(x + g.trace.x, y + g.trace.y);
            if(c != C_BLACK)
                UG_DrawPixel(x + g.trace.x, y + g.trace.y, c);
        }
    }
}

static void erase_oldline(s16_t x1, s16_t y1, s16_t x2, s16_t y2)
{
    for(int x = x1; x <= x2; x++) {
        if(y1 < y2) {
            for(int y = y1; y <= y2; y++) {
                UG_DrawPixel(x, y, get_grid_color(x, y));
            }
        }
        else {
            for(int y = y2; y <= y1; y++) {
                UG_DrawPixel(x, y, get_grid_color(x, y));
            }
        }
    }
}

void lineto_erase(s16_t x, s16_t y, s16_t oldy)
{
    erase_oldline(g.trace.lx, g.trace.ly2, x, oldy);
    UG_DrawLine(g.trace.lx, g.trace.ly, x, y, g.trace.c_fg);
    moveto(x, y, oldy);
}

static int ypos_correct(int y)
{
    const int k1 = 88, k2 = 92, b = 0;
    y = y * k1;
    y = y / k2 + b;
    y = g.trace.y + g.trace.h - y;
    if(y < g.trace.y)
        return g.trace.y;
    else if(y >= g.trace.y + g.trace.h)
        return g.trace.y + g.trace.h - 1;
    else
        return y;
}

static void draw_trigger_line(unsigned short trig_level)
{
    static unsigned short old_trig_y;
    unsigned short trig_y = trig_level * 255UL / 3300;
    trig_y = ypos_correct(trig_y);
    erase_oldline(g.trace.x, old_trig_y, g.trace.x + g.trace.w, old_trig_y);
    UG_DrawLine(g.trace.x, trig_y, g.trace.x + g.trace.w, trig_y, C_DARK_RED);
    old_trig_y = trig_y;
}

static int cross_zero(int x1, int x2, int y)
{
    DataConfig_t* pdc = DC_Get();
    if(((int)pdc->data.buf[x1] - y) * ((int)pdc->data.buf[x2] - y) <= 0) {
        if(pdc->data.buf[x1] < pdc->data.buf[x2]) {
            return 1;    // rising
        }
        if(pdc->data.buf[x1] > pdc->data.buf[x2]) {
            return -1;    // falling
        }
    }
    return 0;
}

static unsigned short get_cross_pos(void)
{
    DataConfig_t* pdc = DC_Get();
    if(pdc->cfg.trig_mode == DC_TRIG_MODE_NORMAL) {
        for(int i = 0; i < DC_ADC_BUF_SIZE / 2; i++) {
            int x1 = DC_ADC_BUF_SIZE / 2 + i;
            int x2 = DC_ADC_BUF_SIZE / 2 - i;
            int trig_y = pdc->cfg.trig_level * 255UL / 3300;

            switch(pdc->cfg.trig_edge) {
                case DC_TRIG_EDGE_BOTH:
                    if(cross_zero(x1 - 1, x1, trig_y))
                        return x1;
                    if(cross_zero(x2, x2 + 1, trig_y))
                        return x2;
                    break;
                case DC_TRIG_EDGE_RISING:
                    if(cross_zero(x1 - 1, x1, trig_y) > 0)
                        return x1;
                    if(cross_zero(x2, x2 + 1, trig_y) > 0)
                        return x2;
                    break;
                case DC_TRIG_EDGE_FALLING:
                    if(cross_zero(x1 - 1, x1, trig_y) < 0)
                        return x1;
                    if(cross_zero(x2, x2 + 1, trig_y) < 0)
                        return x2;
                    break;
            }
        }
    }
    return DC_ADC_BUF_SIZE / 2;
}

void UI_DrawTrace(void)
{
    static float init_phase = 0.0;
    init_phase += 0.1;
    int firstdot = 1;
    static s16_t old_buf[280], new_buf[280];

    while(DMA1_Channel1->CNDTR > 10);
    DMA_Cmd(DMA1_Channel1, DISABLE);

    DataConfig_t* pdc = DC_Get();
    if(pdc->cfg.trig_mode == DC_TRIG_MODE_NORMAL
        || pdc->cfg.trig_mode == DC_TRIG_MODE_SINGLE)
        draw_trigger_line(pdc->cfg.trig_level);
    int cross_pos = get_cross_pos();
    int window_pos = cross_pos - g.trace.w / 2;
    if(window_pos < 0)
        window_pos += DC_ADC_BUF_SIZE;
    for(int i = 0; i < g.trace.w; i++) {
        new_buf[i] = ypos_correct(pdc->data.buf[i + window_pos]);
    }

    for(int i = g.trace.x; i < g.trace.x + g.trace.w; i++) {
        if(firstdot) {
            moveto(i, new_buf[i], old_buf[i]);
            firstdot = 0;
        }
        else {
            lineto_erase(i, new_buf[i], old_buf[i]);
        }
    }
    memcpy(old_buf, new_buf, sizeof(new_buf));
    DMA_Cmd(DMA1_Channel1, ENABLE);
}

void UI_UpdateLabels(void)
{
    const UG_FONT* pfont = &FONT_12X16;
    label_t* pl;

    UG_FontSetHSpace(0);
    UG_FontSetVSpace(0);
    UG_FontSelect(pfont);
    DataConfig_t* pdc = DC_Get();
    for(int i = 0; g.labels[i].name; i++) {
        pl = &(g.labels[i]);
        UG_COLOR c;
        if(pdc->status.mode == DC_MODE_NORMAL)
            c = g.c_label_bg;
        else {
            if(g.active_label == i)
                c = g.c_active_label_bg;
            else
                c = g.c_label_bg;
        }
        UG_SetBackcolor(c);
        UG_FillFrame(pl->x, pl->y, pl->x + pl->w - 1, pl->y + pl->h - 1, c);
        UG_SetForecolor(C_WHITE);
        UG_PutString(pl->x, pl->y, (char*)pl->name);
        UG_SetForecolor(C_GREEN);
        UG_PutString(pl->x, pl->y + pfont->char_height, pl->text);
        UG_SetBackcolor(C_BLACK);
    }
}

void UI_SetLabelText(int id, const char* str)
{
    strncpy(g.labels[id].text, str, LABEL_TEXT_LEN);
    UI_UpdateLabels();
//    g.labels[id].updated = true;
}

void UI_Init(void)
{
//    g.menu_height = 22;
    g.trace.c_fg = C_YELLOW;
    for(int i = 0; g.labels[i].name; i++) {
//        g.labels[i].updated = true;
        g.labels[i].w = 80;
        g.labels[i].h = 32;
    }
    UG_FillScreen(C_BLACK);
    UI_UpdateLabels();
    draw_grid();
}

void UI_NextLabel(void)
{
    if(g.active_label != UI_LABEL_LAST) {
        g.active_label++;
    }
    else {
        g.active_label = UI_LABEL_FIRST;
    }
}

int UI_GetActiveLabel(void)
{
    return g.active_label;
}
