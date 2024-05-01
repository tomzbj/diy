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
 

#ifndef OS_SEM_H
#define OS_SEM_H

/** @file os_sem.h Semaphore header file*/

#include "cocoos.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SEM_OFS1 20000
#define SEM_OFS2 21000

#define OS_WAIT_SEM(sem)		do {\
                                    if ( os_sem_larger_than_zero( sem )  ){\
                                        os_sem_decrement( sem );\
                                    }\
                                    else{\
                                        os_task_wait_sem_set( running_tid, sem );\
                                        OS_SCHEDULE(SEM_OFS1);\
                                    }\
                                } while (0)


#define OS_SIGNAL_SEM(sem)      do {\
                                    if ( os_task_waiting_this_semaphore( sem ) == 0 ) {\
                                        os_sem_increment( sem );\
                                    }\
                                    else {\
                                        os_task_release_waiting_task( sem );\
                                        OS_SCHEDULE(SEM_OFS2);\
                                    }\
                                } while (0)


#define OS_SIGNAL_SEM_NO_SCHEDULE(sem)  do {\
                                            if ( os_task_waiting_this_semaphore( sem ) == 0 ) {\
                                                os_sem_increment( sem );\
                                            }\
                                            else {\
                                                os_task_release_waiting_task( sem );\
                                            }\
                                        } while (0)




typedef uint8_t Sem_t;

void os_sem_init(void);
uint8_t os_sem_larger_than_zero( Sem_t sem );
void os_sem_decrement( Sem_t sem );
void os_sem_increment( Sem_t sem );

#ifdef __cplusplus
}
#endif

#endif
