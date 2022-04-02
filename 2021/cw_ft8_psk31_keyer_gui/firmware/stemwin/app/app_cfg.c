#include "platform.h"
#include "DIALOG.h"
#define ID_WINDOW_0         (GUI_ID_USER + 0x00)
#define ID_BT_OK            (GUI_ID_USER + 0x01)
#define ID_BT_CANC          (GUI_ID_USER + 0x03)
#define ID_T1               (GUI_ID_USER + 0x04)
#define ID_T2               (GUI_ID_USER + 0x05)
#define ID_T3               (GUI_ID_USER + 0x06)
#define ID_T4               (GUI_ID_USER + 0x07)
#define ID_T5               (GUI_ID_USER + 0x08)
#define ID_T6               (GUI_ID_USER + 0x09)
#define ID_RA1              (GUI_ID_USER + 0x0a)
#define ID_RA2              (GUI_ID_USER + 0x0b)
#define ID_SL1              (GUI_ID_USER + 0x0c)
#define ID_BT_T1N           (GUI_ID_USER + 0x20)
#define ID_BT_T2N           (GUI_ID_USER + 0x21)
#define ID_BT_T3N           (GUI_ID_USER + 0x22)
#define ID_BT_T4N           (GUI_ID_USER + 0x23)
#define ID_BT_T5N           (GUI_ID_USER + 0x24)
#define ID_BT_T1P           (GUI_ID_USER + 0x25)
#define ID_BT_T2P           (GUI_ID_USER + 0x26)
#define ID_BT_T3P           (GUI_ID_USER + 0x27)
#define ID_BT_T4P           (GUI_ID_USER + 0x28)
#define ID_BT_T5P           (GUI_ID_USER + 0x29)
#define ID_VAL1             (GUI_ID_USER + 0x30)
#define ID_VAL2             (GUI_ID_USER + 0x31)
#define ID_VAL3             (GUI_ID_USER + 0x32)
#define ID_VAL4             (GUI_ID_USER + 0x33)
#define ID_VAL5             (GUI_ID_USER + 0x34)

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
//
    {WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 320, 240, 0, 0x0, 0},    //

    {BUTTON_CreateIndirect, "Ok", ID_BT_OK, 30, 210, 100, 30, 0, 0x0, 0},    //
    {BUTTON_CreateIndirect, "Cancel", ID_BT_CANC, 180, 210, 100, 30, 0, 0x0, 0},    //

    {TEXT_CreateIndirect, "CW WPM", ID_T1, 5, 10, 150, 30, 0, 0x0, 0},    //
    {TEXT_CreateIndirect, "CW AUD. FREQ.", ID_T2, 5, 42, 150, 30, 0, 0x0, 0},    //
    {TEXT_CreateIndirect, "FT8 AUD. FREQ.", ID_T3, 5, 74, 150, 30, 0, 0x0, 0},    //
    {TEXT_CreateIndirect, "ECHO VOL.", ID_T4, 5, 106, 150, 30, 0, 0x0, 0},    //
    {TEXT_CreateIndirect, "OUTPUT VOL.", ID_T5, 5, 138, 150, 30, 0, 0x0, 0},    //
    {TEXT_CreateIndirect, "BRIGHTNESS", ID_T6, 5, 170, 150, 30, 0, 0x0, 0},    //

    {TEXT_CreateIndirect, "40", ID_VAL1, 190, 10, 80, 30, 0, 0x0, 0},    //
    {TEXT_CreateIndirect, "500", ID_VAL2, 190, 42, 80, 30, 0, 0x0, 0},    //
    {TEXT_CreateIndirect, "500", ID_VAL3, 190, 74, 80, 30, 0, 0x0, 0},    //
    {TEXT_CreateIndirect, "5", ID_VAL4, 190, 106, 80, 30, 0, 0x0, 0},    //
    {TEXT_CreateIndirect, "5", ID_VAL5, 190, 138, 80, 30, 0, 0x0, 0},    //

    {BUTTON_CreateIndirect, "-", ID_BT_T1N, 150, 10, 30, 30, 0, 0x0, 0},    //
    {BUTTON_CreateIndirect, "-", ID_BT_T2N, 150, 42, 30, 30, 0, 0x0, 0},    //
    {BUTTON_CreateIndirect, "-", ID_BT_T3N, 150, 74, 30, 30, 0, 0x0, 0},    //
    {BUTTON_CreateIndirect, "-", ID_BT_T4N, 150, 106, 30, 30, 0, 0x0, 0},    //
    {BUTTON_CreateIndirect, "-", ID_BT_T5N, 150, 138, 30, 30, 0, 0x0, 0},    //

    {BUTTON_CreateIndirect, "+", ID_BT_T1P, 280, 10, 30, 30, 0, 0x0, 0},    //
    {BUTTON_CreateIndirect, "+", ID_BT_T2P, 280, 42, 30, 30, 0, 0x0, 0},    //
    {BUTTON_CreateIndirect, "+", ID_BT_T3P, 280, 74, 30, 30, 0, 0x0, 0},    //
    {BUTTON_CreateIndirect, "+", ID_BT_T4P, 280, 106, 30, 30, 0, 0x0, 0},    //
    {BUTTON_CreateIndirect, "+", ID_BT_T5P, 280, 138, 30, 30, 0, 0x0, 0},    //

    {SLIDER_CreateIndirect, "SL1", ID_SL1, 150, 170, 160, 30, 0, 0x0, 0}    //
};

