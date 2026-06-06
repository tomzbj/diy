#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "ui.h"

UI ui;
SDL_Wrapper sdl = SDL_Wrapper(320, 172, 2);
extern uint32_t now;

static void get_vals_f(void* pvals)
{
  auto vals = (UI::VALS*)pvals;
  extern uint32_t now;
  vals->time = now;
  vals->time_next = ui.cfgs.time_next;
  vals->duration = ui.cfgs.duration;
  vals->moisture += 7;
}

struct EEPROM {
    uint32_t time_next, duration, brightness;
} eeprom;

static void save_cfg_f(void* pcfgs)
{
  auto p = (UI::CFGS*)pcfgs;
  eeprom.brightness = p->brightness;
  eeprom.duration = p->duration;
  eeprom.time_next = p->time_next;
  now = p->time;
  {
    FILE* f = fopen("eeprom.bin", "wb");
    if(f == nullptr)
      return;
    fwrite(&eeprom, sizeof(eeprom), 1, f);
    fclose(f);
  }
}

static void load_cfg_f(void* pcfgs)
{
  {
    FILE* f = fopen("eeprom.bin", "rb");
    if(f == nullptr)
      return;
    fread(&eeprom, sizeof(eeprom), 1, f);
    fclose(f);
  }
  auto p = (UI::CFGS*)pcfgs;
  p->time = now;
  p->brightness = eeprom.brightness;
  p->duration = eeprom.duration;
  p->time_next = eeprom.time_next;
}

static void adjust_brightness_f(void* pval)
{
  _dbg();
}

void DISP_Config(void)
{
  lv_init();
  lv_port_disp_init();
  lv_port_indev_init();
  ui.bind_cb(ui.CB_ID::LOAD_CFG, load_cfg_f);
  ui.bind_cb(ui.CB_ID::SAVE_CFG, save_cfg_f);
  ui.bind_cb(ui.CB_ID::ADJUST_BRIGHTNESS, adjust_brightness_f);
  ui.bind_cb(ui.CB_ID::GET_VALS, get_vals_f);
  ui.init();
}
