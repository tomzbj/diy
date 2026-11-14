/* Author: tomzbj  Updated: 2026-10-5 Rev: 0.05 */

#ifndef _ZKEY_H
#define _ZKEY_H

#include "zfifo.h"
#include <cstdint>
#include <memory>

class ZKEY {
  public:
    enum {
      NONE, CLICK, PRESS, LONGPRESS, ACTION_NUM
    };

    ZKEY(int (*read_f)(int key_id), int key_num, int longpress_count,
      int fifo_size = 16) :    //
      _fifo(fifo_size), _read_f(read_f), _key_num(key_num), _longpress_count(
        longpress_count), _trig(0), _cont(0)
    {
      _key_count = std::make_unique<int[]>(key_num);
      _key_state = std::make_unique<STATE[]>(key_num);
      for(int i = 0; i < ACTION_NUM; i++)
        _handler[i] = nullptr;
    }
    void scan(void); /* scan keys, enqueue (call from 1ms task) */
    void poll(void); /* drain queue (call from main loop) */
    int bind(int action, void (*f)(int key_id));

  private:
    enum class STATE {
      NONE, PRESSED, LONGPRESSED
    };

    struct evt_t {
        uint8_t key_id;
        uint8_t action;
    };

    void post(int key_id, int action);

    ZFIFO _fifo;
    int (*_read_f)(int key_id);
    void (*_handler[ACTION_NUM])(int key_id);
    int _key_num, _longpress_count;
    uint16_t _trig, _cont;
    std::unique_ptr<int[]> _key_count;
    std::unique_ptr<STATE[]> _key_state;
};

#endif
