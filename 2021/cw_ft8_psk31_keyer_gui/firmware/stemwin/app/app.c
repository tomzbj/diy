#include "platform.h"
#include "DIALOG.h"

#define ID_WINDOW_0                 (GUI_ID_USER + 0x00)
#define ID_BT_OK                    (GUI_ID_USER + 0x01)
#define ID_BATT                     (GUI_ID_USER + 0x02)
#define ID_MODE                     (GUI_ID_USER + 0x03)
#define ID_RTC                      (GUI_ID_USER + 0x100)

#define BUTTON_HEIGHT 28

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
//
    {WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 320, 240, 0, 0, 0},    //
    {PROGBAR_CreateIndirect, "Progbar", ID_BATT, 240, 0, 78, 22, 0, 0, 0},    //
    {TEXT_CreateIndirect, "CW", ID_MODE, 5, 0, 90, 22, 0, 0, 0},    //
    {TEXT_CreateIndirect, "00:00:00", ID_RTC, 110, 0, 90, 22, 0, 0, 0}    //
};

//static void draw_battery(void) { int x0 = 80, y0 = 5; DataConfig_t* pdc = DC_Get(); }
static WM_HWIN edit;
static char edit_buf[100];

void GUI_AddChar(char c)
{
    MULTIEDIT_SetFont(edit, GUI_FONT_COMIC24B_ASCII);
    MULTIEDIT_SetReadOnly(edit, 0);
    MULTIEDIT_AddKey(edit, c);
    MULTIEDIT_SetReadOnly(edit, 1);
    WM_InvalidateWindow(edit);
    MULTIEDIT_SetFont(edit, GUI_FONT_32_ASCII);
}

void GUI_ClearMultiEdit(void)
{
    MULTIEDIT_SetText(edit, "");
    MULTIEDIT_SetReadOnly(edit, 1);
//    WM_InvalidateWindow(edit);
}

