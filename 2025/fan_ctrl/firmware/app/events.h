#ifndef _EVENTS_H
#define _EVENTS_H

class EVENTS {
  public:
    enum {
      NONE = -1, KEY_PRESSED, KEY_LONGPRESSED, TIMER
    };
    void poll(void);

  private:

};

#endif
