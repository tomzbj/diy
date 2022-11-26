#ifndef _DISP_H
#define _DISP_H

void DISP_Update(void);
void DISP_Config(void);

#define print_log() DISP_Log(__FILE__, __LINE__)

void DISP_Log(char* s, int n);

#endif
