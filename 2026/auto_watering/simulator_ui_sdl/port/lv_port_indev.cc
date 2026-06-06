#include "lv_port_indev.h"
#include "sdl_wrapper.h"
#include <cstdio>

static void mouse_init(void);
static void mouse_read(lv_indev_t* indev, lv_indev_data_t* data);
static bool mouse_is_pressed(void);
static void mouse_get_xy(int32_t* x, int32_t* y);

lv_indev_t* indev_mouse;

void lv_port_indev_init(void)
{
  mouse_init();
  indev_mouse = lv_indev_create();
  lv_indev_set_type(indev_mouse, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev_mouse, mouse_read);
}

static void mouse_init(void)
{
}

static void mouse_read(lv_indev_t* indev_drv, lv_indev_data_t* data)
{
  mouse_get_xy(&data->point.x, &data->point.y);
  if(mouse_is_pressed())
    data->state = LV_INDEV_STATE_PRESSED;
  else
    data->state = LV_INDEV_STATE_RELEASED;
}

static bool mouse_is_pressed(void)
{
  int x, y;
  return SDL_GetMouseState(&x, &y);
}

static void mouse_get_xy(int32_t* x, int32_t* y)
{
  extern class SDL_Wrapper sdl;
  int xx = 0, yy = 0;
  SDL_GetMouseState(&xx, &yy);
  *x = xx * sdl.zoom_factor_w();
  *y = yy * sdl.zoom_factor_h();
}
