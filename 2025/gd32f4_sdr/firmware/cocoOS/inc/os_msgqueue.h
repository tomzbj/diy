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
 


#ifndef OS_MSGQUEUE_H__
#define OS_MSGQUEUE_H__

/** @file os_msgqueue.h Message queue header file*/

#include "os_defines.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
    uint8_t signal;
    uint8_t reserved;   /* Alignment byte */
    uint8_t pad0;
    uint8_t pad1;
    uint32_t delay;     /* Delay of posting in ticks */
    uint32_t reload;    /* Reload value for periodic messages */
} Msg_t;


typedef uint8_t MsgQ_t;


enum {
    MSG_QUEUE_UNDEF,
    MSG_QUEUE_DEF,
    MSG_QUEUE_EMPTY,
    MSG_QUEUE_FULL,
    MSG_QUEUE_RECEIVED,
    MSG_QUEUE_POSTED
};




#define OS_MSG_Q_POST(task_id, msg, delay, period, async )     do {\
                                                                uint8_t os_posted;\
                                                                MsgQ_t queue;\
                                                                queue = os_msgQ_find(task_id);\
                                                                os_task_set_wait_queue(running_tid, queue);\
                                                                Evt_t event;\
                                                                event = os_msgQ_event_get( queue );\
                                                                os_task_set_change_event(running_tid, event);\
                                                                do {\
                                                                    os_posted = os_msg_post( (Msg_t*)&msg, os_msgQ_find(task_id), delay, period );\
                                                                    if ( os_posted == MSG_QUEUE_FULL ){\
                                                                        if ( async == 0 ) {\
                                                                            os_task_set_msg_result(running_tid, os_posted);\
                                                                            event_wait(event);\
                                                                            os_posted = os_task_get_msg_result(running_tid);\
                                                                            event = os_task_get_change_event(running_tid);\
                                                                        }\
                                                                        else {\
                                                                            os_posted = MSG_QUEUE_UNDEF;\
                                                                        }\
                                                                    }\
                                                                } while ( os_posted == MSG_QUEUE_FULL );\
                                                                if ( MSG_QUEUE_POSTED == os_posted ) {\
                                                                	os_signal_event(event);\
                                                                	os_event_set_signaling_tid( event, running_tid );\
                                                                }\
                                                            } while(0)



#define OS_MSG_Q_RECEIVE(task_id, pMsg, async, cb)     do {\
                                                    uint8_t os_received;\
                                                    MsgQ_t queue;\
                                                    queue = os_msgQ_find(task_id);\
                                                    os_task_set_wait_queue(running_tid, queue);\
                                                    Evt_t event;\
                                                    event = os_msgQ_event_get(queue);\
                                                    os_task_set_change_event(running_tid, event);\
                                                    do {\
                                                        os_received = os_msg_receive((Msg_t*)pMsg, os_msgQ_find(task_id));\
                                                        if ( os_received == MSG_QUEUE_EMPTY ){\
                                                            if ( async == 0 ) {\
       	                                                        os_task_set_msg_result(running_tid, os_received);\
                                                                if (cb) {\
                                                                 ((void (*)())cb)();\
                                                                }\
                                                                event_wait(event);\
                                                                os_received = os_task_get_msg_result(running_tid);\
                                                                event = os_task_get_change_event(running_tid);\
                                                            }\
                                                            else {\
                                                                ((Msg_t*)pMsg)->signal = NO_MSG_ID;\
                                                                os_received = MSG_QUEUE_UNDEF;\
                                                            }\
                                                        }\
                                                    } while ( os_received == MSG_QUEUE_EMPTY );\
                                                    if ( MSG_QUEUE_RECEIVED == os_received) {\
                                                    	os_signal_event(event);\
                                                    	os_event_set_signaling_tid(event, running_tid );\
													}\
                                                } while(0)




void os_msgQ_init( void );
MsgQ_t os_msgQ_create( Msg_t *buffer, uint8_t nMessages, uint16_t msgSize,  uint8_t task_id );
MsgQ_t os_msgQ_find( uint8_t task_id );
//Sem_t os_msgQ_sem_get( MsgQ_t queue );
Evt_t os_msgQ_event_get( MsgQ_t queue );
void os_msgQ_tick( MsgQ_t queue );

uint8_t os_msg_post( Msg_t *msg, MsgQ_t queue, uint32_t delay, uint32_t period );
uint8_t os_msg_receive( Msg_t *msg, MsgQ_t queue );


#ifdef __cplusplus
}
#endif

#endif
