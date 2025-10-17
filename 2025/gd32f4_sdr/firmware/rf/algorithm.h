#include <cstdint>
#include <cstdio>
#include <cmath>

class OFFSET {
  public:

    void offset(int32_t& ofst, int size, const int16_t* inp, int16_t* out)
    {
      const uint32_t* inp32 = (const uint32_t*)inp;
      uint32_t* out32 = (uint32_t*)out;
      uint32_t bias = ofst >> 16;
      bias |= bias << 16;
      int32_t sum = 0;
      for(int i = size / 8 / 2; i != 0; i--) {
        uint32_t acc[8];
        for(int j = 0; j < 8; j++)
          acc[j] = *inp32++;
        for(int j = 0; j < 8; j++) {
          sum = __SMLAD(acc[j], 0x00010001, sum);
          acc[j] = __SSUB16(acc[j], bias);
        }
        for(int j = 0; j < 8; j++)
          *out32++ = acc[j];
      }
      ofst -= ofst >> (16 - 11);    // ofst * 2048 / 65536.
      ofst += sum >> (ilog2(size) - 11);    // sum * 2048 / size.
    }
  private:
    int ilog2(int x)
    {
      return sizeof(int) * 8 - __builtin_clz(x) - 1;
    }

};
class MIX_DECIMATOR {
  public:

    static void nco_gen(float freq, int16_t* _nco_i, int16_t* _nco_q, int _batch_size,
      int _sample_rate, float phase = 0.0f)
    {
      int f = (int)((float)freq * _batch_size / _sample_rate + 0.5);
      for(int i = 0; i < _batch_size; i++) {
        const float rad = i * 2.0f * (float)M_PI * f / _batch_size + phase;
        _nco_i[i] = __SSAT((int32_t)(sinf(rad) * 25000.0f), 16);
        _nco_q[i] = __SSAT((int32_t)(cosf(rad) * 25000.0f), 16);
      }
    }

    void mix_cic(const int16_t* tab, int size, const int16_t* inp, float* out)
    {
      int32_t intg0 = _cic_state[0], intg1 = _cic_state[1], intg2 = _cic_state[2];
      int32_t comb0 = _cic_state[3], comb1 = _cic_state[4], comb2 = _cic_state[5];
      const uint32_t* tab32 = (uint32_t*)tab;
      const uint32_t* inp32 = (uint32_t*)inp;
      for(int i = size / 32; i != 0; i--) {
        for(int j = 0; j < 16; j++) {
          //      for(int k = 0; k < 16; k++) {
          const uint32_t tabv = *tab32++;
          const uint32_t inpv = *inp32++;
          const int32_t sgnll = __SMULBB(tabv, inpv) >> 11;
          const int32_t sgnlh = __SMULTT(tabv, inpv) >> 11;
          intg0 += sgnll;
          intg1 += intg0;
          intg2 += intg1;
          intg0 += sgnlh;
          intg1 += intg0;
          intg2 += intg1;
          //      }
        }
        int32_t accm0, accm1, accm2;
        accm0 = intg2 - comb0;
        comb0 = intg2;
        accm1 = accm0 - comb1;
        comb1 = accm0;
        accm2 = accm1 - comb2;
        comb2 = accm1;
        *out++ = accm2 / 1073741824.0f;
      }
      _cic_state[0] = intg0;
      _cic_state[1] = intg1;
      _cic_state[2] = intg2;
      _cic_state[3] = comb0;
      _cic_state[4] = comb1;
      _cic_state[5] = comb2;
    }

    void fir(const float* inp, float* out, int size)
    {
      float stt[32];
      for(int i = 0; i < 32; i++)
        stt[i] = _fir_state[i];
      for(int i = size / 32; i != 0; i--) {
        for(int j = 0; j < 8; j++) {
          for(int k = 0; k < 4; k++)
            stt[j * 4 + k] = *inp++;
          float acc = 0.0f;
          for(int k = 0; k < 16; k++)
            acc += fir_coeff[k]
              * (stt[(j * 4 + 3 - k + 32) % 32] + stt[(j * 4 + 4 + k) % 32]);
          *out++ = acc;
        }
      }
    }

