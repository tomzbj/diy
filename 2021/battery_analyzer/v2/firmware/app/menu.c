#include "platform.h"

enum {
    MI_DIS_CURR, MI_END_VOLT
};
static void action(int action_id)
{
    DataConfig_t* pdc = DC_Get();
    pdc->status.disp_state = DC_DISP_STATE_SELECT;

    switch(action_id) {
        case MI_DIS_CURR: {
            static signed short dis_curr_vals[] =    //
                {10, 100, 200, 300, 500, 750, 1000};
            zs_set_integer(&(pdc->cfg.discharge_curr), dis_curr_vals,
                sizeof(dis_curr_vals) / sizeof(dis_curr_vals[0]), "mA");
            break;
        }
        case MI_END_VOLT: {
            static signed short term_volt_vals[] = {900, 1000, 2400, 2700, 3000};
            static const char* term_volt_names[] = {"0.9V Zn-C/Alkl.",
                "1.0V NiMH/NiCd", "2.4V LiFePO4", "2.7V Li-ION", "3.0V Li-ION"};
            zs_set_string(&(pdc->cfg.end_volt), term_volt_vals, term_volt_names,
                sizeof(term_volt_vals) / sizeof(term_volt_vals[0]));
            break;
        }
    }
}

void MENU_Config(void)
{
#define MENU_ITEMS 10
#define MENU_ITEMS_PER_PAGE 3
#define SELECT_ITEMS_PER_PAGE 3

    static unsigned char menu_buf[MENU_ITEMS * sizeof(zm_item_t)];
    zm_init(menu_buf, MENU_ITEMS, MENU_ITEMS_PER_PAGE, action);

    zm_add_item(ZM_TOPLEVEL, 0, "Discharge Current", MI_DIS_CURR);
    zm_add_item(ZM_TOPLEVEL, 0, "Ending Voltage", MI_END_VOLT);

    zs_init(SELECT_ITEMS_PER_PAGE, DC_Update);
}

void MENU_View(void)
{
    UG_FontSelect(&FONT_8X12);
    UG_FontSetHSpace(0);
    for(int i = 0; i < MENU_ITEMS_PER_PAGE; i++) {
        int checked;
        const char* name = zm_get_item(i, &checked);
        UG_PutString(8, i * 11 - 1, (char*)name);
        if(checked)
            UG_PutString(0, i * 11 - 1, ">");
    }
}

void SELECT_View(void)
{
    UG_FontSelect(&FONT_6X12);
    UG_FontSetHSpace(1);
    for(int i = 0; i < SELECT_ITEMS_PER_PAGE; i++) {
        int current, checked;
        const char* name = zs_get_item(i, &current, &checked);
        char buf[16];
        buf[0] = current ? '>' : ' ';
        sprintf(buf + 1, "%s%s", name, checked ? " *" : "");
        UG_PutString(0, i * 11 - 1, buf);
    }
}
