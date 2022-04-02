#include "misc.h"
#include "disp.h"
#include "current.h"
#include "menu.h"

#define IsClick(key) ((key & 0xf) == 0xc)
#define IsLongPress(key) ((key & 0xffffff) == 0xfffffe)

typedef enum {
    KEY_NONE, KEY_UP_CLICK, KEY_DOWN_CLICK, KEY_LEFT_CLICK, KEY_RIGHT_CLICK
} KEY_Status_t;

void KEY_Config(void)
{
}

void KEY_Parse(KEY_Status_t status)
{
    DISP_State_t disp_state = DISP_GetState();
    if(disp_state == DISP_STATE_NORMAL) {
        switch(status) {
            case KEY_UP_CLICK:
                CURRENT_Adjust(CURRENT_DIR_UP);
                break;
//            case KEY1_LONGPRESS: break;
            case KEY_DOWN_CLICK:
                CURRENT_Adjust(CURRENT_DIR_DOWN);
                break;
            case KEY_RIGHT_CLICK:
                DISP_SetState(DISP_STATE_MENU);
                break;
            default:
                break;
        }
    }
    else if(disp_state == DISP_STATE_MENU) {
        switch(status) {
            case KEY_UP_CLICK:
                MENU_Action(MENU_ACTION_UP);
                break;
            case KEY_LEFT_CLICK:
                if(MENU_IsToplevel()) {
                    DISP_Clear();
                    DISP_SetState(DISP_STATE_NORMAL);
                    MENU_Config();
                }
                else
                    MENU_Action(MENU_ACTION_BACK);
                break;
            case KEY_DOWN_CLICK:
                MENU_Action(MENU_ACTION_DOWN);
                break;
            case KEY_RIGHT_CLICK:
                MENU_Action(MENU_ACTION_RUN);
                break;
            default:
                break;
        }
    }
}

#define NUM_OF_KEYS 4
const unsigned short key_pin[4] = {GPIO_Pin_9, GPIO_Pin_10, GPIO_Pin_11,
    GPIO_Pin_12};

void KEY_Read(void)
{
    static unsigned long key[NUM_OF_KEYS];
    KEY_Status_t status = KEY_NONE;
//    printf("%04x\n", (GPIOA->IDR & (GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12)));

    for(int i = 0; i < NUM_OF_KEYS; i++) {
        key[i] <<= 1;
        if(!bit_is_set(GPIOA, key_pin[i]))
            key[i] |= 1;
    }

    if(IsClick(key[0]))
        status = KEY_RIGHT_CLICK;
    else if(IsClick(key[1]))
        status = KEY_UP_CLICK;
    else if(IsClick(key[2]))
        status = KEY_LEFT_CLICK;
    else if(IsClick(key[3]))
        status = KEY_DOWN_CLICK;

    if(status != KEY_NONE) {
        KEY_Parse(status);
    }
}
