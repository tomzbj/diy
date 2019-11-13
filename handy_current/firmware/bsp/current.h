#ifndef _CURRENT_H
#define _CURRENT_H

#define CURRENT_DIR_UP 10001
#define CURRENT_DIR_DOWN 10002

#define CURRENT_PREF_S125 201
#define CURRENT_PREF_S123 202
#define CURRENT_PREF_R5 203
#define CURRENT_PREF_R10 204

float CURRENT_GetVal(void);
void CURRENT_SetVal(float val);
void CURRENT_Config(void);
void CURRENT_SetChannel(int channel);
int CURRENT_GetChannel(void);
void CURRENT_Adjust(int dir);

#endif
