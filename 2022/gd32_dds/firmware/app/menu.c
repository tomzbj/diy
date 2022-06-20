#include "platform.h"
#include "misc.h"

#define MENU_ITEMS 25
#define MENU_ITEMS_PER_PAGE 4
#define SELECT_ITEMS_PER_PAGE 4

typedef struct {
    int level;
    int type;
    int string_id;
    int id;
} menu_create_info_t;

enum {
    MI_FREQ_1, MI_FREQ_2, MI_FREQ_3, MI_FREQ_4, MI_UNIT
};

static const menu_create_info_t menu_create_info[] = {
//
    {ZM_TOPLEVEL, ZM_ITEM_EDIT, STR_FREQ_1, MI_FREQ_1},    //
    {ZM_TOPLEVEL, ZM_ITEM_EDIT, STR_FREQ_2, MI_FREQ_2},    //
    {ZM_TOPLEVEL, ZM_ITEM_EDIT, STR_FREQ_3, MI_FREQ_3},    //
    {ZM_TOPLEVEL, ZM_ITEM_EDIT, STR_FREQ_4, MI_FREQ_4},    //
    {ZM_TOPLEVEL, ZM_ITEM_SELECT, STR_UNIT, MI_UNIT},    //
//
    };

static struct {
    unsigned char menu_buf[512];
    unsigned char pool[512];
    unsigned char* item_name[MENU_ITEMS];
    int item_count;
} g;

enum {
    WAVEFORM_SINE, WAVEFORM_RECTANGLE, WAVEFORM_TRIANGLE, WAVEFORM_SAWTOOTH
};
static void action_f(int action_type, int action_id)
{
    DataConfig_t* pdc = DC_Get();
    switch(action_type) {
        case ZM_ITEM_SELECT:
            pdc->state.disp_state = DC_STATE_SELECT;
            switch(action_id) {
                case MI_UNIT: {
                    static const char* unit_names[] = {"Hz", "kHz", "MHz",
                        "AUTO"};
                    static const int vals[] = {DC_UNIT_HZ, DC_UNIT_KHZ,
                        DC_UNIT_MHZ, DC_UNIT_AUTO};
                    zm_select_set_string(&(pdc->cfg.unit), vals,
                        sizeof(vals[0]), (const char**)unit_names,
                        sizeof(vals) / sizeof(vals[0]));
                }
                    break;
            }
            break;
        case ZM_ITEM_EDIT:
            pdc->state.disp_state = DC_STATE_EDIT;
            switch(action_id) {
                case MI_FREQ_1:
                    zm_edit_set_int(&(pdc->cfg.freq[0]), "FREQ_1/Hz", 0,
                        9999999, 7);
                    break;
                case MI_FREQ_2:
                    zm_edit_set_int(&(pdc->cfg.freq[1]), "FREQ_2/Hz", 0,
                        9999999, 7);
                    break;
                case MI_FREQ_3:
                    zm_edit_set_int(&(pdc->cfg.freq[2]), "FREQ_3/Hz", 0,
                        9999999, 7);
                    break;
                case MI_FREQ_4:
                    zm_edit_set_int(&(pdc->cfg.freq[3]), "FREQ_4/Hz", 0,
                        9999999, 7);
                    break;
            }
            break;
    }
}

static unsigned char* alloc_item(int id)
{
//    DataConfig_t* pdc = DC_Get();
    int lang = STRING_LANG_EN;
    unsigned char* p = STRING_Get(id, lang);
//    printf("%d %s\n", lang, p);
    int len = strlen((char*)p);

    unsigned char* t = bget(len + 2);
    memset(t, '\0', len + 2);
//    strncpy((char*)t, (char*)p, len);
    memcpy(t, p, len);
//    strcpy((char*)t, (char*)p);
    g.item_name[g.item_count] = t;
    g.item_count++;

//    strdup
    return t;
}

static void release_items(void)
{
    for(int i = 0; i < g.item_count; i++) {
        brel(g.item_name[i]);
    }
    g.item_count = 0;
}

static void stat(void)
{
    long curalloc, totfree, maxfree, nget, nrel;
    bstats(&curalloc, &totfree, &maxfree, &nget, &nrel);
    printf("## %ld %ld %ld %ld %ld\n", curalloc, totfree, maxfree, nget, nrel);
}

