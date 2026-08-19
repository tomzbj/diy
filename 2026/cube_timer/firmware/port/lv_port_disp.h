#if 1

#ifndef LV_PORT_DISP_TEMPL_H
#define LV_PORT_DISP_TEMPL_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "lvgl.h"
#endif

  void lv_port_disp_init(void);
  void disp_enable_update(void);
  void disp_disable_update(void);

#ifdef __cplusplus
} 
#endif

#endif 

#endif 
