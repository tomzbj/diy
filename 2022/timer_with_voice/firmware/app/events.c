#include "platform.h"

static void key_handler(int event)
{
//    return;
    DataConfig_t* pdc = DC_Get();
    switch(event) {
        case EVENT_KEY1_PRESS:
//            printf("key1 p\n");
            if(pdc->data.time + 600 > 5999)
                return;
            pdc->data.time += 600;
            if(pdc->status.state == DC_STATE_STOPPED) {
                pdc->status.state = DC_STATE_RUNNING;
                // push play "timer started"
                AUDIO_StartPlay("timergo1_8.wav");
            }
            else
                AUDIO_StartPlay("gun13_8.wav");
            break;
        case EVENT_KEY1_LONGPRESS:
            if(pdc->status.state == DC_STATE_STOPPED) {
                pdc->status.state = DC_STATE_RUNNING;
                // push play "timer started"
                AUDIO_StartPlay("timergo1_8.wav");
            }

            else if(pdc->status.state == DC_STATE_RUNNING) {
                pdc->status.state = DC_STATE_STOPPED;
                // push play "timer stopped"
                AUDIO_StartPlay("timerno1_8.wav");
            }
//            printf("key1 lp\n");
            break;
        case EVENT_KEY2_PRESS:
            if(pdc->data.time + 60 > 5999)
                return;
            pdc->data.time += 60;
            if(pdc->status.state == DC_STATE_STOPPED) {
                pdc->status.state = DC_STATE_RUNNING;
                // push play "timer started"
                AUDIO_StartPlay("timergo1_8.wav");
            }
            else
                AUDIO_StartPlay("gun5_8.wav");
//            printf("key2 p\n");
            break;
        case EVENT_KEY2_LONGPRESS:
            pdc->data.time = 0;
            if(pdc->status.state == DC_STATE_RUNNING) {
                pdc->status.state = DC_STATE_STOPPED;
                // push play "timer stopped"
                AUDIO_StartPlay("timerno1_8.wav");
            }

//            printf("key2 lp\n");
            break;
    }
    int time = pdc->data.time + 1;
    LCD_WriteInt((time / 60) * 100 + (time % 60));
    LCD_Update();
}

void EVENTS_Config(void)
{
    ze_bind(key_handler);
//    ze_bind(AUDIO_event_handler);
    printf("[%s: %d] EVENTS Initialized.\n", __FILE__, __LINE__);
}
