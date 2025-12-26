#include "zpid.h"
#include <cmath>

float ZPID::do_pid(float sp, float pv)
{
  float err = sp - pv;
  _sum_err += err;
  float d = err - _prev_err;
  _prev_err = err;
  float b;
  if(fabs(err) > 2 * _pid.lime) {    //  误差超过2倍lime时为PD模式, 并且不记录累积误差
    b = 0;
    _sum_err = 0;
  }
  else if(fabs(err) < _pid.lime)    // 误差小于lime时为PID模式
    b = 1;
  else
    b = (2 * _pid.lime - fabs(err)) / _pid.lime;    // 误差在lime和2倍lime之间时为PD和PID混合
  return _pid.p * err + _pid.i * _sum_err * b + _pid.d * d;
}

ZPID::PID ZPID::classic_tune(float A, int tu, int output_min, int output_max)    // 经典的pid整定公式， 具体参数还可以进一步打磨
{
  PID pid = {0};
  float ku = 4 * ((output_max - output_min) / 2) / 3.14 / A;
  pid.p = ku / 2.2;
  pid.i = 1.2 * pid.p / tu;
  pid.d = pid.p * tu / 8;
  pid.lime = A;
  return pid;
}

int ZPID::auto_tune(void)
{
  float temp = _cbs.get_temp_f();
  int output = (temp > _sp) ? _output_min : _output_max;    // 开关模式控温
  tuning.t_min = temp < tuning.t_min ? temp : tuning.t_min;
  tuning.t_max = temp > tuning.t_max ? temp : tuning.t_max;

  if(tuning.prev_output != output && output > (_output_min + _output_max) / 2) {    // 记录每周期的温度最小和最大值
    tuning.min[tuning.cycle_count] = tuning.t_min;
    tuning.t_min = temp;
    tuning.max[tuning.cycle_count] = tuning.t_max;
    tuning.t_max = temp;
    tuning.period[tuning.cycle_count] = _count;
    tuning.cycle_count++;
  }
  tuning.prev_output = output;

  if(tuning.cycle_count > 6) {    // 满6个周期后, 取第3,4,5周期的平均振幅和平均周期
    float sum_diff = 0;
    int sum_period = 0;
    for(int i = 3; i <= 5; i++) {
      sum_diff += (tuning.max[i] - tuning.min[i]);
      sum_period += (tuning.period[i] - tuning.period[i - 1]);
    }
    _pid = classic_tune(sum_diff / 6, sum_period / 3, _output_min, _output_max);
    _mode = MODE::PID;
  }
  return output;
}

void ZPID::poll(void)
{
  switch(_mode) {
    case MODE::OCP:    // 出现OCP事件时把输出调至最小, 等待_ocp_count个周期后切换回原模式
      _output = _output_min;
      if(_ocp_count > 0)
        _ocp_count--;
      else {
        _mode = _prev_mode;
        _ocp_count = _default_ocp_count;
      }
      break;
    case MODE::STOP:
      _output = _output_min;
      break;
    case MODE::AUTO_TUNE:
      _output = auto_tune();
      break;
    case MODE::PID:
      _output = do_pid(_sp, _cbs.get_temp_f()) + (_output_min + _output_max) / 2;
      _output = _output < _output_min ? _output_min : _output;
      _output = _output > _output_max ? _output_max : _output;
      break;
  }
  _cbs.output_f(_output);
  _count++;
}
