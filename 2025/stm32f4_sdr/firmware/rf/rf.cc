#include "misc.h"
#include "platform.h"
#include "rf.h"
#include <cmath>

int __errno = 0;

void RF::play(void)
{
  if(_audio_q.empty())
    return;
//  DAC1->DHR12R1 = audio_q.front();
  DAC_OUT0_R12DH (DAC0) = _audio_q.front();
  _audio_q.pop();

  if(0) {
    static int count = 0;
    ++count %= 50000;
    if(!count)
      printf("play\n");
  }
}

void RF::poll(void)
{
  //  16384 sample & 6.4Msps, 192M SYSCLK: 491520 cycles
  if(adc_updated_ == false)
    return;
  adc_updated_ = false;
  volatile int t1 = DWT->CYCCNT, t0 = t1, t;

  static int32_t offset = 2048 << 16;
  static auto workbuf_i = _workbuf_i.get();
  static auto workbuf_q = _workbuf_q.get();
  static auto dacbuf = _workbuf_i.get();

  if(flag_view_rf == true) {
    for(int i = 0; i < _adc_buf_size; i++) {
      printf("%d\n", ((int16_t*)_cur_data)[i]);
    }
    flag_view_rf = false;
    return;
  }

  _ofs.offset(offset, _adc_buf_size, (int16_t*)_cur_data, (int16_t*)_cur_data);
  ( {t = DWT->CYCCNT; stat.time_dcr = t - t1; t1 = t;});
  _dec_i.mix_cic(_nco_i, _adc_buf_size, (int16_t*)_cur_data, workbuf_i);
  _dec_q.mix_cic(_nco_q, _adc_buf_size, (int16_t*)_cur_data, workbuf_q);
  ( {t = DWT->CYCCNT; stat.time_cic = t - t1; t1 = t;});

  auto WRK_BUF_SIZE = _adc_buf_size / 32;
  auto DAC_BUF_SIZE = WRK_BUF_SIZE / 4;

  _dec_i.fir(workbuf_i, workbuf_i, WRK_BUF_SIZE);
  _dec_q.fir(workbuf_q, workbuf_q, WRK_BUF_SIZE);
  ( {t = DWT->CYCCNT; stat.time_fir = t - t1; t1 = t;});
  _dec_i.bpf_iir(workbuf_i, workbuf_i, config.bpf, DAC_BUF_SIZE);
  _dec_q.bpf_iir(workbuf_q, workbuf_q, config.bpf, DAC_BUF_SIZE);
  ( {t = DWT->CYCCNT; stat.time_bpf = t - t1; t1 = t;});

  _dem.am(0.95, DAC_BUF_SIZE, workbuf_i, workbuf_q, dacbuf);
  ( {t = DWT->CYCCNT; stat.time_dem= t-t1; t1 = t;});
// agc
  _dem.agc_config(0.9995f, 0.001f);
  _dem.agc(dacbuf, dacbuf, DAC_BUF_SIZE);

// dac
  for(int i = 0; i < DAC_BUF_SIZE; i++) {
    if(_audio_q.size() < 128)
      _audio_q.push((1 << config.volume) * dacbuf[i] + 2048);
  }

  ( {t = DWT->CYCCNT; stat.time_all= t-t0;});
}

void RF::tune(uint32_t freq)
{
  config.freq = freq;
  MIX_DECIMATOR::nco_gen(config.freq, _nco_i, _nco_q, _adc_buf_size, _sample_rate);
}

void RF::init(void)
{
  config.freq = 639000;
  config.volume = 9;
  config.bpf = 4;
  MIX_DECIMATOR::nco_gen(config.freq, _nco_i, _nco_q, _adc_buf_size, _sample_rate);
}
