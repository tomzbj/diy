/* Author: tomzbj  Updated: 2024-05-01 Rev: 0.02 */

#ifndef _ZUART2_H
#define _ZUART2_H

#include "zfifo.h"
#include <stdint.h>

class ZUart2 {
public:
    typedef struct {
        uint8_t (*readbyte_f)(void);
        void (*writebyte_f)(uint8_t data);
        void (*pre_tx_f)(void);
        void (*post_tx_f)(void);
    } cb_t;

    ZUart2(const cb_t& cbs, void* rx_fifo_workbuf, int rx_fifo_size,
        void* tx_fifo_workbuf, int tx_fifo_size)    //
    : _cbs(cbs), _rx_fifo(rx_fifo_workbuf, rx_fifo_size), _tx_fifo(
        tx_fifo_workbuf, tx_fifo_size)
    {
        _rx_idle_count = 0;
        _rx_idle_timeout = 5;
    }
    void set_timeout(int timeout)
    {
        _rx_idle_timeout = timeout;
    }
    virtual ~ZUart2(void)
    {
    }
//    void poll(void);
    uint8_t peek(void)
    {
        return _rx_fifo.peek();
    }
    int available(void)
    {
        return _rx_fifo.len();
    }
    void tick(void)
    {
        if(_rx_fifo.len() > 0) {
            _rx_idle_count++;
            if(_rx_idle_count > _rx_idle_timeout)
                _rx_idle_flag = true;
        }
    }
    int read(void* msg, int size)
    {
        int len = _rx_fifo.get(msg, size);
        if(_rx_fifo.len() == 0) {
            _rx_idle_flag = false;
            _rx_idle_count = 0;
        }
        return len;
    }
    void write(const void* msg, int size)
    {
        _tx_irq = false;
        auto pdata = (uint8_t*)msg;
        for(int i = 0; i < size; i++)
            _cbs.writebyte_f(pdata[i]);
        _tx_irq = true;
    }
    void write(uint8_t c)
    {
        _tx_irq = false;
        _cbs.writebyte_f(c);
        _tx_irq = true;
    }
    void tc_irqhandler(void)
    {
        if(_tx_irq == false)
            return;
        if(_tx_fifo.len() > 0) {
            uint8_t c;
            _tx_fifo.get(&c, 1);
            _cbs.writebyte_f(c);
        }
    }
    void rxne_irqhandler(void)
    {
        if(_cbs.readbyte_f != NULL) {
            uint8_t c = _cbs.readbyte_f();
            _rx_fifo.put(&c, 1);
        }
    }
    void rxfifo_put(uint8_t* data, int len)
    {
        _rx_fifo.put(data, len);
    }
    bool idle(void)
    {
        return _rx_idle_flag;
    }

private:
    const cb_t& _cbs;
    ZFIFO<uint8_t> _rx_fifo, _tx_fifo;
    int _rx_idle_count, _rx_idle_timeout;
    bool _rx_idle_flag = false;
    bool _tx_irq = false;
};

#endif
