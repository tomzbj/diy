#ifndef _DAC_GDF3_H
#define _DAC_GDF3_H

void DAC_Config(void);
void DAC_SetFreq(int freq);
void DAC_SetTabSize(int tab_size);
void DAC_ViewTab(int (*printf_f)(const char* fmt, ...));

#endif
