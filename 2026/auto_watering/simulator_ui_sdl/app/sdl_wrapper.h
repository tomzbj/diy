#ifndef _GUI_H
#define _GUI_H

#include "sdl.h"

class SDL_Wrapper {

  public:
    SDL_Wrapper(int x, int y, float zoom_factor);
    ~SDL_Wrapper(void);
    int update(void);
    void* get_framebuffer(void);
    //    static void quit_f(void);
    int w(void)
    {
      return _w;
    }
    int h(void)
    {
      return _h;
    }
    float zoom_factor_w(void)
    {
      return _zoom_factor_w;
    }
    float zoom_factor_h(void)
    {
      return _zoom_factor_h;
    }
  private:
    int _w, _h;
    float _zoom_factor_w, _zoom_factor_h;
    SDL_Renderer* _ren;
    SDL_Surface* _surface;
    SDL_Window* _pwin;
};

#endif
