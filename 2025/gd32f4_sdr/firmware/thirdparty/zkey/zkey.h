/* Author: tomzbj  Updated: 2024-05-29 Rev: 0.01 */

#ifndef _ZKEY_H
#define _ZKEY_H

#include <cstdint>
#include <memory>

class ZKey {
  public:
    enum {
      NONE, CLICK, PRESS, LONGPRESS, DOUBLECLICK
    };

    typedef struct {
        int (*read_f)(int key_id);
        void (*action_f)(int key_id, int action);
    } cbs_t;

    ZKey(cbs_t& cbs, int key_num, int longpress_count, int doubleclick_threshold) :    //
    _cbs(cbs), _key_num(key_num), _longpress_count(longpress_count), _doubleclick_threshold(
      doubleclick_threshold)
    {
      _key_count = std::make_unique<int[]>(key_num);
      _key_state = std::make_unique<int[]>(key_num);
      _waiting_for_double = std::make_unique<int[]>(key_num);
      _last_release_time = std::make_unique<int[]>(key_num);
    }
    void poll(void);

  private:
    enum {
      STATE_NONE, STATE_PRESSED, STATE_LONGPRESSED
    };

    cbs_t& _cbs;
    uint16_t _trig, _cont;
    int _key_num, _longpress_count, _poll_count, _doubleclick_threshold;
    std::unique_ptr<int[]> _key_count, _key_state, _waiting_for_double,
      _last_release_time;
};

#endif
