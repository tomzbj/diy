#ifndef _PLAYBACK_H_
#define _PLAYBACK_H_

void PLAYBACK_Handler(void);
void PLAYBACK_Play(char* fname);
void PLAYBACK_Stop(void);
bool PLAYBACK_GetStatus(void);

#endif /* APP_PLAYBACK_PLAYBACK_H_ */
