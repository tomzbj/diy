#ifndef _ZDDS_H
#define _ZDDS_H

#include <cstdint>

class ZDDS {
  public:
    typedef struct {
        void (*start_f)(uint16_t* addr, int count);
        void (*stop_f)(void);
    } cbs_t;

    void set_sample_rate(uint32_t sample_rate)
    {
      _sample_rate = sample_rate;
      _step = 1;
    }
    void set_freq(uint32_t freq)
    {
      _step = (uint32_t)(freq * 4294967296.0 / _sample_rate);
    }
    void set_freq(uint32_t freq, uint32_t freq2)
    {
      _step = (uint32_t)(freq * 4294967296.0 / _sample_rate);
      _step2 = (uint32_t)(freq2 * 4294967296.0 / _sample_rate);
    }

    ZDDS(const cbs_t& cbs) : _cbs(cbs)
    {
    }
    void start(void);
    void stop(void);

    void dma_ftf(void);

  private:
    uint16_t _dma_tab[256] = {0};
    uint32_t _step = 1000000UL, _step2 = 0UL, _sample_rate, _counter = 0, _counter2 = 0;
    float _amp = 1.0, _ofs = 0.0;

    void gen_tab(void);
    const cbs_t& _cbs;
};

#endif