    void bpf_iir(const float* inp, float* out, int coeff_index, int size)
    {
      const float* coeff = bpf_iir_coeff[coeff_index];
      for(int i = 0; i < 3; i++) {
        float* stt = _bpf_iir_state + i * 2;
        for(int j = 0; j < size; j++) {
          float acc = inp[j];
          for(int k = 1; k < 3; k++)
            acc -= coeff[k + 3] * stt[k - 1];
          float res = coeff[0] * acc;
          for(int k = 1; k < 3; k++)
            res += coeff[k] * stt[k - 1];
          stt[1] = stt[0];
          stt[0] = acc;
          out[j] = res;
        }
      }
    }

  private:
    int32_t _cic_state[8];
    float _fir_state[32];
    float _bpf_iir_state[6];

    const float fir_coeff[32] = {
      //
      -1.140811e-03f, -1.890000e-03f, -1.979986e-03f, -2.935298e-04f, +3.893323e-03f,
      +9.027487e-03f, +1.062969e-02f, +3.585712e-03f, -1.312862e-02f, -3.242807e-02f,
      -3.968470e-02f, -1.938192e-02f, +3.497384e-02f, +1.137517e-01f, +1.923674e-01f,
      +2.415355e-01f, +2.415355e-01f, +1.923674e-01f, +1.137517e-01f, +3.497384e-02f,
      -1.938192e-02f, -3.968470e-02f, -3.242807e-02f, -1.312862e-02f, +3.585712e-03f,
      +1.062969e-02f, +9.027487e-03f, +3.893323e-03f, -2.935298e-04f, -1.979986e-03f,
      -1.890000e-03f, -1.140811e-03f, };

    const float bpf_iir_coeff[7][6] = {
    //
      {+1.7631e-4f, +3.5263e-4f, +1.7631e-4f, +1.0000e+0f, -1.9620e+0f, +9.6279e-1f},    //
      {+1.0717e-3f, +2.1435e-3f, +1.0717e-3f, +1.0000e+0f, -1.9052e+0f, +9.0957e-1f},    //
      {+4.0978e-3f, +8.1956e-3f, +4.0978e-3f, +1.0000e+0f, -1.8109e+0f, +8.2732e-1f},    //
      {+1.5042e-2f, +3.0084e-2f, +1.5042e-2f, +1.0000e+0f, -1.6243e+0f, +6.8449e-1f},    //
      {+7.5147e-2f, +1.5029e-1f, +7.5147e-2f, +1.0000e+0f, -1.0890e+0f, +3.8964e-1f},    //
      {+2.2851e-1f, +4.5702e-1f, +2.2851e-1f, +1.0000e+0f, -2.7048e-1f, +1.8453e-1f},    //
      {+7.2112e-1f, +1.4422e+0f, +7.2112e-1f, +1.0000e+0f, +1.3629e+0f, +5.2160e-1f},    //
      };

    static inline int32_t __SMULBB(uint32_t x, uint32_t y)
    {
      int32_t r;
      asm volatile("smulbb %[r], %[x], %[y]" : [r] "=r" (r) : [x] "r" (x), [y] "r" (y) : );
      return r;
    }

    static inline int32_t __SMULTT(uint32_t x, uint32_t y)
    {
      int32_t r;
      asm volatile("smultt %[r], %[x], %[y]" : [r] "=r" (r) : [x] "r" (x), [y] "r" (y) : );
      return r;
    }
};

class DEMODULATOR {
  public:

    void am(float alpha, int size, const float* inp0, const float* inp1, float* out)
    {
      for(int i = 0; i < size; i++) {
        const float ii = inp0[i];
        const float iq = inp1[i];
        const float sig = sqrtf(ii * ii + iq * iq);
        // DC removal filter.
        const float tmp = sig + alpha * _dem_state;
        out[i] = tmp - _dem_state;
        _dem_state = tmp;
      }
    }

    void agc_config(float decay, float threshold)
    {
      _agc_decay = decay;
      _agc_threshold = threshold;
    }
    void agc(const float* inp, float* out, int size)
    {
      if(_agc_state < _agc_threshold)
        _agc_state = _agc_threshold;
      for(int i = 0; i < size; i++) {
        const float level = fabs(inp[i]);
        if(level > _agc_state)
          _agc_state = level;
        out[i] = inp[i] / _agc_state;
        if(_agc_state > _agc_threshold)
          _agc_state *= _agc_decay;
      }
    }
  private:
    float _dem_state, _agc_state;
    float _agc_decay, _agc_threshold;
};
