#include <stdbool.h>
#include "ff.h"
#include "usart.h"
#include "vs1003.h"
#include "pwr_f1.h"

struct {
    FIL fil;
    bool fil_opened;
} g_play_info = {fil_opened:false};

void PLAYBACK_Stop(void)
{
    if(g_play_info.fil_opened == true) {
        f_close(&(g_play_info.fil));
        g_play_info.fil_opened = false;
    }
}

bool PLAYBACK_GetStatus(void)
{
    return g_play_info.fil_opened;
}

void PLAYBACK_Play(char* fname)
{
    PLAYBACK_Stop();
    FRESULT res;

    res = f_open(&(g_play_info.fil), fname, FA_READ);
    if(res) {
        uprintf(USART1, "Failed to open file.\n");
        return;
    }
    g_play_info.fil_opened = true;
}

void PLAYBACK_Handler(void)
{
    unsigned char buf[32];
    unsigned int size;

    if(g_play_info.fil_opened == true) {
        if(VS1003_GetDREQ() > 0) {
            f_read(&(g_play_info.fil), buf, 32, &size);
            VS1003_WriteData(buf, size);
        }
        if(f_eof(&(g_play_info.fil))) {
            PLAYBACK_Stop();
            uprintf(USART1, "playback end.\n");
        }
    }
}
