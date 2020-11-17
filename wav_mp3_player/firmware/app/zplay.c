#include "zplay.h"

static struct {
    zplay_cfg_t cfg;
    int irq_flag_ht, irq_flag_tc;
} g;

void zplay_init(zplay_cfg_t* cfg)
{
    g.cfg = *cfg;
}

void zplay_stop(void)
{
    g.cfg.dma_write_stop_f();
    g.cfg.close_f();
}

void zplay_poll(void)
{
    if(g.irq_flag_ht) {
        g.cfg.read_f(g.cfg.buf);
        g.irq_flag_ht = 0;
    }
    if(g.irq_flag_tc) {
        g.cfg.read_f(g.cfg.buf + g.cfg.buf_size / 2);
        g.irq_flag_tc = 0;
    }
}

void zplay_isr_ht(void)
{
    g.irq_flag_ht = 1;
}

void zplay_isr_tc(void)
{
    g.irq_flag_tc = 1;
}

void zplay_start(void)
{
    if(g.cfg.open_f() < 0)
        return;
    g.irq_flag_ht = 1;
    g.irq_flag_tc = 1;
    zplay_poll();
    g.cfg.dma_write_start_f(g.cfg.buf, g.cfg.buf_size);
}
