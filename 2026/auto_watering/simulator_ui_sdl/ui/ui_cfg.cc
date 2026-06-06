#include "ui.h"
#include "ui_cfg.h"
#include <string>

static void btn_cfg_save_event_cb(lv_event_t* e)
{
  if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
    auto p = (UI_CFG*)lv_event_get_user_data(e);
    p->save_cfg();
    p->pui->switch_page(UI::PAGE::MAIN);
  }
}

static void btn_cfg_event_cb(lv_event_t* e)
{
  if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
    auto p = (UI_CFG*)lv_event_get_user_data(e);
    p->load_cfg();
    p->pui->switch_page(UI::PAGE::MAIN);
  }
}

static void adjust_brightness_cb(lv_event_t* e)    // for slider
{
  auto ta = (lv_obj_t*)lv_event_get_target(e);
  auto p = (UI_CFG*)lv_event_get_user_data(e);
  auto val = lv_slider_get_value(ta);
  if(lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED) {
    _dbg();
    p->pui->adjust_brightness(val);
  }
}

UI_CFG::UI_CFG(UI* ui) :
  UI_PAGE(ui)
{
}

void UI_CFG::hide(void)
{
  lv_obj_add_flag(_page_root, LV_OBJ_FLAG_HIDDEN);
}

void UI_CFG::highlight_widget(void)
{
  for(int i = 0; i < 8; i++) {
    pobj obj = *_widgets[i];
    if(i == _active_widget)
      lv_obj_set_style_border_width(obj, 1, 0);
    else
      lv_obj_set_style_border_width(obj, 0, 0);
  }
}

void UI_CFG::load_cfg(void)
{
  pui->load_cfg();

  lv_slider_set_value(sl_brightness, pui->cfgs.brightness, LV_ANIM_OFF);
  char buf[32] = {0};
  lv_textarea_set_text(ta_start_hour, itoa(pui->cfgs.time_next / 3600, buf, 10));
  lv_textarea_set_text(ta_start_min, itoa((pui->cfgs.time_next / 60) % 60, buf, 10));
  lv_textarea_set_text(ta_cur_hour, itoa(pui->cfgs.time / 3600, buf, 10));
  lv_textarea_set_text(ta_cur_min, itoa((pui->cfgs.time / 60) % 60, buf, 10));
  lv_textarea_set_text(ta_duration, itoa(pui->cfgs.duration, buf, 10));

  for(int i = 0; i < 8; i++) {
    _active_widget = i;
    update_widget(NONE);
  }
  _active_widget = 0;
}

void UI_CFG::save_cfg(void)
{
  pui->cfgs.brightness = lv_slider_get_value(sl_brightness);
  pui->cfgs.time_next = (atoi(lv_textarea_get_text(ta_start_hour)) * 3600
    + atoi(lv_textarea_get_text(ta_start_min)) * 60) % 86400;
  pui->cfgs.time = (atoi(lv_textarea_get_text(ta_cur_hour)) * 3600
    + atoi(lv_textarea_get_text(ta_cur_min)) * 60) % 86400;
  pui->cfgs.duration = atoi(lv_textarea_get_text(ta_duration));
  pui->save_cfg();
}

void UI_CFG::show(void)
{
  lv_obj_remove_flag(_page_root, LV_OBJ_FLAG_HIDDEN);
  load_cfg();
  _state = TOGGLE;
  _active_widget = 0;
  highlight_widget();
}

void UI_CFG::update_widget(int action)
{
  auto obj = *_widgets[_active_widget];
  int limits[5] = {24, 60, 24, 60, 16};
  if(lv_obj_has_class(obj, &lv_textarea_class)) {
    const char* ptext = lv_textarea_get_text(obj);
    int val = atoi(ptext);
    if(action == INC)
      val++;
    else if(action == DEC)
      val += limits[_active_widget] - 1;
    val %= limits[_active_widget];
    char buf[16] = {0};
    sprintf(buf, "%02d", val);
    lv_textarea_set_text(obj, buf);
  }
  else if(lv_obj_has_class(obj, &lv_slider_class)) {
    int val = lv_slider_get_value(obj);
    if(action == DEC && val > lv_slider_get_min_value(obj))
      val--;
    else if(action == INC && val < lv_slider_get_max_value(obj))
      val++;
    lv_slider_set_value(obj, val, LV_ANIM_ON);
    pui->adjust_brightness(val);
  }
}

void UI_CFG::send_key(int key)
{
//  _dbg(); printf("%d\n", key);
  auto obj = *_widgets[_active_widget];
  if(_state == TOGGLE) {
    switch(key) {
      case UI::KEY::UP_PRESS:
        _active_widget += 7;
        _active_widget %= 8;
        highlight_widget();
        break;
      case UI::KEY::DOWN_PRESS:
        _active_widget++;
        _active_widget %= 8;
        highlight_widget();
        break;
      case UI::KEY::UP_LONGPRESS:
        case UI::KEY::DOWN_LONGPRESS:
        if(obj == btn_ok) {
          save_cfg();
          pui->switch_page(UI::PAGE::MAIN);
        }
        else if(obj == btn_cancel) {
          load_cfg();
          pui->switch_page(UI::PAGE::MAIN);
        }
        else
          _state = MODIFY;
        lv_obj_set_style_border_width(obj, 3, 0);
        break;
    }
  }
  else if(_state == MODIFY) {
    switch(key) {
      case UI::KEY::UP_PRESS:
        update_widget(DEC);
        break;
      case UI::KEY::DOWN_PRESS:
        update_widget(INC);
        break;
      case UI::KEY::UP_LONGPRESS:
        case UI::KEY::DOWN_LONGPRESS:
        _state = TOGGLE;
        lv_obj_set_style_border_width(obj, 1, 0);
        break;
    }

  }
}