static const int cw_wpm_opt[] = {10, 15, 20, 25, 30, 40, 50, 60, 80, 100, 120};
static const int cw_carrier_freq_opt[] = {200, 300, 400, 500, 600, 800, 1000};
static const int ft8_carrier_freq_opt[] = {200, 300, 400, 500, 600, 800, 1000};
static const int volume_opt[] = {1, 2, 3, 4, 5, 6, 7, 8};

enum {
    PREV, NEXT
};
static int alter(const int arr[], int n, int val, int action)
{
    int min = arr[0], max = arr[n - 1], i = 0;
    if(val < min)
        return min;
    if(val > max)
        return max;
    if(action == PREV) {
        for(i = n - 2; i >= 0; i--) {
            if(arr[i] < val)
                return arr[i];
        }
        i = 0;
    }
    else if(action == NEXT) {
        for(i = 0; i < n - 1; i++) {
            if(arr[i] > val)
                return arr[i];
        }
        i = n - 1;
    }
    return arr[i];
}

static void restore_default_val(WM_MESSAGE* pMsg)
{
    char buf[16];
    DataConfig_t* pdc = DC_Get();
    sprintf(buf, "%d", pdc->cfg.cw_wpm);
    TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_VAL1), buf);
    sprintf(buf, "%d", pdc->cfg.cw_carrier_freq);
    TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_VAL2), buf);
    sprintf(buf, "%d", pdc->cfg.ft8_carrier_freq);
    TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_VAL3), buf);
    sprintf(buf, "%d", pdc->cfg.echo_volume);
    TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_VAL4), buf);
    sprintf(buf, "%d", pdc->cfg.output_volume);
    TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_VAL5), buf);
    SLIDER_SetValue(WM_GetDialogItem(pMsg->hWin, ID_SL1), pdc->cfg.dim);
}

static void button_cbs(WM_MESSAGE* pMsg, int id)
{
    if(id >= ID_BT_T1N && id <= ID_BT_T5P) {
        const struct {
            int id;
            int id_val;
            const int* arr;
            int n;
            int action;
        } btns[] = { {ID_BT_T1N, ID_VAL1, cw_wpm_opt, sizeof(cw_wpm_opt)
            / sizeof(cw_wpm_opt[0]), PREV}, {ID_BT_T2N, ID_VAL2,
            cw_carrier_freq_opt, sizeof(cw_carrier_freq_opt)
                / sizeof(cw_carrier_freq_opt[0]), PREV}, {ID_BT_T3N, ID_VAL3,
            ft8_carrier_freq_opt, sizeof(ft8_carrier_freq_opt)
                / sizeof(ft8_carrier_freq_opt[0]), PREV}, {ID_BT_T4N, ID_VAL4,
            volume_opt, sizeof(volume_opt) / sizeof(volume_opt[0]), PREV}, {
        ID_BT_T5N, ID_VAL5, volume_opt, sizeof(volume_opt)
            / sizeof(volume_opt[0]), PREV}, {ID_BT_T1P, ID_VAL1, cw_wpm_opt,
            sizeof(cw_wpm_opt) / sizeof(cw_wpm_opt[0]), NEXT}, {ID_BT_T2P,
        ID_VAL2, cw_carrier_freq_opt, sizeof(cw_carrier_freq_opt)
            / sizeof(cw_carrier_freq_opt[0]), NEXT}, {ID_BT_T3P, ID_VAL3,
            ft8_carrier_freq_opt, sizeof(ft8_carrier_freq_opt)
                / sizeof(ft8_carrier_freq_opt[0]), NEXT}, {ID_BT_T4P, ID_VAL4,
            volume_opt, sizeof(volume_opt) / sizeof(volume_opt[0]), NEXT}, {
        ID_BT_T5P, ID_VAL5, volume_opt, sizeof(volume_opt)
            / sizeof(volume_opt[0]), NEXT}, };
        char buf[16];
        for(int i = 0; i < sizeof(btns) / sizeof(btns[0]); i++) {
            if(id == btns[i].id) {
                TEXT_GetText(WM_GetDialogItem(pMsg->hWin, btns[i].id_val), buf,
                    16);
                int val = atoi(buf);
                val = alter(btns[i].arr, btns[i].n, val, btns[i].action);
                sprintf(buf, "%d", val);
                TEXT_SetText(WM_GetDialogItem(pMsg->hWin, btns[i].id_val), buf);
            }
        }
    }
    else if(id == ID_BT_OK) {
        DataConfig_t* pdc = DC_Get();
        char buf[16];
_dbg();
        TEXT_GetText(WM_GetDialogItem(pMsg->hWin, ID_VAL1), buf, 16);
        pdc->cfg.cw_wpm = atoi(buf);
_dbg();
        TEXT_GetText(WM_GetDialogItem(pMsg->hWin, ID_VAL2), buf, 16);
        pdc->cfg.cw_carrier_freq = atoi(buf);
_dbg();
        TEXT_GetText(WM_GetDialogItem(pMsg->hWin, ID_VAL3), buf, 16);
        pdc->cfg.ft8_carrier_freq = atoi(buf);
_dbg();
        TEXT_GetText(WM_GetDialogItem(pMsg->hWin, ID_VAL4), buf, 16);
        pdc->cfg.echo_volume = atoi(buf);
_dbg();
        TEXT_GetText(WM_GetDialogItem(pMsg->hWin, ID_VAL5), buf, 16);
_dbg();
        pdc->cfg.output_volume = atoi(buf);
_dbg();
        pdc->cfg.dim = SLIDER_GetValue(WM_GetDialogItem(pMsg->hWin, ID_SL1));
_dbg();
        DC_Update();
_dbg();
        DISP_Dimming(pdc->cfg.dim);
_dbg();
RF_SetEchoVolume(pdc->cfg.echo_volume);
RF_SetOutputVolume(pdc->cfg.output_volume);
        WM_HideWindow(pdc->status.hw_cfg);
_dbg();
        WM_ShowWindow(pdc->status.hw_main);
_dbg();
    }
    else if(id == ID_BT_CANC) {
        DataConfig_t* pdc = DC_Get();
        restore_default_val(pMsg);
        DISP_Dimming(pdc->cfg.dim);
        WM_HideWindow(pdc->status.hw_cfg);
        WM_ShowWindow(pdc->status.hw_main);
    }
}

