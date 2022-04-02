#include "platform.h"
#include "DIALOG.h"

#define ID_WINDOW_0             (GUI_ID_USER + 0x00)
#define ID_TEXT_PV              (GUI_ID_USER + 0x01)
#define ID_TEXT_SP              (GUI_ID_USER + 0x02)
#define ID_TEXT_PV_V              (GUI_ID_USER + 0x03)
#define ID_TEXT_PV_I              (GUI_ID_USER + 0x04)
#define ID_TEXT_SP_V              (GUI_ID_USER + 0x05)
#define ID_TEXT_SP_I            (GUI_ID_USER + 0x06)

// USER START (Optionally insert additional defines)
// USER END
// USER START (Optionally insert additional static data)
// USER END

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
//
    {WINDOW_CreateIndirect, "", ID_WINDOW_0, 4, 0, 128, 128, 0, 0x0, 0},    //
    {TEXT_CreateIndirect, "", ID_TEXT_PV, 0, 20, 160, 20, 0, 0x0, 0},    //
    {TEXT_CreateIndirect, "", ID_TEXT_PV_V, 0, 40, 160, 20, 0, 0x0, 0},    //
    {TEXT_CreateIndirect, "", ID_TEXT_SP, 0, 60, 160, 20, 0, 0x0, 0},    //
    {TEXT_CreateIndirect, "", ID_TEXT_SP_V, 0, 80, 160, 20, 0, 0x0, 0},    //
// USER START (Optionally insert additional widgets)
// USER END
    };
// USER START (Optionally insert additional static code)
//static void draw_battery(void) { int x0 = 80, y0 = 5; DataConfig_t* pdc = DC_Get(); }
// USER END
static void _cbDialog(WM_MESSAGE* pMsg)
{
    WM_HWIN hItem;
//    int NCode;
//    int Id;
    // USER START (Optionally insert additional variables)
    // USER END
    switch(pMsg->MsgId) {
        case WM_INIT_DIALOG:

//            hItem = pMsg->hWin; WINDOW_SetBkColor(hItem, GUI_LIGHTGREEN);

            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_PV);
            TEXT_SetText(hItem, "ACTUAL");
            TEXT_SetFont(hItem, GUI_FONT_16B_ASCII);
            TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_PV_V);
            TEXT_SetText(hItem, "3.03V 155mA");
            TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
            TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);

            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_SP);
            TEXT_SetText(hItem, "SETPOINT");
            TEXT_SetFont(hItem, GUI_FONT_16B_ASCII);
            TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_SP_V);
            TEXT_SetText(hItem, "3.04V 800mA");
            TEXT_SetFont(hItem, GUI_FONT_24_1);
            TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);

            // USER START (Optionally insert additional code for further widget initialization)
//            WM_CreateTimer(WM_GetDialogItem(pMsg->hWin, ID_TEXT_0), 0, 5, 0);
            WM_CreateTimer(WM_GetClientWindow(pMsg->hWin), 0, 5, 0);

            // USER END
            break;
            /*        case WM_NOTIFY_PARENT:
             Id = WM_GetId(pMsg->hWinSrc);
             NCode = pMsg->Data.v;
             switch(Id) {
             case ID_BUTTON_OK:    // Notifications sent by 'ZERO'
             switch(NCode) {
             case WM_NOTIFICATION_CLICKED:
             // USER START (Optionally insert code for reacting on notification message)
             // USER END
             break;
             case WM_NOTIFICATION_RELEASED:
             // USER START (Optionally insert code for reacting on notification message)
             {
             _dbg();
             DataConfig_t* pdc = DC_Get();
             //                            pdc->status.volt_ofs = pdc->data.volt;
             }
             // USER END
             break;
             // USER START (Optionally insert additional code for further notification handling)
             // USER END
             }
             break;
             // USER START (Optionally insert additional code for further Ids)
             // USER END
             }
             break;
             // USER START (Optionally insert additional message handling)
             */
        case WM_TIMER:
            WM_RestartTimer(pMsg->Data.v, 250);
//            WM_InvalidateWindow(WM_GetDialogItem(pMsg->hWin, ID_TEXT_UNIT));
//            WM_InvalidateWindow(WM_GetDialogItem(pMsg->hWin, ID_PROGBAR_0));
            break;
        case WM_PAINT: {
//            char buf[32];
//            DataConfig_t* pdc = DC_Get();
            /*
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
             */
//            TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_UNIT), buf);
//            sprintf(buf, "Vbat:%.1fV", pdc->data.vbat);
//            PROGBAR_SetValue(WM_GetDialogItem(pMsg->hWin, ID_PROGBAR_0), ((int)(pdc->data.vbat * 10)) * 100);
            GUI_SetColor(GUI_DARKGRAY);
//            GUI_FillRect(155, 8, 157, 13);

//            TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_VBAT), buf);
        }
            break;
            // USER END
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
    return hWin;
}
// USER START (Optionally insert additional public code)
// USER END
