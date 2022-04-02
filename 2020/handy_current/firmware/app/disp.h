#ifndef _DISP_H
#define _DISP_H

typedef enum {DISP_STATE_NORMAL, DISP_STATE_MENU} DISP_State_t;

void DISP_Config(void);
void DISP_Clear(void);
void DISP_Update(void);
void DISP_DrawPixel(unsigned short x, unsigned short y, unsigned long c);
void DISP_Off(void);
void DISP_SetUpdate(void);
DISP_State_t DISP_GetState(void);
void DISP_SetState(DISP_State_t state);

#endif
