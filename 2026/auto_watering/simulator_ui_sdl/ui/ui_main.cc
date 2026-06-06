#include "ui_main.h"
#include "ui.h"
#include <utility>

static void btn_manual_cb(lv_event_t* e)
{
  auto code = lv_event_get_code(e);
  auto p = (UI_MAIN*)lv_event_get_user_data(e);

  if(code == LV_EVENT_CLICKED) {
    p->pui->switch_page(UI::PAGE::CFG);
  }
}

void UI_MAIN::show(void)
{
  lv_obj_remove_flag(_page_root, LV_OBJ_FLAG_HIDDEN);
}

void UI_MAIN::hide(void)
{
  lv_obj_add_flag(_page_root, LV_OBJ_FLAG_HIDDEN);
}

void UI_MAIN::send_key(int key)
{
  if(key == UI::KEY::UP_PRESS)
    pui->manual_run();
  if(key == UI::KEY::DOWN_LONGPRESS)
    pui->switch_page(UI::PAGE::CFG);
}

void UI_MAIN::update(void)
{
  char buf[64] = {0};
  uint32_t time = pui->vals.time % 86400;
  sprintf(buf, "%02lu:%02lu:%02lu", time / 3600, (time / 60) % 60, time % 60);
  lv_label_set_text(l_time, buf);

  uint32_t time_next = pui->vals.time_next % 86400;
  sprintf(buf, "%02lu:%02lu:%02lu", time_next / 3600, (time_next / 60) % 60,
    time_next % 60);
  lv_label_set_text(l_time_next, buf);

  uint32_t duration = pui->vals.duration % 100;
  sprintf(buf, "%lus", duration);
  lv_label_set_text(l_duration, buf);

  uint32_t moisture = pui->vals.moisture;
  sprintf(buf, "%lu.%lu%%", moisture / 10, moisture % 10);
  lv_label_set_text(l_moisture, buf);
}

UI_MAIN::UI_MAIN(UI* ui) :
  UI_PAGE(ui)
{
}

void UI_MAIN::init(void)
{
//  this->pui = pui;
  static lv_style_t style_common;
  lv_style_init(&style_common);
  lv_style_set_text_font(&style_common, UI::font_cfg);
  lv_style_set_pad_all(&style_common, 0);

  {
    _page_root = lv_obj_create(lv_scr_act());
    lv_obj_set_size(_page_root, UI::WIDTH, UI::HEIGHT);
    lv_obj_clear_flag(_page_root, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(_page_root, 0, 3);
    lv_obj_add_style(_page_root, &style_common, 0);

    pobj con = lv_obj_create(_page_root);
    lv_obj_set_size(con, UI::WIDTH - 0, UI::HEIGHT - 0);
    lv_obj_set_style_radius(con, 15, 0);
    lv_obj_add_style(con, &style_common, 0);
    lv_obj_set_style_border_color(con, UI::light_grey, 0);
    lv_obj_set_style_border_width(con, 1, 0);
    lv_obj_set_style_bg_color(con, UI::light_grey, 0);
    lv_obj_clear_flag(con, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(con, LV_OBJ_FLAG_CLICKABLE);
  }
  struct CON {
      int x, y, w, h;
      const char* name;
  };
  const CON con_inf[6] = {
  //
    {10, 10, lv_pct(40), lv_pct(40), "当前时间"},    //
    {lv_pct(45), 10, lv_pct(40), lv_pct(40), "下次浇水"},    //
    {10, lv_pct(50), lv_pct(40), lv_pct(40), "浇水持续"},    //
    {lv_pct(45), lv_pct(50), lv_pct(40), lv_pct(40), "土壤湿度"},    //
    {lv_pct(88), 10, lv_pct(10), lv_pct(40), "手\n动"},    //
    {lv_pct(88), lv_pct(50), lv_pct(10), lv_pct(40), "设\n置"},    //
    };
  pobj val[6];

  for(int i = 0; i < 6; i++) {
    con_main[i] = lv_obj_create(_page_root);
    auto& con = con_main[i];
    auto& inf = con_inf[i];
    lv_obj_set_size(con, inf.w, inf.h);
    lv_obj_set_style_radius(con, 15, 0);
    lv_obj_add_style(con, &style_common, 0);
    lv_obj_set_style_border_color(con, UI::black, 0);
    lv_obj_set_style_border_width(con, 1, 0);
    lv_obj_set_style_bg_color(con, UI::white, 0);
    lv_obj_clear_flag(con, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(con, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_align(con, LV_ALIGN_TOP_LEFT, inf.x, inf.y);

    auto l_name = lv_label_create(con);
    lv_label_set_text(l_name, inf.name);
    lv_obj_set_style_text_color(l_name, UI::black, 0);
    lv_obj_set_style_text_font(l_name, UI::font_main, 0);
    if(i < 4)
      lv_obj_align(l_name, LV_ALIGN_CENTER, 0, -15);
    else
      lv_obj_align(l_name, LV_ALIGN_CENTER, 0, 0);
    if(i < 4) {
      val[i] = lv_label_create(con);
      lv_obj_set_style_text_color(val[i], UI::black, 0);
      lv_obj_set_style_text_font(val[i], UI::font_main, 0);
      lv_obj_align(val[i], LV_ALIGN_CENTER, 0, 15);
    }
  }
  l_time = val[0];
  l_time_next = val[1];
  l_duration = val[2];
  l_moisture = val[3];

  /*  lv_label_set_text(val[0], ""); lv_label_set_text(val[1], "");
   lv_label_set_text(val[2], ""); lv_label_set_text(val[3], "");*/

  lv_obj_add_flag(con_main[4], LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_flag(con_main[5], LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_event_cb(con_main[4], btn_manual_cb, LV_EVENT_ALL, this);
  lv_obj_add_event_cb(con_main[5], btn_manual_cb, LV_EVENT_ALL, this);
  hide();
}
