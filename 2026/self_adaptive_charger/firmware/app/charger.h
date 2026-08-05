#ifndef _CHARGER_H
#define _CHARGER_H

class CHARGER {
  public:
    struct CBS {
        void (*adc_get_f)(float* volt, float* curr);
        void (*set_voltcurr_f)(float volt, float curr);
    };
    CHARGER(const CBS& cbs) :
      _cbs(cbs), _count(0), _stat_str("---")
    {
    }
    void poll(void);
    const char* get_stat_str(void)
    {
      return _stat_str ? _stat_str : "---";
    }

  private:
    CBS _cbs;
    int _count;
    const char* _stat_str;
    void adjust(void);
};
#endif
