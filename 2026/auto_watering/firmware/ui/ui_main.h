#include "ui_page.h"

class UI_MAIN: public UI_PAGE {
  public:

    UI_MAIN(UI* ui);
    void init(void);
    void update(void);
    void show(void);
    void hide(void);
    void send_key(int key);

  private:
    pobj l_time, l_time_next, l_duration, l_moisture;
    pobj con_main[6];
};
