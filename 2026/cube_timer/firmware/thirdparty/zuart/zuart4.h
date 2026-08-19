/* Author: tomzbj  Updated: 2026-04-03 Rev: 0.05 */

#ifndef _ZUART4_H
#define _ZUART4_H

#include "zfifo.h"
#include <stdint.h>

class ZUART4: public ZFIFO {
  public:
    typedef struct {
        void (*write_f)(uint8_t* data, int size);
        void (*pre_tx_f)(void);
        void (*post_tx_f)(void);
    } cbs_t;

    ZUART4(const cbs_t& cbs, int rxfifo_size) : ZFIFO(rxfifo_size), _cbs(cbs)
    {
      _rx_idle_count = 0;
      _rx_idle_timeout = 5;
    }
    void set_timeout(int timeout)
    {
      _rx_idle_timeout = timeout;
    }
    int available(void)
    {
      return len();
    }
    void tick(void)
    {
      if(len() > 0) {
        _rx_idle_count++;
        if(_rx_idle_count > _rx_idle_timeout)
          _rx_idle_flag = true;
      }
    }
    int read(void* msg, int size)
    {
      int len = get(msg, size);
      if(available() == 0) {
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
      this->put(data, len);
      clear_idle();
    }
    bool idle(void)
    {
      return _rx_idle_flag;
    }

  private:
    const cbs_t& _cbs;
    int _rx_idle_count, _rx_idle_timeout;
    bool _rx_idle_flag = false;

    void clear_idle(void)
    {
      _rx_idle_flag = false;
      _rx_idle_count = 0;
    }
};

#endif
