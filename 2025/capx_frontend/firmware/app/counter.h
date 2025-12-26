#ifndef _COUNTER_H
#define _COUNTER_H

class COUNTER {
  public:
    uint32_t get(void)
    {
      return _freq;
    }
    void carry(void)
    {
      _counter += 65536;
    }

    void update(void)
    {
      _freq = _counter + TIM1->CNT;
      _counter = 0;
      TIM1->CNT = 0;
    }

  private:
    uint32_t _counter, _freq;
};
#endif