static void set_wm_text(int id, WM_HWIN hWin, const GUI_FONT* font)
{
    WM_HWIN h = WM_GetDialogItem(hWin, id);
    WM_CALLBACK* cb = WM_GetCallback(h);
    if(cb == BUTTON_Callback)
        BUTTON_SetFont(h, font);
    if(cb == TEXT_Callback)
        TEXT_SetFont(h, font);
}
static void set_wm_text_align(int id, WM_HWIN hWin, int align)
{
    WM_HWIN h = WM_GetDialogItem(hWin, id);
    WM_CALLBACK* cb = WM_GetCallback(h);
    if(cb == BUTTON_Callback)
        BUTTON_SetTextAlign(h, align);
    if(cb == TEXT_Callback)
        TEXT_SetTextAlign(h, align);
}

static void _cbDialog(WM_MESSAGE* pMsg)
{
//    WM_HWIN hItem;
    int NCode, Id;
    switch(pMsg->MsgId) {
        case WM_INIT_DIALOG:

            for(int id = ID_BT_OK; id <= ID_BT_CANC; id++) {
                set_wm_text(id, pMsg->hWin, GUI_FONT_24_ASCII);
            }
            for(int id = ID_T1; id <= ID_T6; id++) {
                set_wm_text(id, pMsg->hWin, GUI_FONT_20B_ASCII);
            }
            for(int id = ID_BT_T1N; id <= ID_BT_T5P; id++) {
                set_wm_text(id, pMsg->hWin, GUI_FONT_20B_ASCII);
            }
            for(int id = ID_VAL1; id <= ID_VAL5; id++) {
                set_wm_text(id, pMsg->hWin, GUI_FONT_24_ASCII);
                set_wm_text_align(id, pMsg->hWin,
                GUI_TA_HCENTER | GUI_TA_VCENTER);
            }
            for(int id = ID_T1; id <= ID_T6; id++) {
                set_wm_text_align(id, pMsg->hWin,
                GUI_TA_LEFT | GUI_TA_VCENTER);
            }
            restore_default_val(pMsg);
            break;
        case WM_NOTIFY_PARENT:
            Id = WM_GetId(pMsg->hWinSrc);
            NCode = pMsg->Data.v;
            if(NCode == WM_NOTIFICATION_RELEASED) {
                button_cbs(pMsg, Id);
            }
            switch(Id) {
                case ID_SL1:    // Notifications sent by 'SLIDER_DIMMING'
                    switch(NCode) {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            break;
                        case WM_NOTIFICATION_VALUE_CHANGED: {
                            int dim = SLIDER_GetValue(pMsg->hWinSrc);
                            DISP_Dimming(dim);
                        }
                            break;
                    }
                    break;
            }
            break;
        default:
            WM_DefaultProc(pMsg);
            break;
    }
}
WM_HWIN CreateWindow_Config(void);
WM_HWIN CreateWindow_Config(void)
{
    WM_HWIN hWin;
    hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate),
        _cbDialog, WM_HBKWIN, 0, 0);
    return hWin;
}
