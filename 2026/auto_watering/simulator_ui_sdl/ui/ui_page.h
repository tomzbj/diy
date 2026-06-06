#ifndef _UI_PAGE_H
#define _UI_PAGE_H

#include "lvgl.h"

class UI;

class UI_PAGE {
  public:

    typedef lv_obj_t* pobj;
    UI* pui;
    pobj _page_root;

    UI_PAGE(UI* ui) :
      pui(ui)
    {
    }
    virtual ~UI_PAGE() = default;

    // 開機時只呼叫一次，用於建立控制項
    virtual void init(void) = 0;
    virtual void send_key(int key) = 0;

    virtual void show(void) = 0;
    virtual void hide(void) = 0;

    virtual void update(void)
    {
    }    // 依然可以用於定時更新數據
};

#endif
