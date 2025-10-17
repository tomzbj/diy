# cocoOS
A cooperative operating system based on coroutines

cocoOS is a free, open source, cooperative task scheduler, based on coroutines targeted for embedded microcontrollers like AVR, MSP430 and STM32.


Task procedures scheduled by cooperative kernels are so called RTC's, run to completion tasks. They execute from the first line of the procedure to the last line. 

The use of coroutines enables us to implement task procedures that does not have to execute all the way down to the last line of code. Instead execution can end in the middle e.g waiting for a semaphore to be released. When execution is resumed, it continues at that line. 

With coroutines, this can be done without having to save the complete task context when switching to another task.

 

Also, task procedures can be outlined in the same style as when using a traditional preemptive RTOS.

 

cocoOS is extremely portable to any target which makes it a perfect choice during early phases in the development when which OS to use is still an open issue. You can even run it under Linux or Windows.

# Getting Started

 

## Configuration

To setup cocoOS you have to define 6 macros. They can be setup in os_defines.h, or as defined as compiler flags, e.g. `-DN_TASKS=2...` If not defines as flags, they will be set to default values according to the list below:

    - N_TASKS: maximum number of tasks            (0-254, default=1)
    - N_QUEUES: maxmimum number of message queues (0-254, default=0)
    - N_SEMAPHORES: maximum number of semaphores  (0-254, default=0)
    - N_EVENTS: maximum number of events          (0-254, default=0)
    - ROUND_ROBIN: should round robin scheduling be used ? (0)
    - Mem_t: address type, e.g. uint32_t          (uint32_t)

Asserts will fire if the maximum numbers are violated during runtime. 

To save RAM it is recommended to keep these values as low as possible.
 

## System setup - application main function

As usual you have to setup your system, ports, clocks etc in the beginning of your main function. 

Then you setup the cocoOS kernel with a call to os_init() and proceeds with

creating all tasks, semaphores and events.


 
```
int main(void) {
  /* Setup ports, clock... */
  system_init();
  

  /* Initialize cocoOS */

  os_init();

  /* Create kernel objects */
  task_create( taskProc, &taskData, 1, NULL, 0, 0 );
  mySem = sem_bin_create( 1 );

 

  os_start();
  
  /* Will never end up here */
  return 0;
}
```
This is the preferred order of initialization. The os_start() function will call os_enable_interrupts() that can be used to enable the clock interrupt driving the os_tick().

 

## Time

cocoOS keeps track of time by counting ticks. You must feed the counting with a call to os_tick() periodically from the clock tick ISR.

 

A system of one main clock and several sub clocks is used in cocoOS. The main clock is controlled by the os_tick() function and decrements the timers used for task_wait(), msg_post_in() and msg_post_every(). If your application does not need more than a single time base, the main clock fed by the os_tick() call is all you need. The main clock is typically realized using one of the hardware timers within your target microcontroller.

 

But if your application has to react to events in another time resolution than what is provided by the main clock, you can use the sub clocks. The sub clocks are typically not associated with a hardware timer, but is instead "ticked" by calling os_sub_tick(id) from within your application code.

Sub clocks can also be advanced with a step greater than one using os_sub_nTick(id, nTicks).

 

There is one task function associated with the sub clocks: task_wait_id(id,ticks). id is a value in the range 1-255 assigned by the application, and ticks is the number of ticks to wait.

 

An example of the use of sub clocks could be a task that should be run after 64 bytes has been received on the UART. The task starts the wait by calling task_wait_id( 1, 64 ). And in the UART rx ISR os_sub_tick(1) is called for each received character.


## Tasks

An application is built up by a number of tasks. Each task is a associated with a (preferably short) procedure with a well defined purpose.

The execution of the tasks, is managed by the os kernel, by letting the highest priority task among all execution ready tasks to execute.

 

All tasks have to make at least one blocking call to a sheduling kernel function. This gives lower priority tasks a chance to execute.
 
The task procedure must enclose its code with the task_open() and task_close() macros, as shown below.

 
```
static void task(void) {
  task_open();
  ...
  ...
  task_close();
}
```

 

Such a task will be executed once only. If a task is intended to be executed "for ever", an endless loop must be implemented:

 
```
static void hello_task(void) {
  task_open();
    for(;;) {
      uart_send_string("Hello World!");
      task_wait( 20 );
    }
  task_close();
}

 

int main(void) {
  /* Setup ports, clock... */
  system_init();
  

  os_init();

  /* Create kernel objects */
  task_create( hello_task, &taskData, 1, NULL, 0, 0 ); 


  os_start();
  
  /* Will never end up here */
  return 0;
}
```

 

## Scheduling
When a task has finished it gives the CPU control to another task by calling one of the scheduling macros:
 
    - task_wait()

    - task_wait_id()
    - event_wait()

    - event_wait_timeout()
    - event_wait_multiple()
    - event_signal()
    - sem_wait()
    - sem_signal()
    - msg_q_get()
    - msg_q_give()

    - msg_post()

    - msg_post_in()

    - msg_post_every()

    - msg_receive()


Normally the scheduler will give the cpu to the highest priority task ready for execution. It is possible to choose a round robin scheduling algorithm by putting the following line in os_defines.h:


## ROUND_ROBIN
When round robin is used, the scheduler to scan the list of tasks and run the next found task in the ready state ignoring the prio level of the tasks.

## event_wait() and event_wait_ex()
A task can be set to wait for an event to be signaled, by calling event_wait(). If the event is signaled from another task, everything works as expected: when the event is signaled, the waiting task is ready for execution again.
```
void task() {
  task_open();
  for (;;) {
    event_wait(anEvt);
    do_something();
  }
  task_close();
```

However, if the event is signaled from an interrupt, a race condition can occur during execution of event_wait() which can result in an inconsistent state of the task. The task will wait forever and never receive the event. This can happen if the interrupt is enabled and gets triggered during the event_wait() call.

The solution is to do the following:
- disable the interrupt from start, and also in the isr, so it is not automatically re-enabled.
- use event_wait_ex() instead and provide it with a callback
- in the callback: enable the interrupt

```
void myCallback(void) {
  doStuffBeforeWait(); // this is a perfect place to e.g turn off radio power before entering wait
  enableEventSignalingInterrupt();
}

void task() {
  task_open();
  for(;;) {
    event_wait_ex(anEvt, myCallback);
    do_something();
  }
  task_close();
}
```


