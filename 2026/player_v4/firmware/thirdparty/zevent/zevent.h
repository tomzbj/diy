/* Author: tomzbj  Updated: 2025-11-4 Rev2 */

#ifndef _ZEVENT_H
#define _ZEVENT_H

#include "zfifo.h"

class ZEVENT {
  public:
    ZEVENT(int zefifo_size, int max_events) : _fifo(zefifo_size)
    {
      _events = std::make_unique<evt_cb[]>(max_events);
      _num_events = 0;
      _max_events = max_events;
    }
    void post(uint32_t id, uint32_t data)
    {
      evt_t evt = {id, data};
      _fifo.put(&evt, sizeof(evt));
    }
    void poll(void)
    {
      if(_fifo.available() < sizeof(evt_t))
        return;
      evt_t evt;
      _fifo.get(&evt, sizeof(evt_t));
      for(int i = 0; i < _num_events; i++) {
        if(_events[i].id == evt.id) {
          _events[i].action_f(evt.data);
        }
      }
    }

    int bind(uint32_t id, void (*action_f)(uint32_t data))
    {
      if(_num_events >= _max_events)
        return -1;
      _events[_num_events].action_f = action_f;
      _events[_num_events].id = id;
      _num_events++;
      return 0;
    }

  private:

    typedef struct {
        uint32_t id;
        uint32_t data;
    } evt_t;

    typedef struct {
        uint32_t id;
        void (*action_f)(uint32_t data);
    } evt_cb;

    ZFIFO _fifo;
    std::unique_ptr<evt_cb[]> _events;
    int _num_events, _max_events;
};

#endif
