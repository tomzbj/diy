#ifndef _ZUART_H
#define _ZUART_H

#define ZUART_PORT_NUM 1
#define ZUART_RX_MAX_LEN 64
#define ZUART_TX_MAX_LEN 64
#define ZUART_TX_MODE_INTERRUPT 0

typedef struct {
    void (*parser_f)(const void* msg, int size, int source);
    unsigned char (*readbyte_f)(int source);
    void (*writebyte_f)(unsigned char data, int source);
} zu_cfg_t;

void zu_poll(void);
void zu_init(zu_cfg_t* cfg);
void zu_idle_irqhandler(int usart_id);
void zu_rxne_irqhandler(int usart_id);
void zu_tc_irqhandler(int usart_id);
void zu_writedata(const void* msg, int size, int usart_id);

#endif
