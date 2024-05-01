#ifndef _RTC_HAL_H
#define _RTC_HAL_H

class CRTC {
public:
    CRTC(void)
    {
    }
    void init(void);
    void set_time(int h, int m, int s);
    void get_time(int* h, int* m, int* s);
    void show_calendar(void);
private:
    RTC_HandleTypeDef _hrtc;
};

#endif
