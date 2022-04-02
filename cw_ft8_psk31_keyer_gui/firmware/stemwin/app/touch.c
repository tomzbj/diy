#include "platform.h"

int GUI_TOUCH_X_MeasureX(void)
{
    return XPT2046_ReadY();
}

int GUI_TOUCH_X_MeasureY(void)
{
    return XPT2046_ReadX();
}

void GUI_TOUCH_X_ActivateX(void)
{
}

void GUI_TOUCH_X_ActivateY(void)
{
}
