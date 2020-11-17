#ifndef _ZPLAY_H
#define _ZPLAY_H

typedef struct {
    void (*dma_write_stop_f)(void);
    void (*dma_write_start_f)(void* buf, int size);
    void (*read_f)(void* buf);
    int (*open_f)(void);
    void (*close_f)(void);
    void* buf;
    int buf_size;
} zplay_cfg_t;

void zplay_init(zplay_cfg_t* cfg);
void zplay_start(void);
void zplay_stop(void);
void zplay_isr_tc(void);
void zplay_isr_ht(void);
void zplay_poll(void);

#endif
