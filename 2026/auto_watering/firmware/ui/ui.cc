#include "ui.h"
#include "lvgl.h"
#include "ui_main.h"
#include "ui_cfg.h"

LV_FONT_DECLARE (font_big);

const lv_font_t* UI::font_main = &font_big;
const lv_font_t* UI::font_cfg = &font_big;
const lv_color_t UI::black {lv_color_make(0, 0, 0)},    //
  UI::white {lv_color_make(255, 255, 255)},    //
  UI::light_green {lv_color_make(215, 255, 215)},    //
  UI::light_grey {lv_color_make(224, 224, 224)},    //
  UI::light_blue {lv_color_make(225, 225, 255)},    //
  UI::deep_blue {lv_color_make(45, 55, 155)},    //
  UI::dark_green {lv_color_make(55, 125, 55)},    //
  UI::dark_blue {lv_color_make(55, 55, 125)},    //
  UI::light_red {lv_color_make(255, 225, 225)},    //
  UI::dark_red {lv_color_make(192, 16, 16)},    //
  UI::light_yellow {lv_color_make(255, 255, 225)},    //
  UI::red {lv_color_make(255, 0, 0)};

void UI::update(void)
{
  _cbs[GET_VALS](&vals);
  _pages[_active_page]->update();
  /// TODO!
}

void UI::send_key(KEY key)
{
  _pages[_active_page]->send_key(key);
}

void UI::load_cfg(void)
{
  if(_cbs[LOAD_CFG] != nullptr)
    _cbs[LOAD_CFG](&cfgs);
}

void UI::save_cfg(void)
{
  if(_cbs[SAVE_CFG] != nullptr)
    _cbs[SAVE_CFG](&cfgs);
}

void UI::manual_run(void)
{
  if(_cbs[MANUAL_RUN] != nullptr)
    _cbs[MANUAL_RUN](&cfgs);
}

void UI::adjust_brightness(int brightness)
{
  if(_cbs[ADJUST_BRIGHTNESS] != nullptr)
    _cbs[ADJUST_BRIGHTNESS](&brightness);
}

void UI::switch_page(PAGE page_id)
{
  for(const auto& page : _pages)
    page->hide();
  _pages[page_id]->show();
  _active_page = page_id;
}

void UI::init(void)
{
  _pages.push_back(std::make_unique<UI_MAIN>(this));
  _pages.push_back(std::make_unique<UI_CFG>(this));

  _pages[0]->init();
  _pages[1]->init();
  load_cfg();
  switch_page(PAGE::MAIN);
}

UI::UI(void)
{
  _cbs.reserve(CB_COUNT);
}

UI::~UI() = default;