void UI_CFG::update(void)
{
}

void UI_CFG::init(void)
{
//  pui = ui;
  _page_root = lv_obj_create(lv_scr_act());
  lv_obj_align(_page_root, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_size(_page_root, UI::WIDTH - 20, UI::HEIGHT);    //设置容器大小
  lv_obj_set_style_radius(_page_root, 0, 0);    //设置容器圆角为直角
  lv_obj_set_style_border_width(_page_root, 0, 0);

  lv_obj_set_flex_flow(_page_root, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(_page_root, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER,
    LV_FLEX_ALIGN_CENTER);
  lv_obj_set_style_pad_all(_page_root, 1, 0);
  lv_obj_set_style_pad_gap(_page_root, 0, LV_PART_MAIN);

  pobj row[5] = {0};
  for(uint32_t i = 0; i < sizeof(row) / sizeof(row[0]); i++) {
    row[i] = lv_obj_create(_page_root);
    auto& obj = row[i];
    lv_obj_set_size(obj, lv_pct(100), 33);
    lv_obj_set_style_pad_all(obj, 0, 0);
    lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER,
      LV_FLEX_ALIGN_CENTER);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);

    if(i % 2 == 0)
      lv_obj_set_style_bg_color(obj, UI::light_green, 0);
    else
      lv_obj_set_style_bg_color(obj, UI::light_yellow, 0);
    lv_obj_set_style_border_width(obj, 0, 0);
  }

  pobj lbl_cur_time = lv_label_create(row[0]);
  pobj lbl_start_time = lv_label_create(row[1]);
  pobj lbl_duration = lv_label_create(row[2]);
  pobj lbl_brightness = lv_label_create(row[3]);
  btn_ok = lv_button_create(row[4]);
  btn_cancel = lv_button_create(row[4]);
  pobj lbl_btn_ok = lv_label_create(btn_ok);
  pobj lbl_btn_cancel = lv_label_create(btn_cancel);

  ta_cur_hour = lv_textarea_create(row[0]);
  pobj colon1 = lv_label_create(row[0]);
  ta_cur_min = lv_textarea_create(row[0]);

  ta_start_hour = lv_textarea_create(row[1]);
  pobj colon2 = lv_label_create(row[1]);
  ta_start_min = lv_textarea_create(row[1]);

  ta_duration = lv_textarea_create(row[2]);
  sl_brightness = lv_slider_create(row[3]);
  lv_slider_set_min_value(sl_brightness, 0);
  lv_slider_set_max_value(sl_brightness, 10);
  lv_obj_add_event_cb(sl_brightness, adjust_brightness_cb, LV_EVENT_ALL, this);

  lv_label_set_text(lbl_cur_time, "当前时间");
  lv_label_set_text(lbl_start_time, "下次浇水");
  lv_label_set_text(lbl_duration, "浇水持续时间(秒)");
  lv_label_set_text(lbl_brightness, "屏幕亮度");
  lv_label_set_text(colon1, ":");
  lv_label_set_text(colon2, ":");
  lv_label_set_text(lbl_btn_ok, LV_SYMBOL_OK);
  lv_label_set_text(lbl_btn_cancel, LV_SYMBOL_CLOSE);

  lv_obj_set_width(lbl_cur_time, lv_pct(50));
  lv_obj_set_width(lbl_start_time, lv_pct(50));
  lv_obj_set_width(sl_brightness, lv_pct(50));
  lv_obj_set_height(sl_brightness, 15);
  lv_obj_set_size(btn_ok, lv_pct(30), 25);
  lv_obj_set_size(btn_cancel, lv_pct(30), 25);
  lv_obj_align(lbl_btn_ok, LV_ALIGN_CENTER, 0, 0);
  lv_obj_align(lbl_btn_cancel, LV_ALIGN_CENTER, 0, 0);
  lv_obj_add_event_cb(btn_ok, btn_cfg_save_event_cb, LV_EVENT_ALL, this);
  lv_obj_add_event_cb(btn_cancel, btn_cfg_event_cb, LV_EVENT_ALL, this);

  for(auto& obj : {ta_cur_min, ta_cur_hour, ta_start_hour, ta_start_min, ta_duration}) {
    lv_obj_set_style_margin_all(obj, 2, 0);
    lv_obj_set_style_pad_all(obj, 0, 0);
    lv_obj_set_size(obj, lv_pct(15), 20);
    lv_textarea_set_one_line(obj, true);
    lv_textarea_set_accepted_chars(obj, "1234567890");
    lv_textarea_set_text(obj, "00");
    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
  }
  for(auto& obj : {ta_cur_min, ta_cur_hour, lbl_cur_time, colon1, ta_start_min,
    ta_start_hour, lbl_start_time, colon2, ta_duration, lbl_duration, lbl_brightness})
    lv_obj_set_style_text_font(obj, UI::font_cfg, 0);
  for(auto& pobj : _widgets) {
    lv_obj_set_style_border_color(*pobj, UI::red, 0);
    lv_obj_set_style_border_width(*pobj, 0, 0);
  }

  hide();
}
