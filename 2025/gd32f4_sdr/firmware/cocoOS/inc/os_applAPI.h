/*
 * Copyright (c) 2012 Peter Eckstrand
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted (subject to the limitations in the
 * disclaimer below) provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
 * GRANTED BY THIS LICENSE.  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
 * HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This file is part of the cocoOS operating system.
 * Author: Peter Eckstrand <info@cocoos.net>
 */
 

#ifndef _os_applapi_h__
#define _os_applapi_h__

#include "cocoos.h"

#ifdef __cplusplus
extern "C" {
#endif


/** @file os_applAPI.h cocoOS API header file*/
/*********************************************************************************/
/*  task_open()                                                 *//**
*   
*   Macro for definition of task beginning.
*
*   @remarks \b Usage: Should be placed at the beginning of the task procedure @n
* @code 


static void myTask(void) {
 static uint8_t i;
 task_open();	
  ...
  task_wait( 10 );
  ...
 task_close();
}
 @endcode 
 *******************************************************************************/
#define task_open()                 OS_BEGIN


/*********************************************************************************/
/*  task_close()                                                 *//**
*   
*   Macro for definition of task end.
*   @remarks \b Usage: Should be placed at the end of the task procedure @n
* @code 


static void myTask(void) {
 static uint8_t i;
 task_open();	
  ...
  task_wait( 10 );
  ...
 task_close();
}
 @endcode 
 *******************************************************************************/
#define task_close()                OS_END


/*********************************************************************************/
/*  task_wait(x)                                                 *//**
*   
*   Macro for suspending a task a specified amount of ticks of the master clock.
*   When the wait time has expired, the task is ready to execute again and will continue
*   at the next statement when the task is scheduled to run.
*
*   @param x Number of master clock ticks to wait.
*   @remarks \b Usage: @n
* @code 


static void myTask(void) {
 task_open();	
  ...
  task_wait( 10 );
  ...
 task_close();
}
 @endcode 
 *******************************************************************************/
#define task_wait(x)                OS_WAIT_TICKS(x,0)


/*********************************************************************************/
/*  task_wait_id(id,x)                                                 *//**
*   
*   Macro for suspending a task a specified amount of ticks of a sub clock.
*   When the wait time has expired, the task is ready to execute again and will continue
*   at the next statement when the task is scheduled to run.
*
*   @param id Sub clock id. Valid range 1-255.
*   @param x Number of sub clock ticks to wait, 32 bit value.
*   @remarks \b Usage: @n
* @code 


static void myTask(void) {
 task_open();	
  ...
  task_wait_id( 2, 10 );
  ...
 task_close();
}
 @endcode 
 *******************************************************************************/
#define task_wait_id(id,x)                OS_WAIT_TICKS(x,id)


/*********************************************************************************/
/*  task_suspend( id )                                                 *//**
*   
*   Macro for suspending a task
*
*   @param id of the task to suspend
*   @remarks If the task to suspend is the same as the current running task, the
*   task is immediately suspended and other tasks are scheduled to execute.
*   If the task to suspend is another than the current task, that task is immediately put
*   in suspended state, and the current task continues to execute. @n \b Usage: @n
* @code 

static Msg_t msgPool[ POOL_SIZE ];
uint8_t task1_id;
uint8_t task2_id;

static void led_task(void) {
 task_open();	
  for(;;) {
	led_toggle();
	task_wait( 100 );
  }
 task_close();
}


static void button_task(void) {
 task_open();
 for (;;) {
    if ( buttonPressed ) {
        task_suspend( task1_id );
    }
    task_wait(10);
 }
 task_close();
}


int main( void ) {
    ...
    task1_id = task_create( led_task, 1, msgPool, POOL_SIZE, sizeof(Msg_t) );
    task2_id = task_create( button_task, 2, NULL, 0, 0 );
	...
}

 @endcode 
 *******************************************************************************/
#define task_suspend( id )    OS_SUSPEND_TASK( id )


 /*********************************************************************************/
/*  task_resume(id)                                                 *//**
*   
*   Macro for resuming a task
*
*   @param id of the task to resume
*   @remarks The macro has no effect if the task is not in the SUSPENDED state.
*   If the task was waiting for a semaphore when it was suspended, the task will be reset and
*   execution will restart from the task procedure top.@n \b Usage: @n
* @code 

uint8_t ledTask_id;

static void led_task(void) {
 task_open();	
  for(;;) {
	led_toggle();
	task_wait( 100 );
  }
 task_close();
}


static void button_task(void) {
 task_open();
 for (;;) {
     if ( button_1_Pressed ) {
       task_suspend( ledTask_id );
     }
     else if ( button_2_Pressed ){
       task_resume( ledTask_id );
     }
 }
 task_close();
}


int main( void ) {
	...
	ledTask_id = task_create( led_task, 1, msgPool, POOL_SIZE, sizeof(Msg_t) );
    task_create( button_task, 2, NULL, 0, 0 );
	...
}

 @endcode 
 *******************************************************************************/
#define task_resume( id )     OS_RESUME_TASK( id )


/*********************************************************************************/
/*  event_wait(event)                                                 *//**
*   
*   Macro for wait for a single event.
*
*   @param event: the event to wait for
*   @remarks \b Usage: @n
* @code 
Evt_t myEvent;
main() {
 ...
 myEvent = event_create();
 ...
}

static void myTask(void) {
 task_open();	
  ...
  event_wait( myEvent );
  ...
 task_close();
}
 @endcode 
 *******************************************************************************/
#define event_wait(event)    OS_WAIT_SINGLE_EVENT(event,0,0)
#define event_wait_ex(event, cb)    OS_WAIT_SINGLE_EVENT(event,0,cb)


/*********************************************************************************/
/*  event_wait_timeout(event,timeout)                                                 *//**
*   
*   Macro for wait for a single event to be signaled or a timeout to occur.
*
*   @param event: the event to wait for
*   @param timeout: maximum wait time in main clock ticks, 32bit value. If timeout = 0,
*   no timeout will be used, and the task will wait forever until the event is signaled.
*
*   @remarks \b Usage: @n
* @code 
Evt_t myEvent;
main() {
 ...
 myEvent = event_create();
 ...
}

static void myTask(void) {
 task_open();	
  ...
  event_wait_timeout( myEvent, 100 );
  ...
 task_close();
}
 @endcode 
 *******************************************************************************/
#define event_wait_timeout(event,timeout)    OS_WAIT_SINGLE_EVENT(event,timeout,0)
#define event_wait_timeout_ex(event,timeout,cb)    OS_WAIT_SINGLE_EVENT(event,timeout,cb)

/*********************************************************************************/
/*  event_get_timeout()                                                 *//**
*
*   Get the timeout value from the task when we wait for an event with timeout.
*   That way we know if this is due to a timeout (value == 0)  or actual event (value != 0)
*
*   @remarks \b Usage: @n
* @code
Evt_t myEvent;
main() {
 ...
 myEvent = event_create();
 ...
}

static void myTask(void) {
 task_open();
  ...
  event_wait_timeout( myEvent, 100 );
  if( event_get_timeout() == 0 ) {
    // timeout - the event was not signaled.
  }
  else {
    // the event was signaled
  }

  ...
 task_close();
}
 @endcode
 *******************************************************************************/
#define event_get_timeout()                   OS_GET_TASK_TIMEOUT_VALUE()

/*********************************************************************************/
/*  event_wait_multiple(waitAll, args...)                                                 *//**
*   
*   Macro for wait for multiple events.
*   @param waitAll 1 if wait for all, 0 if wait for any event
*   @param args list of Evt_t type events
*   @remarks \b Usage: @n
* @code 
Evt_t myEvent1;
Evt_t myEvent2;
Evt_t myEvent3;
main() {
 ...
 myEvent1 = event_create();
 myEvent2 = event_create();
 myEvent3 = event_create();
 ...
}

static void myTask(void) {
 task_open();	
  ...
  event_wait_multiple(1, myEvent1, myEvent2, myEvent3);
  ...
 task_close();
}
 @endcode 
 *******************************************************************************/
#define event_wait_multiple(waitAll, args...)   OS_WAIT_MULTIPLE_EVENTS( waitAll, args)


/*********************************************************************************/
/*  event_signal(event)                                                 *//**
*   
*   Macro for signalling an event.
*
*   @param event: the event to be signalled
*   @remarks \b Usage: @n
* @code 
Evt_t myEvent;
main() {
 ...
 myEvent = event_create();
 ...
}

static void myTask(void) {
 task_open();	
  ...
  event_signal( myEvent );
  ...
 task_close();
}
 @endcode 
 *******************************************************************************/
#define event_signal(event) OS_SIGNAL_EVENT(event)


/*********************************************************************************/
/*  event_ISR_signal(event)                                                 *//**
*   
*   Macro for signalling an event from an ISR
*
*   @param event: the event to be signalled
*   @remarks \b Usage: @n
* @code 
Evt_t evRxChar;
main() {
 ...
 evRxChar = event_create();
 ...
}

ISR (SIG_UART_RECV) {   
    rx.data[ rx.head ] = UDR;
    event_ISR_signal( evRxChar );
}
 @endcode 
 *******************************************************************************/
#define event_ISR_signal(event) OS_INT_SIGNAL_EVENT(event)


/*********************************************************************************/
/*  sem_wait(sem)                                                 *//**
*   
*   Macro for aquiring a semaphore.
*
*   @param sem Semaphore.
*   @remarks \b Usage: @n
* @code 
Sem_t mySem;
main() {
 ...
 mySem = sem_create( 0 );
 ...
}

static void myTask(void) {
 task_open();	
  ...
  sem_wait( mySem );
  ...
 task_close();
}
 @endcode 
 *******************************************************************************/
#define sem_wait(sem)  OS_WAIT_SEM(sem)


/*********************************************************************************/
/*  sem_signal(sem)                                                 *//**
*   
*   Macro for releasing a semaphore.
*
*   @param sem Semaphore.
*   @remarks \b Usage: @n
* @code 
Sem_t mySem;
main() {
 ...
 mySem = sem_create( 0 );
 ...
}

static void myTask(void) {
 task_open();	
  ...
  sem_signal( mySem );
  ...
 task_close();
}
 @endcode 
 *******************************************************************************/
#define sem_signal(sem)     OS_SIGNAL_SEM(sem)


/*********************************************************************************/
/*  sem_ISR_signal(sem)                                                 *//**
*
*   Macro for releasing a semaphore from within an ISR.
*   The semaphore is signalled and waiting tasks will be set to ready state, but
*   scheduling will not be done. Scheduling will be performed when the ISR has finished
*   and the interrupted task calls a scheduling function.
*
*   @param sem Semaphore.
*   @remarks \b Usage: @n
* @code
Sem_t mySem;
main() {
 ...
 mySem = sem_create( 0 );
 ...
}

ISR(void) {

  ...
  sem_ISR_signal( mySem );
  ...

}
 @endcode
 *******************************************************************************/
#define sem_ISR_signal(sem)     OS_SIGNAL_SEM_NO_SCHEDULE(sem)


/*********************************************************************************/
/*  msg_post(task_id, msg)                                            *//**
*   
*   Posts a message to the message queue of a task.
*   @param task_id id of the task that will receive the message
*   @param msg the message to post
*   @return None
*   @remarks Performs synchronized message posting, i.e the first task waits for the
*   queue to be available before posting. If the message queue is full, the queue is
*   released and the task wait until it becomes non-full.@n
*   \b Usage: @n
* @code

typedef struct {
    Msg_t super;
    uint8_t led;
} LedMsg_t;

 
static LedMsg_t msgpool_1[ 16 ];
static uint8_t taskId1;
static uint8_t taskId2;

int main(void)
{
    ...
    taskId1 = task_create( task1, 1, NULL, 0, 0 );
    taskId2 = task_create( task2, 2, msgpool_1, 16, sizeof(LedMsg_t) );
    ...
    os_start();
    return 0;
}


static void task1(void) {
    static LedMsg_t ledMsg;

    ledMsg.super.signal = LED_SIG;
    ledMsg.led = 5;
    
    task_open();
    
    for (;;) {
        
        task_wait( 3000 );

        msg_post( taskId2, ledMsg );

    }

    task_close();

}
 @endcode 
*		

*       
*/
/*********************************************************************************/
#define msg_post(task_id, msg)   OS_MSG_Q_POST(task_id, msg, 0, 0, 0)


/*********************************************************************************/
/*  msg_post_async(task_id, msg)                                            *//**
*
*   Posts a message to the message queue of a task. If the queue is full, the
*   message is not posted and the task continues execution, i.e. it will not wait
*   for the queue to be non-full.
*
*   @param task_id id of the task that will receive the message
*   @param msg the message to post
*   @return None @n
*   \b Usage: @n
* @code

typedef struct {
    Msg_t super;
    uint8_t led;
} LedMsg_t;


static LedMsg_t msgpool_1[ 16 ];
static uint8_t taskId1;
static uint8_t taskId2;

int main(void)
{
    ...
    taskId1 = task_create( task1, 1, NULL, 0, 0 );
    taskId2 = task_create( task2, 2, msgpool_1, 16, sizeof(LedMsg_t) );
    ...
    os_start();
    return 0;
}


static void task1(void) {
    static LedMsg_t ledMsg;

    ledMsg.super.signal = LED_SIG;
    ledMsg.led = 5;

    task_open();

    for (;;) {

        task_wait( 3000 );

        msg_post_async( taskId2, ledMsg );

    }

    task_close();

}
 @endcode
*

*
*/
/*********************************************************************************/
#define msg_post_async(task_id, msg)   OS_MSG_Q_POST(task_id, msg, 0, 0, 1)


/*********************************************************************************/
/*  msg_post_in(task_id, msg, delay)                                            *//**
*   
*   Posts a message to the message queue of a task. The message will be received
*   by the receiver task when the delay has expired. The delay is related to the master
*   clock.
*
*   @param task_id id of the task that will receive the message
*   @param msg the message to post
*   @param delay post delay time in master clock ticks
*   @return None
*   @n
*   \b Usage: @n
* @code

typedef struct {
    Msg_t super;
    uint8_t led;
} LedMsg_t;

 
static LedMsg_t msgpool_1[ 16 ];
static uint8_t taskId1;
static uint8_t taskId2;

int main(void)
{
    ...
    taskId1 = task_create( task1, 1, NULL, 0, 0 );
    taskId2 = task_create( task2, 2, msgpool_1, 16, sizeof(LedMsg_t) );
    ...
    os_start();
    return 0;
}


static void task1(void) {
    static LedMsg_t ledMsg;

    ledMsg.super.signal = LED_SIG;
    ledMsg.led = 5;
    
    task_open();
    
    for (;;) {
        
        task_wait( 3000 );

        msg_post_in( taskId2, ledMsg, 100 );

    }

    task_close();

}
 @endcode 
*		

*       
*/
/*********************************************************************************/
#define msg_post_in(task_id, msg, delay)   OS_MSG_Q_POST(task_id, msg, delay, 0, 0)


/*********************************************************************************/
/*  msg_post_every(task_id, msg, period)                                            *//**
*   
*   Posts a periodic message to the message queue of a task. The message will be delivered
*   to the receiver task each time the timer expires. The period is related to the master
*   clock.
*   @param task_id id of the task that will receive the message
*   @param msg the message to post
*   @param period period time in master clock ticks
*   @return None
*   @n
*   \b Usage: @n
* @code

typedef struct {
    Msg_t super;
    uint8_t led;
} LedMsg_t;

 
static LedMsg_t msgpool_1[ 16 ];
static uint8_t taskId1;
static uint8_t taskId2;

int main(void)
{
    ...
    taskId1 = task_create( task1, 1, NULL, 0, 0 );
    taskId2 = task_create( task2, 2, msgpool_1, 16, sizeof(LedMsg_t) );
    ...
    os_start();
    return 0;
}


static void task1(void) {
    static LedMsg_t ledMsg;

    ledMsg.super.signal = LED_SIG;
    ledMsg.led = 5;

    msg_post_every( taskId2, ledMsg, 100 );
  
    task_open();
    
    for (;;) {
        ...
        ...
        task_wait( 3000 );
    }

    task_close();

}
 @endcode 
*		

*       
*/
/*********************************************************************************/
#define msg_post_every(task_id, msg, period)   OS_MSG_Q_POST(task_id, msg, period, period, 0)


/*********************************************************************************/
/*  msg_receive( task_id, pMsg )                                            *//**
*   
*   Receives a message from the queue 
*
*   @param task_id id of the current task
*   @param pMsg pointer to a message that will receive a message from the queue
*   @return None
*   @remarks Performs synchronized message receiveing. The task first waits for
*   the message queue to be available. Then it checks for a message. If the queue
*   is empty, the queue is released and the tasks waits for a message to be posted
*   by another task.
*   @n
*   \b Usage: @n
* @code

typedef struct {
    Msg_t super;
    uint8_t led;
} LedMsg_t;

 
static LedMsg_t msgpool_1[ 16 ];
static uint8_t taskId1;
static uint8_t taskId2;

int main(void)
{
    ...
    taskId1 = task_create( task1, 1, NULL, 0 );
    taskId2 = task_create( task2, 2, msgpool_1, 16, sizeof(LedMsg_t) );
    ...
    os_start();
    return 0;
}


static void task2(void) {
    static LedMsg_t msg;
    uint8_t led;
    task_open();
    
    for (;;) {
        
        msg_receive( taskId2, &msg );

        if ( msg.super.signal == LED_SIG ) {
            led = msg.led;
            LED_TOGGLE( led );
        } 
    }

    task_close();

}
 @endcode 
*       
*/
/*********************************************************************************/
#define msg_receive( task_id, pMsg )        OS_MSG_Q_RECEIVE( task_id, pMsg, 0, 0 )

#define msg_receive_ex( task_id, pMsg, cb )        OS_MSG_Q_RECEIVE( task_id, pMsg, 0, cb )


/*********************************************************************************/
/*  msg_receive_async( task_id, pMsg )                                            *//**
*
*   Receives a message from the queue but returns immediately if queue is empty.
*
*   @param task_id id of the current task
*   @param pMsg pointer to a message that will receive a message from the queue.
*   If queue is empty, the signal member of the message pointed at by pMsg will be
*   set to NO_MSG_ID.
*
*   @return None
*   @n
*   \b Usage: @n
* @code

typedef struct {
    Msg_t super;
    uint8_t led;
} LedMsg_t;


static LedMsg_t msgpool_1[ 16 ];
static uint8_t taskId1;
static uint8_t taskId2;

int main(void)
{
    ...
    taskId1 = task_create( task1, 1, NULL, 0 );
    taskId2 = task_create( task2, 2, msgpool_1, 16, sizeof(LedMsg_t) );
    ...
    os_start();
    return 0;
}


static void task2(void) {
    static LedMsg_t msg;
    uint8_t led;
    task_open();

    for (;;) {

        task_wait( 3000 );

        msg_receive_async( taskId2, &msg );

        if ( msg.super.signal == LED_SIG ) {
            led = msg.led;
            LED_TOGGLE( led );
        }
        else if ( msg.super.signal == NO_MSG_ID ) {
            
        }
    }

    task_close();

}
 @endcode
*
*/
/*********************************************************************************/
#define msg_receive_async( task_id, pMsg )        OS_MSG_Q_RECEIVE( task_id, pMsg, 1 )



void os_init( void );
void os_start( void );
void os_tick( void );
void os_sub_tick( uint8_t id );
void os_sub_nTick( uint8_t id, uint32_t nTicks );
uint8_t os_get_running_tid(void);

uint8_t task_create( taskproctype taskproc, void *data, uint8_t prio, Msg_t* msgPool, uint8_t poolSize, uint16_t msgSize );
void task_kill( uint8_t tid );
void *task_get_data( void );
Sem_t sem_bin_create( uint8_t initial );
Sem_t sem_counting_create( uint8_t max, uint8_t initial );

Evt_t event_create( void );
uint8_t event_signaling_taskId_get( Evt_t ev );
TaskState_t task_state_get(uint8_t tid);
void os_cbkSleep( void );

#ifdef __cplusplus
}
#endif

#endif