void zm_create_menu(const menu_create_info_t menu_create_info[], int n_items)
{
    for(int i = 0; i < n_items; i++) {
        const menu_create_info_t* p = &menu_create_info[i];
        zm_add_item(p->level, p->type, alloc_item(p->string_id), p->id);
    }
}

static void ze_discard_f(void)
{
    DC_Get()->state.disp_state = DC_STATE_MENU;
}

static void ze_save_f(void)
{
    DC_Update();
    DC_Get()->state.disp_state = DC_STATE_MENU;
}

void MENU_Config(void)
{
    static int first_run = 1;
    zm_init(g.menu_buf, MENU_ITEMS, MENU_ITEMS_PER_PAGE, action_f);

    if(first_run) {
        bpool(g.pool, sizeof(g.pool));
        first_run = 0;
    }
    release_items();
    int num_of_items = sizeof(menu_create_info) / sizeof(menu_create_info[0]);
    zm_create_menu(menu_create_info, num_of_items);

    zm_select_init(SELECT_ITEMS_PER_PAGE, DC_Update);
    zm_edit_init(ze_save_f, ze_discard_f);
    stat();
    print_log();
}

void MENU_View(void)
{
    UG_FontSetHSpace(0);
    UG_FontSelect(&FONT_8X8);
//    UG_HZ_FontSelect(&HZFONT_11X11);
//    UG_HZ_FontSelect(&HZFONT_11X11, DC_Get()->rom_addr.hzk11);

    for(int i = 0; i < MENU_ITEMS_PER_PAGE; i++) {
        int checked;
//        UG_HZ_PutString(6, i * 11 - 1, (const char*)zm_get_item(i, &checked));
        UG_PutString(8, i * 8, (char*)zm_get_item(i, &checked));
        if(checked)
//            UG_HZ_PutString(0, i * 11 - 1, (unsigned char*)(">"));
            UG_PutString(0, i * 8, (char*)(">"));
    }
}

void MENU_View_Edit(void)
{
    UG_FontSetHSpace(0);
    const UG_FONT* font = &FONT_8X12;
    UG_FontSelect(font);

//    UG_FontSelect(&HZFONT_11X11);
    UG_PutString(0, -1, (char*)zm_edit_get_name());
    UG_PutString(0, 21, " SAVE");
    UG_PutString(120 - font->char_width * 7, 21, "DISCARD");
    const char* item = zm_edit_get_val();

    int xl = (128 - strlen(item) * font->char_width) / 2;
    UG_PutString(xl, 10, (char*)item);

    int cursor_pos = zm_edit_get_cursor_pos();
    if(cursor_pos >= 0) {
        int xr = (128 + strlen(item) * font->char_width) / 2;
        xr = xr - cursor_pos * font->char_width - 1;
        UG_DrawFrame(xr, 10, xr - font->char_width, 22, 1);
    }
    else if(cursor_pos == ZE_POS_SAVE) {
        UG_DrawFrame(0, 21, 50, 31, 1);    // save
    }
    else if(cursor_pos == ZE_POS_DISCARD) {
        UG_DrawFrame(120 - font->char_width * 7, 21, 127, 31, 1);    // discard
    }
    /*
     for(int i = 0; i < MENU_ITEMS_PER_PAGE; i++) {
     int checked;
     UG_PutString(6, i * 11 - 1, (char*)zm_get_item(i, &checked));
     if(checked)
     UG_PutString(0, i * 11 - 1, (char*)(">"));
     }
     */
}

void SELECT_View(void)
{
    UG_FontSetHSpace(0);
    UG_FontSelect(&FONT_8X8);
//    UG_HZ_FontSelect(&HZFONT_11X11);
//    UG_HZ_FontSelect(&HZFONT_11X11, DC_Get()->rom_addr.hzk11);

    for(int i = 0; i < SELECT_ITEMS_PER_PAGE; i++) {
        int current, checked;
        const char* name = (const char*)zm_select_get_item(i, &current,
            &checked);
        char buf[16];
        buf[0] = current ? '>' : ' ';
        printf("%s %d %d\n", name, strlen(name), checked);
        sprintf((char*)(buf + 1), "%s%s", name, checked ? " (*)" : "");
//        UG_HZ_PutString(0, i * 11 - 1, buf);
        UG_PutString(0, i * 8, buf);
    }
}
