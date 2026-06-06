#include "ui_page.h"

class UI_CFG: public UI_PAGE {
  public:

    UI_CFG(UI* ui);
    void init(void);
    void update(void);
    void send_key(int key);
    void show(void);
    void hide(void);
    void save_cfg(void);
    void load_cfg(void);

  private:

    pobj ta_cur_hour, ta_cur_min, ta_start_hour, ta_start_min, ta_duration,
      sl_brightness, btn_ok, btn_cancel;    //    label_brightness_value;

    pobj* _widgets[8] = {&ta_cur_hour, &ta_cur_min, &ta_start_hour, &ta_start_min,
      &ta_duration, &sl_brightness, &btn_ok, &btn_cancel};

    enum STATE {
      TOGGLE, MODIFY
    } _state;
    enum ACTION {
      INC, DEC, NONE
    };
    int _active_widget;

    void highlight_widget(void);
    void update_widget(int action);
};
