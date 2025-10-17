/* Author: tomzbj  Updated: 2024-05-29 Rev: 0.03 */

#include "zkey.h"

void ZKey::poll(void)
{
  int i;
  uint16_t key = 0;
  for(i = 0; i < _key_num; i++) {
    if(_cbs.read_f(i))
      key |= (1 << i);
  }
  _trig = key & (key ^ _cont);
  _cont = key;

  for(i = 0; i < _key_num; i++) {
    if(_cbs.read_f(i)) {
      if(_trig & (1 << i)) {
        if(_waiting_for_double[i]
          && (_poll_count - _last_release_time[i] < _doubleclick_threshold)) {
          // 检测到双击
          _cbs.action_f(i, DOUBLECLICK);    // 新增双击回调类型
          _waiting_for_double[i] = false;
          _key_state[i] = STATE_NONE;
        }
        else {
          // 第一次按下
          _cbs.action_f(i, CLICK);
          _key_state[i] = STATE_PRESSED;
        }
//        _cbs.action_f(i, CLICK);
//        _key_state[i] = STATE_PRESSED;
//        _key_count[i] = 0;
      }
      if(_cont & (1 << i)) {
        _key_count[i]++;
        if(_key_count[i] > _longpress_count) {
          _key_state[i] = STATE_LONGPRESSED;
          _waiting_for_double[i] = false;
        }
      }
    }
    else {
      if(_key_state[i] == STATE_PRESSED) {
        _cbs.action_f(i, PRESS);
        _key_state[i] = STATE_NONE;
        _last_release_time[i] = _poll_count;    // 记录释放时间
        _waiting_for_double[i] = true;      // 进入等待双击状态
      }
      if(_key_state[i] == STATE_LONGPRESSED) {
        _cbs.action_f(i, LONGPRESS);
        _key_state[i] = STATE_NONE;
        _waiting_for_double[i] = false;     // 长按释放不触发双击

      }
      _key_count[i] = 0;
    }
  }
  _poll_count++;
}
