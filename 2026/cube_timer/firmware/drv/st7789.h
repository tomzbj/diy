#ifndef _ST7789_H
#define _ST7789_H

#include "tft.h"

class TFT_ST7789: public TFT {
public:
    TFT_ST7789(int width, int height, int data_width, cbs_t cbs);
    void init(void);
    void sleep(void);
    void set_window(int x1, int y1, int x2, int y2);
    void set_dir(int dir);

private:
    int _dir;
    // data members

};

#endif
