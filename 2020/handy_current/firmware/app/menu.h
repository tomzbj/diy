/// @file menu.h
/// @brief 
/// @author Zhang Hao, stavrosatic@gmail.com
/// @version R0.1
/// @date 2017-10-12
#ifndef _MENU_C
#define _MENU_C

typedef enum {
    MENU_ACTION_UP, MENU_ACTION_DOWN, MENU_ACTION_BACK, MENU_ACTION_RUN
} MenuAction_t;

void MENU_Config(void);
void MENU_View(void);
int  MENU_IsToplevel(void);
void MENU_Action(MenuAction_t action);

#endif
