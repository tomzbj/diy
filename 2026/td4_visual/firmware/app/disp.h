#ifndef _DISP_H
#define _DISP_H

#include "ugui.h"

class DISP {
  public:
    enum STATE {
      NORMAL, MOVE, EDIT
    };
    enum ACTION {
      KEY_UP_PRESS, KEY_DOWN_PRESS, KEY_UP_LONGPRESS, KEY_DOWN_LONGPRESS
    };
    void init(void);
    void update(void);
    void go(ACTION action);

  private:
    UG_GUI _gui;
    STATE _state = NORMAL;
    int _cursor_pos = 0;
    int _prev_cursor_pos = -1;
    STATE _prev_state = NORMAL;
};

//void DISP_Config(void);
//void DISP_Update(void);

#endif
