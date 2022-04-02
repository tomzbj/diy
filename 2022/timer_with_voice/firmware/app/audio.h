#ifndef _AUDIO_H
#define _AUDIO_H

void AUDIO_StartPlay(const char* fname);
void AUDIO_StopPlay(void);
void AUDIO_event_handler(int event);
void AUDIO_Setfname(const char* fname);

#endif
