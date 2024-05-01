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
 

#ifndef _os_defs
#define _os_defs

/** @file os_defines.h cocoOS user configuration
          Macros can be defined in this file or as compiler flags e.g. -DN_TASKS=2 -DN_EVENTS=3...
*/

#ifdef __cplusplus
extern "C" {
#endif



/** Max number of used tasks
* @remarks Must be defined. @n Allowed range: 0-254. Value must not be exceeded */
#ifndef N_TASKS
 #define N_TASKS             8
#endif



/** Max number of used message queues
* @remarks Must be defined. @n Allowed range: 0-254. Value must not be exceeded */
#ifndef N_QUEUES
 #define N_QUEUES            0
#endif


/** Max number of used semaphores
* @remarks Must be defined. @n Allowed range: 0-254. Value must not be exceeded */
#ifndef N_SEMAPHORES
 #define N_SEMAPHORES        0
#endif


/** Max number of used events
* @remarks Must be defined. @n Allowed range: 0-254. Value must not be exceeded */
#ifndef N_EVENTS
 #define N_EVENTS            0
#endif


/** Round Robin scheduling
* @remarks If defined, tasks will be scheduled ignoring the priorities */
#ifndef ROUND_ROBIN
 #define ROUND_ROBIN 1
#endif


/** Memory size
 * @remarks Should be set to the size of address pointer */
#ifndef Mem_t
 #define Mem_t uint32_t
#endif

#define NO_MSG_ID   0xff
#define ISR_TID     0xfe

    /* Total number of semaphores needed */
#define N_TOTAL_SEMAPHORES    ( N_SEMAPHORES + N_QUEUES )


/* Total number of events needed */
#define N_TOTAL_EVENTS        ( N_EVENTS + N_QUEUES )

#ifdef __cplusplus
}
#endif

#endif
