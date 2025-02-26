#ifndef _ZEVENT_H
#define _ZEVENT_H

#include "zfifo.h"

class ZEVENT {
  public:
    typedef void (*ze_parser)(int event);

    ZEVENT(void* zefifo, int zefifo_size) : _fifo(zefifo, zefifo_size)
    {
    }
    void post(uint8_t event)
    {
      _fifo.put(&event, 1);
    }

    int get(void)
    {
      uint8_t event;
      int len = _fifo.get(&event, 1);
      if(len == 0)
        return -1;
      else
        return event;
    }

  private:
    ZFIFO _fifo;
};

#endif
