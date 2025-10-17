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
 
 

#ifndef COCOOS_H
#define COCOOS_H

/** @file cocoos.h cocoOS kernel header file*/

#include <stdint.h>
#include "os_port.h"
#include "os_typedef.h"
#include "os_defines.h"
#include "os_event.h"
#include "os_sem.h"
#include "os_task.h"
#include "os_assert.h"
#include "os_msgqueue.h"
#include "os_applAPI.h"

#ifdef __cplusplus

extern "C" {

#endif

#define NO_TID	        255
#define NO_EVENT        255
#define NO_QUEUE        255
#define NO_SEM          255







#define OS_BEGIN            uint16_t os_task_state = os_task_internal_state_get(running_tid); switch ( os_task_state ) { case 0:

#define OS_END	            os_task_kill(running_tid);\
                            running_tid = NO_TID;\
					        return;}


#define OS_SCHEDULE(ofs)    os_task_internal_state_set(running_tid, __LINE__+ofs);\
                            running_tid = NO_TID;\
                            return;\
                            case (__LINE__+ofs):




#define OS_WAIT_TICKS(x,y)	do {\
								os_task_wait_time_set( running_tid, y, x );\
								OS_SCHEDULE(0);\
						   	   } while ( 0 )



extern uint8_t running_tid;
extern uint8_t last_running_task;
extern uint8_t running;

uint8_t os_running( void );


#ifdef UNIT_TEST
void os_run();
void os_run_until_taskState(uint8_t taskId, TaskState_t state);
TaskState_t os_get_task_state(uint8_t taskId);
uint8_t os_get_running_tid(void);
#endif

#ifdef __cplusplus

}

#endif

#endif
