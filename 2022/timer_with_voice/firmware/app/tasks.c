#include "platform.h"

static void hello(void)
{
//    printf("Hello, world.\n");
    RCC_ClocksTypeDef clk;
    RCC_GetClocksFreq(&clk);
    printf("%lu\n", clk.SYSCLK_Frequency);
}

static void update_params(void)
{
    static DataConfig_t old_dc;
    DataConfig_t* pdc = DC_Get();
    if(memcmp(&old_dc.status, &(pdc->status), sizeof(old_dc.status)) != 0) {
    }
    old_dc = *pdc;
}

static void lcd_test(void)
{
    static int count;
    count++;
    LCD_WriteInt(count);
}

static void dots(void)
{
    static int n = 1;
    n = !n;
    LCD_WriteDots(n);
    if(DC_Get()->status.state == DC_STATE_RUNNING)
        LCD_Update();
}

static void time(void)
{
    DataConfig_t* pdc = DC_Get();
    int time = (pdc->data.time / 60) * 100 + (pdc->data.time % 60);
    if(time > 0 && pdc->status.state == DC_STATE_RUNNING) {
        pdc->data.time--;
        //@formatter:off
        switch(pdc->data.time) {
            case 40 * 60: AUDIO_StartPlay("40minr_8.wav"); break;
            case 30 * 60: AUDIO_StartPlay("30minr_8.wav"); break;
            case 20 * 60: AUDIO_StartPlay("20minr_8.wav"); break;
            case 10 * 60: AUDIO_StartPlay("10minr_8.wav"); break;
            case 5 * 60: AUDIO_StartPlay("5minr_8.wav"); break;
            case 4 * 60: AUDIO_StartPlay("4minr_8.wav"); break;
            case 3 * 60: AUDIO_StartPlay("3minr_8.wav"); break;
            case 2 * 60: AUDIO_StartPlay("2minr_8.wav"); break;
            case 1 * 60: AUDIO_StartPlay("1minr_8.wav"); break;
            case 9: case 8: case 7: case 6: case 5:
            case 4: case 3: case 2: case 1:
                AUDIO_StartPlay("turret1_8.wav"); break;
            case 0 * 60:
                AUDIO_StartPlay("kaboom22_8.wav");
                pdc->status.state = DC_STATE_STOPPED;
                break;
        } //@formatter:on

}
    LCD_WriteInt(time);
}

void TASKS_Config(void)
{
    zt_bind(hello, 500, 0);
    zt_bind(lcd_test, 500, 0);
    zt_bind(dots, 500, 1);
    zt_bind(time, 1000, 1);
    zt_bind(update_params, 100, 1);
    zt_bind(zk_poll, 1, 1);

    printf("[%s: %d] TASKS Initialized.\n", __FILE__, __LINE__);
}
