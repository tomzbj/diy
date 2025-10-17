#ifndef _RF_H
#define _RF_H

#include <queue>
#include <memory>

#include "dc_block.h"
#include "algorithm.h"

class RF {
  public:

    void poll(void);
    RF(int sample_rate, int batch_size, uint32_t nco_base) :    //, float* workbuf_i, float* workbuf_q) :    //
//    RF(int sample_rate, int batch_size, int32_t* workbuf_i, int32_t* workbuf_q) :    //
    _sample_rate(sample_rate), _adc_buf_size(batch_size)    //, _dcb(4000, 12)    //, workbuf_i_(workbuf_i), workbuf_q_( workbuf_q)
    {
      _workbuf_i = std::make_unique<float[]>(batch_size / 32);
      _workbuf_q = std::make_unique<float[]>(batch_size / 32);
      _nco_i = (int16_t*)nco_base;
      _nco_q = (int16_t*)(nco_base + batch_size * sizeof(int16_t));
    }
    void init(void);
    void feed(void* ptr)
    {
      _cur_data = ptr;
      adc_updated_ = true;
    }

    void view_nco(void)
    {
      for(int i = 0; i < _adc_buf_size; i++) {
        printf("%6d: %6d %6d\n", i, _nco_i[i], _nco_q[i]);
      }
    }
    void play(void);
    void view_stat(void)
    {
      auto ratio = rcu_clock_freq_get(CK_SYS) / 1000000UL;
      printf("dcr: %8lu cycles, %8lu us\n", stat.time_dcr, stat.time_dcr / ratio);
      printf("cic: %8lu cycles, %8lu us\n", stat.time_cic, stat.time_cic / ratio);
      printf("fir: %8lu cycles, %8lu us\n", stat.time_fir, stat.time_fir / ratio);
      printf("bpf: %8lu cycles, %8lu us\n", stat.time_bpf, stat.time_bpf / ratio);
      printf("dem: %8lu cycles, %8lu us\n", stat.time_dem, stat.time_dem / ratio);
      printf("all: %8lu cycles, %8lu us\n", stat.time_all, stat.time_all / ratio);
      printf("dac_queue_size: %d\n", _audio_q.size());
      printf("\n");
    }
    void view_config(void)
    {
      printf("freq:   %-d\n", config.freq);
      printf("bpf:    %-d\n", config.bpf);
      printf("volume: %-d\n", config.volume);
    }
    void set_volume(int volume)
    {
      config.volume = volume;
    }
    void set_bpf(int bpf)
    {
      config.bpf = bpf;
    }
    void tune(uint32_t freq);
    void view_rf(void)
    {
      flag_view_rf = true;
    }

  private:
    const int _sample_rate, _adc_buf_size;
    volatile void* _cur_data;
    bool adc_updated_ = false;
    bool flag_view_rf = false;
    uint32_t _inteval, _inteval2;

    int16_t* _nco_i;
    int16_t* _nco_q;

    MIX_DECIMATOR _dec_i, _dec_q;
    DEMODULATOR _dem;
    OFFSET _ofs;

    std::unique_ptr<float[]> _workbuf_i, _workbuf_q;
    std::queue<uint16_t> _audio_q;
    int _volume = 8;
    struct {
        uint32_t time_dcr;
        uint32_t time_cic;
        uint32_t time_fir;
        uint32_t time_bpf;
        uint32_t time_dem;
        uint32_t freq_fft;
        uint32_t time_all;
    } stat;
    struct {
//        int ui;
//        int cursor;
        int freq;
//        int demodulation;
        int bpf;
        int volume;
//        int fft_gain;
//        int fft_delay;
    } config;

    void nco_gen(float freq);
};
#endif
