#include "misc.h"
#include "ui.h"
#include "lvgl.h"
#include "meas.h"

static lv_obj_t* g_label;
static lv_obj_t* g_bat;
static int g_bat_level = -1;

LV_FONT_DECLARE(anton);

const lv_font_t* font_anton = &anton;

void UI_SetRemainSec(int sec)
{
  if(sec < 0)
    sec = 0;
  int mm = sec / 60;
  int ss = sec % 60;
  char buf[16] = {0};
  snprintf(buf, sizeof(buf), "%d:%02d", mm, ss);
  if(g_label)
    lv_label_set_text(g_label, buf);
}

void UI_SetTextVisible(int visible)
{
  if(!g_label)
    return;
  if(visible)
    lv_obj_clear_flag(g_label, LV_OBJ_FLAG_HIDDEN);
  else
    lv_obj_add_flag(g_label, LV_OBJ_FLAG_HIDDEN);
}

void UI_SetBatteryLevel(int level)
{
  static const char* icons[] = {
      LV_SYMBOL_BATTERY_EMPTY, LV_SYMBOL_BATTERY_1, LV_SYMBOL_BATTERY_2,
      LV_SYMBOL_BATTERY_3, LV_SYMBOL_BATTERY_FULL};
  if(level < 0)
    level = 0;
  if(level > 4)
    level = 4;
  if(level == g_bat_level || g_bat == nullptr)
    return;
  g_bat_level = level;
  lv_label_set_text(g_bat, icons[level]);
}

void UI_Task(void)
{
  if(MEAS_IsAlarming())
    return;
  lv_tick_inc(100);
  lv_timer_handler();
}

void UI_Config(void)
{
  lv_obj_set_style_bg_color(lv_scr_act(), lv_color_make(0, 0, 0), 0);

  g_label = lv_label_create(lv_screen_active());
  lv_obj_align(g_label, LV_ALIGN_CENTER, 0, 0);
  lv_label_set_text(g_label, "0:00");
  lv_obj_set_style_text_font(g_label, font_anton, 0);
  lv_obj_set_style_text_color(g_label, lv_palette_lighten(LV_PALETTE_GREEN, 1), 0);
  lv_obj_set_style_bg_color(g_label, lv_color_make(0, 0, 0), 0);

  // Symbols live in Montserrat, not Anton
  g_bat = lv_label_create(lv_screen_active());
  lv_obj_align(g_bat, LV_ALIGN_TOP_RIGHT, -6, 4);
  lv_obj_set_style_text_font(g_bat, &lv_font_montserrat_24, 0);
  lv_obj_set_style_text_color(g_bat, lv_color_make(200, 200, 200), 0);
  UI_SetBatteryLevel(4);
}
