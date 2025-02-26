/* Author: tomzbj  Updated: 2024-05-29 Rev: 0.01 */

#ifndef _ZKEY_H
#define _ZKEY_H

#include <cstdint>

class ZKey {
  public:
    enum {
      NONE, CLICK, PRESS, LONGPRESS
    };

    typedef struct {
        int (*read_f)(int key_id);
        void (*action_f)(int key_id, int action);
    } cbs_t;

    ZKey(cbs_t& cbs, int key_num, int longpress_count, int* key_count,
      int* key_state) : _cbs(cbs), _key_num(key_num), _longpress_count(
      longpress_count), _key_count(key_count), _key_state(key_state)
    {
    }
    void poll(void);

  private:
    enum {
      STATE_NONE, STATE_PRESSED, STATE_LONGPRESSED
    };

    cbs_t& _cbs;
    uint16_t _trig, _cont;
    int _key_num, _longpress_count;
    int* _key_count;
    int* _key_state;
};

#endif
