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
 

#ifndef OS_TASK_H__
#define OS_TASK_H__

/** @file os_task.h Task header file*/

#include "os_defines.h"
#include "os_msgqueue.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tcb tcb;

typedef enum {
    SUSPENDED,
    WAITING_SEM,
    WAITING_TIME,
    WAITING_EVENT,
	WAITING_EVENT_TIMEOUT,
    READY,
    RUNNING,
    KILLED
} TaskState_t;


#define TASK_OFS1    30000
#define TASK_OFS2    31000

#define OS_SUSPEND_TASK( id )    do {\
								        os_task_suspend( id );\
                                        if ( id == running_tid ) {\
                                            OS_SCHEDULE(TASK_OFS1);\
                                        }\
							  	    } while (0)




#define OS_RESUME_TASK( id )		do {\
								        os_task_resume( id );\
                                        if ( id == running_tid ) {\
                                            OS_SCHEDULE(TASK_OFS2);\
                                        }\
							  	    } while (0)


void os_task_init(void);
uint8_t os_task_highest_prio_ready_task( void );
uint8_t os_task_next_ready_task( void );
void os_task_ready_set( uint8_t tid );
void os_task_wait_sem_set( uint8_t tid, Sem_t sem );
void os_task_suspend( uint8_t tid );
void os_task_resume( uint8_t tid );
void os_task_kill( uint8_t tid );
uint8_t os_task_prio_get( uint8_t tid );
void os_task_clear_wait_queue( uint8_t tid );
void os_task_wait_time_set( uint8_t tid, uint8_t id, uint32_t time );
void os_task_wait_event( uint8_t tid, Evt_t eventId, uint8_t waitSingleEvent, uint32_t timeout );
void os_task_tick( uint8_t id, uint32_t tickSize );
void os_task_signal_event( Evt_t eventId );
void os_task_run( void );
uint16_t os_task_internal_state_get( uint8_t tid );
void os_task_internal_state_set( uint8_t tid, uint16_t state );
void os_task_release_waiting_task( Sem_t sem );
uint8_t os_task_waiting_this_semaphore( Sem_t sem );
MsgQ_t os_task_msgQ_get( uint8_t tid );
void os_task_set_wait_queue(uint8_t tid, MsgQ_t queue);
MsgQ_t os_task_get_wait_queue(uint8_t tid);
void os_task_set_change_event(uint8_t tid, Evt_t event);
Evt_t os_task_get_change_event(uint8_t tid);
void os_task_set_msg_result(uint8_t tid, uint8_t result);
uint8_t os_task_get_msg_result(uint8_t tid);
uint32_t os_task_timeout_get(uint8_t tid);



#ifdef __cplusplus
}
#endif


#endif
