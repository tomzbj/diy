#ifndef _IFACE_H
#define _IFACE_H

#include "ugui.h"

enum {
    UI_LABEL_TRIG_MODE,
    UI_LABEL_TRIG_LEVEL,
    UI_LABEL_TRIG_EDGE,
    UI_LABEL_COUPLING,
    UI_LABEL_SEN,
    UI_LABEL_TIMEBASE,
    UI_LABEL_FIRST = UI_LABEL_TRIG_MODE,
    UI_LABEL_LAST = UI_LABEL_TIMEBASE
};

void UI_Init(void);
void UI_DrawTrace(void);
void UI_SetLabelText(int id, const char* str);
void UI_UpdateLabels(void);
void UI_NextLabel(void);
int UI_GetActiveLabel(void);

#endif
