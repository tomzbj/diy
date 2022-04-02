#include "platform.h"
#include "misc.h"
//#include <math.h>

#define DATA_COUNT 512

static struct {
    unsigned char data[DATA_COUNT * 2];     // double buffer
    unsigned long addr;
    int size;
    const char* fname;
} g;

/*
 static unsigned short swap(unsigned short x)
 {
 unsigned short y = (x >> 8) | (x << 8);
 return y;
 }
 */

void AUDIO_Setfname(const char* fname)
{
    g.fname = fname;
}

void AUDIO_event_handler(int event)
{
    void* read_ptr = g.data;
    if(event == EVENT_AUDIO_HT)
        read_ptr = g.data;
    else if(event == EVENT_AUDIO_TC)
        read_ptr = &(g.data[DATA_COUNT]);
    {
        const sfud_flash* flash = sfud_get_device_table() + 0;
        sfud_read(flash, g.addr, DATA_COUNT, read_ptr);
    }
    g.addr += DATA_COUNT;
    g.size -= DATA_COUNT;
    /*
     if(0) {
     for(int i = 0; i < DATA_COUNT; i++)
     *(unsigned char*)(read_ptr + i) ^= 0x80;
     }
     if(0) {
     if(event == EVENT_AUDIO_HT)
     printf("H\n");
     if(event == EVENT_AUDIO_TC)
     printf("T\n");
     }
     if(0) {
     static int count = 0;
     count++;
     //        if(count % 10 == 0)
     printf("%d %lu %04lX\n", count, TIM3->ARR, TIM3->CCR1);
     }
     */
    /*    for(i = n; i < n + (DATA_COUNT / 2); i++) {
     g.data[i] = swap(g.data[i]);
     g.data[i] ^= 0x8000;
     g.data[i] /= 64;
     }*/
//    printf("### %lu %d\n", g.addr, g.size);
    if(g.size < 0)
        AUDIO_StopPlay();
}

void AUDIO_StartPlay(const char* fname)
{
    g.fname = fname;
//    int i;
    DataConfig_t* pdc = DC_Get();

    int ret = ztar_find(&(pdc->tar), g.fname, &(g.addr), &(g.size));
    if(ret == -1)
        return;
//    printf("%lu %d\n", g.addr, g.size);
    GPIOB->BSRR = GPIO_Pin_5;
    AUDIO_event_handler(EVENT_AUDIO_HT);
    AUDIO_event_handler(EVENT_AUDIO_TC);

    DMA1_Channel3->CNDTR = DATA_COUNT * 2;
    DMA1_Channel3->CPAR = (unsigned long)(&TIM3->CCR1);
    DMA1_Channel3->CMAR = (unsigned long)(g.data);
    TIM_Cmd(TIM3, ENABLE);
    TIM_DMACmd(TIM3, TIM_DMA_Update, ENABLE);
    DMA_Cmd(DMA1_Channel3, ENABLE);

//    printf("playback start!\n");
}

void AUDIO_StopPlay(void)
{
//    DataConfig_t* pdc = DC_Get();
    TIM_Cmd(TIM3, DISABLE);
    TIM_DMACmd(TIM3, TIM_DMA_Update, DISABLE);
    DMA_Cmd(DMA1_Channel3, DISABLE);
    GPIOB->BRR = GPIO_Pin_5;
    printf("playback done!\n");
}
