#include "misc.h"
#include "lvgl.h"

auto assert = [](int cond) {(cond) ? (exit(-1)) : (void)0;};

SDL_Wrapper::SDL_Wrapper(int x, int y, float zoom_factor)
{
  _w = x;
  _h = y;
  _zoom_factor_w = 1;
  _zoom_factor_h = 1;

  int ret = SDL_Init(SDL_INIT_EVERYTHING);
  assert(ret != 0);
  _pwin = SDL_CreateWindow("GAUGE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    x * zoom_factor, y * zoom_factor, SDL_WINDOW_RESIZABLE);
  assert(_pwin == NULL);
  _ren = SDL_CreateRenderer(_pwin, -1, SDL_RENDERER_ACCELERATED);
  assert(_ren == NULL);
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
  _surface = SDL_CreateRGBSurface(0, x, y, 16, 0xf800, 0x07e0, 0x001f, 0);
}

void* SDL_Wrapper::get_framebuffer(void)
{
  return _surface->pixels;
}

int SDL_Wrapper::update(void)
{
  int _w_actual, _h_actual;
  SDL_Event evt;
  if(SDL_PollEvent(&evt)) {
    SDL_GetWindowSize(_pwin, &_w_actual, &_h_actual);
    _zoom_factor_w = ((float)_w) / _w_actual;
    _zoom_factor_h = ((float)_h) / _h_actual;

    if(0) {
      static int count = 0;
      ++count %= 10;
      if(!count) {
        printf("### %d %d %d %d %f %f\n", _w, _h, _w_actual, _h_actual, _zoom_factor_w,
          _zoom_factor_h);
      }
    }
    int quit = (evt.type == SDL_QUIT) ? 1 : 0;
    if(evt.type == SDL_KEYDOWN) {
      const char* T = SDL_GetKeyName(evt.key.keysym.sym);
//             printf("%s\n", T);
      if(strcasecmp(T, "Escape") == 0)
        quit = 1;
      {
        extern class UI ui;
        if(strcasecmp(T, "U") == 0)
          ui.send_key(UI::KEY::UP_PRESS);
//          printf("U");
        if(strcasecmp(T, "J") == 0)
          ui.send_key(UI::KEY::DOWN_PRESS);
//          printf("J");
        if(strcasecmp(T, "I") == 0)
          ui.send_key(UI::KEY::UP_LONGPRESS);
//          printf("I");
        if(strcasecmp(T, "K") == 0)
          ui.send_key(UI::KEY::DOWN_LONGPRESS);
//          printf("K");
        fflush(stdout);
      }
    }
    if(quit > 0)
      return 1;
  }
  SDL_SetRenderDrawColor(_ren, 0, 0, 0, 0);    //清除背景
  SDL_RenderClear(_ren);
  SDL_Texture* tex = SDL_CreateTextureFromSurface(_ren, _surface);
  SDL_RenderCopy(_ren, tex, NULL, NULL);
  SDL_DestroyTexture(tex);
  SDL_RenderPresent(_ren);

  return 0;
}

SDL_Wrapper::~SDL_Wrapper(void)
{
  (_surface) ? SDL_FreeSurface(_surface) : (void)0;
  (_ren) ? SDL_DestroyRenderer(_ren) : (void)0;
  (_pwin) ? SDL_DestroyWindow(_pwin) : (void)0;

  if(SDL_WasInit(SDL_INIT_EVERYTHING) == 0)
    puts("SDL was NOT running!");
  else {
    SDL_Quit();
    puts("SDL_Quit successfully.");
  }    //    getch();
}
