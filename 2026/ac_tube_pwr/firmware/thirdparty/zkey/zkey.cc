/* Author: tomzbj  Updated: 2026-10-5 Rev: 0.05 */

#include "zkey.h"

void ZKEY::post(int key_id, int action)
{
  evt_t evt = {(uint8_t)key_id, (uint8_t)action};
  _fifo.put(&evt, sizeof(evt));
}

int ZKEY::bind(int action, void (*f)(int key_id))
{
  if(action <= NONE || action >= ACTION_NUM)
    return -1;
  _handler[action] = f;
  return 0;
}

void ZKEY::poll(void)
{
  evt_t evt;
  while(_fifo.len() >= sizeof(evt_t)) {
    _fifo.get(&evt, sizeof(evt_t));
    if(evt.action < ACTION_NUM && _handler[evt.action] != nullptr)
      _handler[evt.action](evt.key_id);
  }
}

void ZKEY::scan(void)
{
  int i;
  uint16_t key = 0;
  for(i = 0; i < _key_num; i++) {
    if(_read_f(i))
      key |= (uint16_t)(1u << i);
  }
  _trig = key & (key ^ _cont);
  _cont = key;

  for(i = 0; i < _key_num; i++) {
    uint16_t bit = (uint16_t)(1u << i);
    if(key & bit) {
      if(_trig & bit) {
        post(i, CLICK);
        _key_state[i] = STATE::PRESSED;
        _key_count[i] = 0;
      }
      _key_count[i]++;
      if(_key_count[i] > _longpress_count)
        _key_state[i] = STATE::LONGPRESSED;
    }
    else {
      if(_key_state[i] == STATE::PRESSED) {
        post(i, PRESS);
        _key_state[i] = STATE::NONE;
      }
      if(_key_state[i] == STATE::LONGPRESSED) {
        post(i, LONGPRESS);
        _key_state[i] = STATE::NONE;
      }
      _key_count[i] = 0;
    }
  }
}
