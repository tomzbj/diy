#ifndef _EVENTS_H
#define _EVENTS_H

class EVENTS {
  public:
    enum {
      NONE = -1, MILLISECOND, KEY_CLICKED, KEY_PRESSED, KEY_LONGPRESSED,
      KEY_DOUBLECLICKED
    };

    void poll(void);

  private:

};

#endif