static void _cbDialog(WM_MESSAGE* pMsg)
{
    WM_HWIN hItem;
    int NCode, Id;

    switch(pMsg->MsgId) {
        case WM_INIT_DIALOG: {
            WINDOW_SetBkColor(pMsg->hWin, GUI_LIGHTCYAN);    //LIGHT_BLUE);

            WM_HWIN bt[50];
            const char* bt_text[] = {"1", "2", "3", "4", "5", "6", "7", "8",
                "9", "0", "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "A",
                "S", "D", "F", "G", "H", "J", "K", "L", "Z", "X", "C", "V", "B",
                "N", "M"};
            const GUI_FONT* font = GUI_FONT_32B_ASCII;
            int i;
            for(i = 0; i < sizeof(bt_text) / sizeof(bt_text[0]); i++) {
                int x, y;
                if(i < 10) {
                    x = 32 * i;
                    y = 95;
                }
                else if(i < 20) {
                    x = 32 * (i - 10);
                    y = 124;
                }
                else if(i < 29) {
                    x = 32 * (i - 20) + 16;
                    y = 153;
                }
                else if(i < 38) {
                    x = 32 * (i - 29) + 32;
                    y = 182;
                }
                else
                    break;
                bt[i] = BUTTON_CreateEx(x, y, 30, BUTTON_HEIGHT, pMsg->hWin,
                WM_CF_SHOW, 0, 0x803 + i);
                BUTTON_SetText(bt[i], bt_text[i]);
                BUTTON_SetFont(bt[i], font);
            }
            bt[i] = BUTTON_CreateEx(256, 182, 62, BUTTON_HEIGHT, pMsg->hWin,
            WM_CF_SHOW, 0, 0x803 + i);
            BUTTON_SetText(bt[i], "<<");
            BUTTON_SetFont(bt[i++], font);
            bt[i] = BUTTON_CreateEx(0, 211, 90, BUTTON_HEIGHT, pMsg->hWin,
            WM_CF_SHOW, 0, 0x803 + i);
            BUTTON_SetText(bt[i], "CFG");
            BUTTON_SetFont(bt[i++], font);
            bt[i] = BUTTON_CreateEx(95, 211, 95, BUTTON_HEIGHT, pMsg->hWin,
            WM_CF_SHOW, 0, 0x803 + i);
            BUTTON_SetText(bt[i], " ");
            BUTTON_SetFont(bt[i++], font);
            bt[i] = BUTTON_CreateEx(192, 211, 30, BUTTON_HEIGHT, pMsg->hWin,
            WM_CF_SHOW, 0, 0x803 + i);
            BUTTON_SetText(bt[i], "/");
            BUTTON_SetFont(bt[i++], font);
            bt[i] = BUTTON_CreateEx(224, 211, 30, BUTTON_HEIGHT, pMsg->hWin,
            WM_CF_SHOW, 0, 0x803 + i);
            BUTTON_SetText(bt[i], "?");
            BUTTON_SetFont(bt[i++], font);

            bt[i] = BUTTON_CreateEx(256, 211, 62, BUTTON_HEIGHT, pMsg->hWin,
            WM_CF_SHOW, 0, 0x803 + i);
            BUTTON_SetText(bt[i], "TX");
            BUTTON_SetFont(bt[i++], font);
            edit = MULTIEDIT_CreateEx(2, 23, 315, 70, pMsg->hWin, WM_CF_SHOW, 0,
                0x803 + i, 100, edit_buf);
            MULTIEDIT_SetText(edit, "|");
            MULTIEDIT_SetTextAlign(edit, (GUI_TA_TOP | GUI_TA_LEFT));
            MULTIEDIT_SetFont(edit, GUI_FONT_32_ASCII);
            MULTIEDIT_SetWrapWord(edit);
            MULTIEDIT_EnableBlink(edit, 200, 1);

            hItem = WM_GetDialogItem(pMsg->hWin, ID_BATT);
            PROGBAR_SetFont(hItem, GUI_FONT_24B_ASCII);
            PROGBAR_SetMinMax(hItem, 3500, 4000);
            PROGBAR_SetValue(hItem, 3845);

            TEXT_SetFont(WM_GetDialogItem(pMsg->hWin, ID_MODE),
            GUI_FONT_24B_ASCII);
            TEXT_SetFont(WM_GetDialogItem(pMsg->hWin, ID_RTC),
            GUI_FONT_24B_ASCII);

            WM_CreateTimer(WM_GetClientWindow(pMsg->hWin), 0, 1, 0);
        }

            break;
        case WM_NOTIFY_PARENT:
            Id = WM_GetId(pMsg->hWinSrc);
            {
                WM_CALLBACK* cb = WM_GetCallback(pMsg->hWinSrc);
                if(cb == BUTTON_Callback) {
                    if(pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
                        char key[8];
                        BUTTON_GetText(pMsg->hWinSrc, key, sizeof(key));
                        if(strcmp(key, "<<") == 0) {
                            MULTIEDIT_AddKey(edit, GUI_KEY_BACKSPACE);
                            MULTIEDIT_AddKey(edit, GUI_KEY_BACKSPACE);
                            MULTIEDIT_AddKey(edit, '|');
                        }
                        else if(strcmp(key, "CFG") == 0) {
                            DataConfig_t* pdc = DC_Get();
                            WM_HideWindow(pdc->status.hw_main);
                            WM_ShowWindow(pdc->status.hw_cfg);
                        }
                        else if(strcmp(key, "TX") == 0) {
//                            static int readonly;
//                            readonly = !readonly;
//                            MULTIEDIT_SetReadOnly(edit, readonly);
                            static char buf[100];
                            MULTIEDIT_GetText(edit, buf, sizeof(buf));
//                            printf("%s\n", buf);
                            int n = strlen(buf);
                            if(buf[n - 1] == '|')
                                n--;
                            RF_Parse(buf, n);
                        }
                        else {
                            MULTIEDIT_AddKey(edit, GUI_KEY_BACKSPACE);
                            MULTIEDIT_AddKey(edit, key[0]);
                            MULTIEDIT_AddKey(edit, '|');
                        }
                    }
                }
                else if(cb == TEXT_Callback) {
                    if(pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
                        if(WM_GetId(pMsg->hWinSrc) == ID_MODE) {
                            DataConfig_t* pdc = DC_Get();
                            const char* modes[] = {"CW", "BPSK31", "FT8"};
                            if(pdc->cfg.tx_mode != DC_TX_MODE_MAX) {
                                pdc->cfg.tx_mode++;
                            }
                            else {
                                pdc->cfg.tx_mode = DC_TX_MODE_MIN;
                            }
                            TEXT_SetText(pMsg->hWinSrc,
                                modes[pdc->cfg.tx_mode]);
                        }
                    }
                }
            }
            NCode = pMsg->Data.v;
            switch(Id) {
                case ID_BT_OK:    // Notifications sent by 'ZERO'
                    switch(NCode) {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED: {
                            _dbg();
//                            DataConfig_t* pdc = DC_Get();
//                            pdc->status.volt_ofs = pdc->data.volt;
                        }
                            break;
                    }
                    break;
            }
            break;
        case WM_TIMER:
            WM_RestartTimer(pMsg->Data.v, 250);
            char buf[16];
            RTC_GetTimeString(buf);
            TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_RTC), buf);
            WM_InvalidateWindow(WM_GetDialogItem(pMsg->hWin, ID_BATT));
            break;
        case WM_PAINT: {
//            char edit_buf[32];
            /*
             DataConfig_t* pdc = DC_Get();
             switch(pdc->cfg.unit) {
             case DC_UNIT_GS:
             sprintf(buf, "%.0fGs", pdc->data.B * 10.0);
             break;
             case DC_UNIT_MT:
             if(pdc->data.B >= 1000.0)
             sprintf(buf, "%.3fT", pdc->data.B / 1000.0);
             else
             sprintf(buf, "%.1fmT", pdc->data.B);
             break;
             }
             TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_UNIT), buf);
             //            sprintf(buf, "Vbat:%.1fV", pdc->data.vbat);
             PROGBAR_SetValue(WM_GetDialogItem(pMsg->hWin, ID_PROGBAR_0),
             ((int)(pdc->data.vbat * 10)) * 100);
             GUI_SetColor(GUI_DARKGRAY);
             GUI_FillRect(155, 8, 157, 13);
             */

//            TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_VBAT), buf);
        }
            break;

        default:
            WM_DefaultProc(pMsg);
            break;
    }
}
WM_HWIN CreateWindow(void);
WM_HWIN CreateWindow(void)
{
    WM_HWIN hWin;
    hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate),
        _cbDialog, WM_HBKWIN, 0, 0);

//    BUTTON_CreateAsChild (100, 100, 30, 30, hWin, 0x803, 0);
//    WINDOW_CreateUser(100, 100, 30, 30, hWin, 0,0, 0, _cbDialog,0);

    return hWin;
}

