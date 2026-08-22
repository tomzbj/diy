#ifndef _DISP_H
#define _DISP_H

#include "misc.h"
#include "u8g2.h"

class DISP {
  public:
    enum {
      STATE_NORMAL, STATE_MENU, STATE_EDIT, STATE_SELECT, STATE_ASKYN
    };

    void init(void);
    void update(int fast);
    void show_provision(const char* ap_ssid, const char* ap_pass);
    void show_need_setup(void);
    void hold_io_for_sleep(void);
    void set_state(int state)
    {
//      _state = saturate(state, STATE_NORMAL, STATE_ASKYN);
    }
    int get_state(void)
    {
      return _state;
    }

  private:
    int _state;
    u8g2_t _u8g2;

    void normal(void);
    void flush_fb(void);
};

#endif
