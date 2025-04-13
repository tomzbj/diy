/** @file docFundamentals.h
<b> Introduction </b> @n 
cocoOS is a free, open source, cooperative task scheduler, based on coroutines. @n

Task procedures scheduled by cooperative kernels are so called RTC's, run to completion tasks.
They execute from the first line of the procedure to the last line. The use of coroutines enables 
us to implement task procedures that does not have to execute all the way down to the last line of
code. Instead execution can end in the middle e.g waiting for a semaphore to be released. 
When execution is resumed, it continues at that line. @n @n 

With coroutines, this can be done without having to save the complete task context when switching
to another task.
Also, task procedures can be outlined in the same style as when using a traditional preemptive RTOS.

cocoOS is extremely portable to any target which makes it a perfect choice during early phases of 
the development when which OS to use is still an open issue.


<b>Getting Started </b>@n

<b>Configuration </b>@n
To setup cocoOS you have to define 4 constants in os_defines.h. These are: @n
    - N_TASKS: maximum number of tasks
    - N_QUEUES: maxmimum number of message queues
    - N_SEMAPHORES: maximum number of semaphores
    - N_EVENTS: maximum numer of events

Allowed range for these constants is 0-254. These constants sets the maximum value for the number of
used ojects of the specified types. It is allowed to use fewer but not more number of objects. To save
RAM it is recommended to keep these values as low as possible. @n

You should also define the Mem_t to the correct size, i.e the address size of your target.
For instance, on a 32 bit system:
typedef uint32_t Mem_t;

<b>System setup - application main function </b> @n
As usual you have to setup your system, ports, clocks etc in the beginning of your main function. 
After setting up the system, you start the OS setup by calling os_init() and then create all tasks, 
semaphores and events. Finally you places a call to os_start() and the application starts.
@n @n
@code
int main(void) {
  /* Setup ports, clock... */
  system_init();
  
  os_init();
  
  /* Create kernel objects */
  task_create( myTaskProc, 0, 1, NULL, 0, 0 );
  mySem = sem_bin_create( 1 );
  
  os_start();
  
  /* Will never end up here */
  return 0;
}
@endcode


This is the preferred order of initialization. It is crucial that the clock tick is not started before the tasks
are created and the kernel is initialized. The os_start() calls the macro os_enable_interrupts() which can be defined
to start the clock tick driving the OS.  

<b>Time </b>@n
cocoOS keeps track of time by counting ticks and you must feed the counting with a call to os_tick() periodically
from the clock tick ISR.

A system of one main clock and several sub clocks is used in cocoOS. The main clock is typically realized 
using one of the hardware timers within your target microcontroller which calls the os_tick() function that
decrements the timers used for task_wait(), msg_post_in() and msg_post_every().
If your application does not need more than a single time base, the main clock fed by the os_tick() call is
all you need. @n

In case your application has to react to events in another time resolution than what is provided by the main
clock, you can use the sub clocks. The sub clocks are typically not associated with a hardware timer, but is
instead "ticked" by calling os_sub_tick(id) from within your application code.

Sub clocks can also be advanced with a step greater than one using os_sub_nTick(id, nTicks). There is one
task function associated with the sub clocks: task_wait_id(id,ticks). id is a value in the range 1-255
assigned by the application, and ticks is the number of ticks to wait. An example of the use of sub clocks
could be a task that should be run after 64 bytes has been received on the UART. The task starts the wait by
calling task_wait_id( 1, 64 ). And in the UART rx ISR os_sub_tick(1) is called for each received character.

<b>Tasks </b>@n
An application is built up by a number of tasks. Each task is a associated with a (preferably short) 
procedure with a well defined purpose. The execution of the tasks, is managed by the os kernel, by letting
the highest priority task among all execution ready tasks to execute. All tasks have to make at least
one blocking call to a sheduling kernel function. This gives lower priority tasks a chance to execute.
 
The task procedure must enclose its code with the task_open() and task_close() macros, as shown below. 
Several tasks can use the same task procedure. This is done by giving the same procedure as argument when
creating the tasks.

@code
static void task(void) {
  task_open();
  ...
  ...
  task_close();
}
@endcode

Such a task will be executed once only. If a task is intended to be executed "for ever", an endless loop
must be implemented.

@code
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
  
  /* Create kernel objects */
  task_create( hello_task, 0, 1, NULL, 0, 0 );
  
  os_init();
  clock_start();
  os_start();
  
  /* Will never end up here */
  return 0;
}
@endcode

<b>Scheduling</b>@n
When a task has finished it gives the CPU control to another task by calling one of the scheduling 
macros:
 
    - task_wait()
    - event_wait()
    - event_wait_timeout()
    - event_wait_multiple()
    - event_signal()
    - sem_wait()
    - sem_signal()
    - msg_post()
    - msg_post_in()
    - msg_post_every()	
    - msg_receive()
    
Normally the scheduler will give the cpu to the highest priority task ready for execution. It is possible to 
choose a round robin scheduling algorithm by putting the following line in os_defines.h:@n
#define ROUND_ROBIN

This will make the scheduler to scan the list of tasks and run the next found task in the ready state.
*/
