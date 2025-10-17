#ifndef _ZEVENT_H
#define _ZEVENT_H

#include "zfifo.h"

class ZEVENT {
  public:
    typedef struct {
        uint32_t id;
        uint32_t data;
    } evt_t;

    ZEVENT(int zefifo_size) : _fifo(zefifo_size)
    {
    }
    void post(evt_t evt)
    {
      _fifo.put(&evt, sizeof(evt));
    }

    bool poll(evt_t* pevt)
    {
      if(_fifo.len() < sizeof(evt_t))
        return false;
      _fifo.get(pevt, sizeof(evt_t));
      return true;
    }

  private:
    ZFIFO _fifo;
};

#endif
