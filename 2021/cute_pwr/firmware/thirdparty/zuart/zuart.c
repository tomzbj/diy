#include "zuart.h"
#include <string.h>

#include "misc.h" //debug

typedef struct {
    int usart_id;
    unsigned char msg[ZUART_RX_MAX_LEN], rx_buf[ZUART_RX_MAX_LEN];
    unsigned char* rx_ptr;
    int rx_size, updated;

#if ZUART_TX_MODE_INTERRUPT
    unsigned char tx_buf[ZUART_TX_MAX_LEN];
    unsigned char* tx_ptr;
    int tx_size;
#endif
} usartx_t;

static struct {
    usartx_t usart[ZUART_PORT_NUM];
    zu_cfg_t cfg;
} g;

void zu_init(zu_cfg_t* cfg)
{
    g.cfg = *cfg;
    for(int i = 0; i < ZUART_PORT_NUM; i++) {
        g.usart[i].rx_ptr = g.usart[i].rx_buf;
        g.usart[i].rx_size = 0;
        g.usart[i].updated = 0;
#if ZUART_TX_MODE_INTERRUPT
        g.usart[i].tx_ptr = g.usart[i].tx_buf;
        g.usart[i].tx_size = 0;
#endif
    }
}

void zu_writedata(void* msg, int size, int usart_id)
{
#if ZUART_TX_MODE_INTERRUPT
    usartx_t* p = &(g.usart[usart_id]);

    while(p->tx_size > 0);
    if(size > ZUART_TX_MAX_LEN)
        size = ZUART_TX_MAX_LEN;
    memcpy(p->tx_buf, msg, size);
    g.cfg.writebyte_f(p->tx_buf[0], usart_id);
    p->tx_ptr = p->tx_buf + 1;
    p->tx_size = size - 1;
#else
    for(int i = 0; i < size; i++) {
        g.cfg.writebyte_f(*(unsigned char*)msg, usart_id);
        msg++;
    }
#endif
}

void zu_poll(void)
{
    for(int i = 0; i < ZUART_PORT_NUM; i++) {
        usartx_t* p = &(g.usart[i]);
        if(p->updated) {
            g.cfg.parser_f(p->msg, p->rx_size, i);
            p->updated = 0;
        }
    }
}

void zu_tc_irqhandler(int usart_id)
{
#if ZUART_TX_MODE_INTERRUPT
    for(int i = 0; i < ZUART_PORT_NUM; i++) {
        usartx_t* p = &(g.usart[i]);
        if(usart_id == p->usart_id) {
            if(p->tx_size > 0) {
                g.cfg.writebyte_f(*(p->tx_ptr), usart_id);
                p->tx_ptr++;
                p->tx_size--;
            }
            break;
        }
    }
#endif
}

void zu_rxne_irqhandler(int usart_id)
{
    for(int i = 0; i < ZUART_PORT_NUM; i++) {
        usartx_t* p = &(g.usart[i]);
        if(usart_id == p->usart_id) {
            if(p->rx_ptr - p->rx_buf >= ZUART_RX_MAX_LEN)
                break;
            *(p->rx_ptr) = g.cfg.readbyte_f(usart_id);    //p->usartx->DR;
            p->rx_ptr++;
            break;
        }
    }
}

void zu_idle_irqhandler(int usart_id)
{
    for(int i = 0; i < ZUART_PORT_NUM; i++) {
        usartx_t* p = &(g.usart[i]);
        if(usart_id == p->usart_id) {
            p->rx_size = p->rx_ptr - p->rx_buf;
            memcpy(p->msg, p->rx_buf, p->rx_size);
            memset(p->rx_buf, 0, sizeof(p->rx_buf));
            p->rx_ptr = p->rx_buf;
            p->updated = 1;
            break;
        }
    }
}
