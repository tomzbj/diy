#if 1
#include "lv_port_disp.h"
#include <stdbool.h>
#include "st7789.h"
extern class TFT_ST7789 tft;    //(320, 172, TFT::DATAWIDTH_8, cbs);
#define MY_DISP_HOR_RES    240
#define MY_DISP_VER_RES    240
#define BYTE_PER_PIXEL (LV_COLOR_FORMAT_GET_SIZE(LV_COLOR_FORMAT_RGB565)) 

static void disp_init(void);
static void disp_flush(lv_display_t* disp, const lv_area_t* area, uint8_t* px_map);

void lv_port_disp_init(void)
{
  disp_init();
  lv_display_t* disp = lv_display_create(MY_DISP_HOR_RES, MY_DISP_VER_RES);
  lv_display_set_color_format(disp, LV_COLOR_FORMAT_RGB565_SWAPPED);
  lv_display_set_flush_cb(disp, disp_flush);

  LV_ATTRIBUTE_MEM_ALIGN static uint8_t buf[MY_DISP_HOR_RES * 10 * BYTE_PER_PIXEL];
//  LV_ATTRIBUTE_MEM_ALIGN static uint8_t buf_2_2[MY_DISP_HOR_RES * 10 * BYTE_PER_PIXEL];
  lv_display_set_buffers(disp, buf, NULL, sizeof(buf), LV_DISPLAY_RENDER_MODE_PARTIAL);
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
static void disp_flush(lv_display_t* disp_drv, const lv_area_t* area, uint8_t* px_map)
{
  if(disp_flush_enabled) {
    tft.set_window(area->x1, area->y1, area->x2, area->y2);
    uint32_t n = (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1);
    tft.write((uint16_t*)px_map, n);
    lv_display_flush_ready(disp_drv);
  }
  lv_display_flush_ready(disp_drv);
}
#else 
typedef int keep_pedantic_happy;
#endif
