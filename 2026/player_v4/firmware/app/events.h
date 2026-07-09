#ifndef _EVENTS_H
#define _EVENTS_H

namespace events {
  enum {
    KEY_CLICK, KEY_PRESS, KEY_LONGPRESS, KEY_DOUBLECLICK, U0_IDLE
  };
}

void EVENTS_Config(void);

#endif
