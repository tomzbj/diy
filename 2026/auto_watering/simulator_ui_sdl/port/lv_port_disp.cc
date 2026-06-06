#include "lv_port_disp.h"
#include <cstdbool>
#include <cstdint>
#include "lvgl.h"
#include "sdl_wrapper.h"
#include <cstring>

#define MY_DISP_HOR_RES    320
#define MY_DISP_VER_RES    172

#define BYTE_PER_PIXEL (LV_COLOR_FORMAT_GET_SIZE(LV_COLOR_FORMAT_RGB565)) 

static void disp_init(void);

static void disp_flush(lv_display_t* disp, const lv_area_t* area, uint8_t* px_map);

void lv_port_disp_init(void)
{
  disp_init();

  lv_display_t* disp = lv_display_create(MY_DISP_HOR_RES, MY_DISP_VER_RES);
  //  lv_display_set_color_format(disp, LV_COLOR_FORMAT_RGB565_SWAPPED);
  lv_display_set_flush_cb(disp, disp_flush);

  LV_ATTRIBUTE_MEM_ALIGN
  static uint8_t buf_2_1[MY_DISP_HOR_RES * 10 * BYTE_PER_PIXEL];

  LV_ATTRIBUTE_MEM_ALIGN
  static uint8_t buf_2_2[MY_DISP_HOR_RES * 10 * BYTE_PER_PIXEL];
  lv_display_set_buffers(disp, buf_2_1, buf_2_2, sizeof(buf_2_1),
    LV_DISPLAY_RENDER_MODE_PARTIAL);

}

static void disp_init(void)
{

}

volatile bool disp_flush_enabled = true;

void disp_enable_update(void)
{
  disp_flush_enabled = true;
}

void disp_disable_update(void)
{
  disp_flush_enabled = false;
}

extern class SDL_Wrapper sdl;
static struct {
    int x1, y1, x2, y2;
} dr_window;

extern "C" void disp_set_window(int x1, int y1, int x2, int y2)
{
  dr_window.x1 = x1;
  dr_window.y1 = y1;
  dr_window.x2 = x2;
  dr_window.y2 = y2;
}

extern "C" void disp_write(uint16_t* buf, int n)
{
  uint8_t* p = (uint8_t*)sdl.get_framebuffer();
  int x = dr_window.x1;
  int y = dr_window.y1;
  int w = dr_window.x2 - dr_window.x1 + 1;
  while(n > 0) {
    int ofs = y * sdl.w() + x;
    //        memcpy(&p[ofs * 2], buf, 2 * w);
    memcpy(p + ofs * 2, buf, 2 * w);
    buf += w;    //sdl.w();
    n -= w;    //sdl.w();
    y++;
    if(y > dr_window.y2)
      break;    //{ y = dr_window.y1; }
  }
}

static void disp_flush(lv_display_t* disp_drv, const lv_area_t* area, uint8_t* px_map)
{
  if(disp_flush_enabled) {

    void disp_set_window(int x1, int y1, int x2, int y2);
    void disp_write(uint16_t* buf, int n);
    disp_set_window(area->x1, area->y1, area->x2, area->y2);
    disp_write((uint16_t*)px_map,
      (area->y2 - area->y1 + 1) * (area->x2 - area->x1 + 1));
    lv_disp_flush_ready(disp_drv);
  }

  lv_display_flush_ready(disp_drv);
}
