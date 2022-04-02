#ifndef _RF_H
#define _RF_H

void RF_Config(void);
void RF_Parse(const void* msg, int size);
void RF_SetEchoVolume(int vol);
void RF_SetOutputVolume(int vol);

#endif
