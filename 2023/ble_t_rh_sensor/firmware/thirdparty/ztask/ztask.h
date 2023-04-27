#ifndef _ZTASK_H
#define _ZTASK_H

typedef void (*zt_func_t)(void);

typedef struct {
    zt_func_t func;
    unsigned long timeout, repeat;
    int en;
} zt_task_t;

class ztask {
public:
    // apis
    ztask(int max_tasks)
    {
        _tasks = new zt_task_t[max_tasks];
        _max_tasks = max_tasks;
    }
    ~ztask(void)
    {
        delete _tasks;
    }
    void poll(void)    // should be called in main loop
    {
        int i;
        for(i = 0; i < _num_tasks; i++) {
            if(_ticks >= _tasks[i].timeout) {
                _tasks[i].timeout = _ticks + _tasks[i].repeat;
                if(_tasks[i].en)
                    _tasks[i].func();
            }
        }
    }
    void tick(void)    // should be called in systick_irqhandler
    {
        _ticks++;
    }
    int bind(zt_func_t func, int repeat, int en)    // timeout: repeat inteval;   en: start immediately or not
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
            _tasks[tid].en = 1;
    }
    void stop(int tid)
    {
        if(tid < _max_tasks)
            _tasks[tid].en = 0;
    }

    // static variables

private:
    zt_task_t* _tasks;
    unsigned long _ticks;
    int _num_tasks, _max_tasks;
};

#endif
