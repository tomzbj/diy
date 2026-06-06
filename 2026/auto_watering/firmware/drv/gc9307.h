#ifndef _GC9307_H
#define _GC9307_H

#include "tft.h"

class TFT_GC9307: public TFT {
  public:
    TFT_GC9307(int width, int height, int data_width, cbs_t cbs);
    void init(void);
    void sleep(void);
    void set_window(int x1, int y1, int x2, int y2);

  private:
    // data members

};

#endif
