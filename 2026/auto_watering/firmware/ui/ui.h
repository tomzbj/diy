#ifndef _UI_H
#define _UI_H

#include "lvgl.h"
#include "misc.h"
#include <vector>
#include <memory>
#include "ui_page.h"

class UI {
  public:

    typedef void (*CBS)(void*);

    enum PAGE {
      MAIN = 0, CFG = 1, PAGE_COUNT, NONE = -1, MIN = MAIN, MAX = CFG,    // pages
    };
    enum CB_ID {
      LOAD_CFG, SAVE_CFG, ADJUST_BRIGHTNESS, GET_VALS, MANUAL_RUN, CB_COUNT
    };
    enum KEY {
      UP_PRESS, DOWN_PRESS, UP_LONGPRESS, DOWN_LONGPRESS
    };

    struct VALS {
        uint32_t time, time_next, duration, moisture;
    } vals;

    struct CFGS {
        uint32_t time, time_next, duration, brightness;
    } cfgs;

    static const lv_color_t black, white, light_green, light_grey, light_blue,
      deep_blue, dark_green, dark_blue, light_red, dark_red, light_yellow, red;
    static const lv_font_t* font_main;
    static const lv_font_t* font_cfg;
    constexpr static int WIDTH = 320, HEIGHT = 172;

    UI(void);
    ~UI(void);
    typedef void (*cb_t)(void*);

    void bind_cb(CB_ID cb_id, CBS cb)
    {
      if(cb_id < CB_COUNT)
        _cbs[cb_id] = cb;
    }
    void init(void);
    void update(void);
    void switch_page(PAGE page);

    // callback wrappers
    void load_cfg(void);
    void save_cfg(void);
    void send_key(KEY key);
    void adjust_brightness(int brightness);
    void manual_run(void);

  private:
    typedef lv_obj_t* pobj;
    std::vector<std::unique_ptr<UI_PAGE>> _pages;
    std::vector<CBS> _cbs;
    PAGE _active_page;
};

#endif // _UI_H
