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
        _cbs.action_f(i, CLICK);
        _key_state[i] = STATE_PRESSED;
        _key_count[i] = 0;
      }
      if(_cont & (1 << i)) {
        _key_count[i]++;
        if(_key_count[i] > _longpress_count) {
          _key_state[i] = STATE_LONGPRESSED;
        }
      }
    }
    else {
      if(_key_state[i] == STATE_PRESSED) {
        _cbs.action_f(i, PRESS);
        _key_state[i] = STATE_NONE;
        _key_count[i] = 0;
      }
      if(_key_state[i] == STATE_LONGPRESSED) {
        _cbs.action_f(i, LONGPRESS);
        _key_state[i] = STATE_NONE;
        _key_count[i] = 0;
      }
    }
  }
}
