#ifndef _ZPID_H
#define _ZPID_H

#include <cstdint>
#include <cmath>

class ZPID {
  public:

    struct CBS {
        float (*get_temp_f)(void);    // 读取温度回调函数
        void (*output_f)(int val);    // pwm输出回调函数
    };
    enum class MODE {
      STOP, AUTO_TUNE, PID, OCP
    };
    struct PID {
        float p, i, d, lime;
    };

    ZPID(const CBS& cbs, int output_min, int output_max, int default_ocp_count) :    //
      _cbs(cbs), _count(0), _output_min(output_min), _output_max(output_max),
      _default_ocp_count(default_ocp_count)
    {
    }

    void set_pid(PID pid)
    {
      _pid = pid;
    }

    PID get_pid(void)
    {
      return _pid;
    }

    void set_sp(float sp)
    {
      _sp = sp;
    }
    void set_mode(MODE mode)
    {
      _mode = mode;
      if(_mode == MODE::AUTO_TUNE)
        auto_tune_init();
    }
    void ocp(void)
    {
      set_mode(MODE::OCP);
    }
    MODE get_mode(void)
    {
      return _mode;
    }

    float do_pid(float sp, float pv);
    void poll(void);
    void auto_tune_init(void)    // 自整定初始化
    {
      tuning.cycle_count = 0;
      tuning.t_max = -MAXFLOAT;
      tuning.t_min = MAXFLOAT;
    }
    int get_output(void)
    {
      return _output;
    }

  private:
    const CBS& _cbs;
    PID _pid;
    float _sum_err, _prev_err, _sp;
    MODE _mode, _prev_mode;
    int _count, _output, _output_min, _output_max, _ocp_count, _default_ocp_count;

    struct {
        int cycle_count;
        float max[8], min[8];
        int period[8];
        int prev_output;
        float t_min, t_max;
    } tuning;    //  用于自整定

    int auto_tune(void);
    PID classic_tune(float sum_diff, int sum_period, int output_min, int output_max);
};

#endif
