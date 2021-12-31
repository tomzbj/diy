#include "GUI.h"

volatile GUI_TIMER_TIME OS_TimeMS;

GUI_TIMER_TIME GUI_X_GetTime(void)
{
    return OS_TimeMS;
}

void GUI_X_Delay(int ms)
{
    int tEnd = OS_TimeMS + ms;
    while((tEnd - OS_TimeMS) > 0);
}

void GUI_X_Init(void)
{
}

void GUI_X_ExecIdle(void)
{
}

void GUI_X_Log(const char* s)
{
    GUI_USE_PARA(s);
}
void GUI_X_Warn(const char* s)
{
    GUI_USE_PARA(s);
}
void GUI_X_ErrorOut(const char* s)
{
    GUI_USE_PARA(s);
}
