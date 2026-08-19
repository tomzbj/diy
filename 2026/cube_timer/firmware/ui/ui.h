#ifndef _UI_H
#define _UI_H

#ifdef __cplusplus
extern "C" {
#endif

void UI_Config(void);
void UI_Task(void);

void UI_SetRemainSec(int sec);
void UI_SetTextVisible(int visible);

// level 0..4: EMPTY, 1/4, 1/2, 3/4, FULL
void UI_SetBatteryLevel(int level);

#ifdef __cplusplus
}
#endif

#endif
