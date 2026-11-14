#ifndef _ZTASK_H
#define _ZTASK_H

#include <cstdint>
#include <memory>

class ZTASK {
  public:

    typedef void (*func_t)(void);
    typedef struct {
        func_t func;
        uint32_t timeout, repeat;
        bool en;
    } task_t;

    ZTASK(int max_tasks)
    {
      _tasks = std::make_unique<task_t[]>(max_tasks);
      _max_tasks = max_tasks;
      _millis_f = nullptr;
    }
    ZTASK(int max_tasks, uint32_t (*millis_f)(void)) : ZTASK(max_tasks)
    {
      _millis_f = millis_f;
    }

    void poll(void)    // should be called in main loop
    {
      int i;
      if(_millis_f != NULL)
        _ticks = _millis_f();
      for(i = 0; i < _num_tasks; i++) {
        if(_ticks >= _tasks[i].timeout) {
          _tasks[i].timeout = _ticks + _tasks[i].repeat;
          if(_tasks[i].en)
            _tasks[i].func();
        }
      }
    }
    void tick(void)    // should be called in systick_irqhandler, do not need it if millis is used
    {
      _ticks++;
    }
    int bind(func_t func, int repeat, bool en)    // timeout: repeat inteval;   en: start immediately or not
    {
      if(_num_tasks < _max_tasks) {
        _tasks[_num_tasks].func = func;
        _tasks[_num_tasks].repeat = repeat;
        _tasks[_num_tasks].timeout = 0;
        _tasks[_num_tasks].en = en;
        return _num_tasks++;
      }
      else
        return -1;
    }
    void start(int tid)
    {
      if(tid < _max_tasks)
        _tasks[tid].en = true;
    }
    void stop(int tid)
    {
      if(tid < _max_tasks)
        _tasks[tid].en = false;
    }

  private:
    std::unique_ptr<task_t[]> _tasks;
    uint32_t _ticks;
    int _num_tasks, _max_tasks;
    uint32_t (*_millis_f)(void);
};

#endif
