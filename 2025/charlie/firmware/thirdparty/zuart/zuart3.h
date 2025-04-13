/* Author: tomzbj  Updated: 2024-09-15 Rev: 0.04 */

#ifndef _ZUART3_H
#define _ZUART3_H

#include "zfifo.h"
#include <stdint.h>

class ZUart3 {
  public:
    typedef struct {
        void (*write_f)(uint8_t* data, int size);
        void (*pre_tx_f)(void);
        void (*post_tx_f)(void);
    } cbs_t;

    ZUart3(const cbs_t& cbs, int rxfifo_size) : _cbs(cbs), _rx_fifo(rxfifo_size)
    {
      _rx_idle_count = 0;
      _rx_idle_timeout = 5;
    }
    void set_timeout(int timeout)
    {
      _rx_idle_timeout = timeout;
    }
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
        clear_idle();
      }
      return len;
    }
    void write(const void* msg, int size)
    {
      if(_cbs.pre_tx_f != nullptr)
        _cbs.pre_tx_f();
      _cbs.write_f((uint8_t*)msg, size);
      if(_cbs.post_tx_f != nullptr)
        _cbs.post_tx_f();
    }
    void write(uint8_t c)
    {
      write(&c, 1);
    }
    void push(uint8_t* data, int len)
    {
      _rx_fifo.put(data, len);
      clear_idle();
    }
    bool idle(void)
    {
      return _rx_idle_flag;
    }

  private:
    const cbs_t& _cbs;
    ZFIFO _rx_fifo;
    int _rx_idle_count, _rx_idle_timeout;
    bool _rx_idle_flag = false;

    void clear_idle(void)
    {
      _rx_idle_flag = false;
      _rx_idle_count = 0;
    }
};

#endif
