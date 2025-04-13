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
 


#include "cocoos.h"

static void os_schedule( void );

uint8_t running_tid;
uint8_t last_running_task;
uint8_t running;

/*********************************************************************************/
/*  void os_init()                                              *//**
*   
*   Initializes the scheduler.
*   @return None.
*   @remarks \b Usage: @n Should be called early in system setup, before starting the task
*   execution
*
*   @code
*   int main(void) {
*   system_init();
*   os_init();
*   ...
*   }
*   @endcode
*       
*		 */
/*********************************************************************************/
void os_init( void ) {
	running_tid = NO_TID;
    last_running_task = NO_TID;
    running = 0;
    os_sem_init();
    os_event_init();
    os_msgQ_init();
    os_task_init();
}




static void os_schedule( void ) {

    running_tid = NO_TID;

#if (ROUND_ROBIN)
    /* Find next ready task */
    running_tid = os_task_next_ready_task();
#else
    /* Find the highest prio task ready to run */
    running_tid = os_task_highest_prio_ready_task();   
#endif
    
    if ( running_tid != NO_TID ) {
        os_task_run();
    }
    else {
        os_cbkSleep();
    }
}




/*********************************************************************************/
/*  void os_start()                                              *//**
*   
*   Starts the task scheduling
*
*   @return None.
*   @remarks \b Usage: @n Should be the last line of main.
*
*   @code
*   int main(void) {
*     system_init();
*     os_init();
*     task_create( myTaskProc, 1, NULL, 0, 0 );
*     ...
*     os_start();
*     return 0;
*   }
*   @endcode
*       
*/
/*********************************************************************************/
void os_start( void ) {
    running = 1;
    os_enable_interrupts();

    for (;;) {
        os_schedule();
    }
}


/*********************************************************************************/
/*  void os_tick()                                              *//**
*   
*   Tick function driving the kernel
*
*   @return None.
*   @remarks \b Usage: @n Should be called periodically. Preferably from the clock tick ISR.
*
*   @code
*   ISR(SIG_OVERFLOW0) {
*     ...
*     os_tick();
*   }
*
*   @endcode
*       
*/
/*********************************************************************************/
void os_tick( void ) {
    /* Master clock tick */
    os_task_tick( 0, 1 );
}


/*********************************************************************************/
/*  void os_sub_tick( id )                                              *//**
*   
*   Tick function driving the sub clocks
*
*   @param id sub clock id, allowed range 1-255
*
*   @return None.
*   @remarks \b Usage: @n Could be called at any desired rate to trigger timeouts.
*   Called from a task or from an interrupt ISR.
*
*   @code
*   ISR(SIG_OVERFLOW0) {
*     ...
*     os_sub_tick(2);
*   }
*
*   @endcode
*       
*/
/*********************************************************************************/
void os_sub_tick( uint8_t id ) {
    /* Sub clock tick */
    if ( id != 0 ) {
        os_task_tick( id, 1 );
    }
}


/*********************************************************************************/
/*  void os_sub_nTick( id, nTicks )                                              *//**
*   
*   Tick function driving the sub clocks. Increments the tick count with nTicks.
*
*   @param id sub clock id, allowed range 1-255.
*   @param nTicks increment size, 16 bit value.
*   @return None.
*   @remarks \b Usage: @n Could be called at any desired rate to trigger timeouts.
*   Called from a task or from an interrupt ISR.
*
*   @code
*   ISR(SIG_OVERFLOW0) {
*     ...
*     os_sub_nTick( 2, 10 );
*   }
*
*   @endcode
*       
*/
/*********************************************************************************/
void os_sub_nTick( uint8_t id, uint32_t nTicks ) {
    /* Sub clock tick */
    if ( id != 0 ) {
        os_task_tick( id, nTicks );
    }
}


uint8_t os_running( void ) {
    return running;
}

uint8_t os_get_running_tid(void) {
    return running_tid;
}

#ifdef UNIT_TEST
void os_run() {
    running = 1;
    os_enable_interrupts();
    os_schedule();
}

void os_run_until_taskState(uint8_t taskId, TaskState_t state) {
    running = 1;
    os_enable_interrupts();
    do {
        os_schedule();
    } while ( state != task_state_get(taskId) );
}

TaskState_t os_get_task_state(uint8_t taskId) {
    return task_state_get(taskId);
}




#endif
