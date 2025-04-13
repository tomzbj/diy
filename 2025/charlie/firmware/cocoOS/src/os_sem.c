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


typedef struct {
    uint8_t maxValue;
    uint8_t value;    
} SemValue_t;


static Sem_t os_sem_create( uint8_t max, uint8_t initial );

#if ( N_TOTAL_SEMAPHORES > 0 )
static SemValue_t semList[ N_TOTAL_SEMAPHORES ];
static Sem_t nSemaphores;
#endif							   

void os_sem_init(void) {
#if ( N_TOTAL_SEMAPHORES > 0 )
    nSemaphores = 0;
#endif
}


static Sem_t os_sem_create( uint8_t max, uint8_t initial ) {
#if ( N_TOTAL_SEMAPHORES > 0 )
    os_assert( nSemaphores < N_TOTAL_SEMAPHORES );

    /* Initialize the value and the waiting list */
    semList[ nSemaphores ].maxValue = max;
    semList[ nSemaphores ].value = initial;
    ++nSemaphores;

    return ( nSemaphores - 1 );
#else
    return 0;
#endif
}


/*********************************************************************************/
/*  Sem_t sem_bin_create(uint8_t initial)                                              *//**
*   
*   Creates and initializes a new binary semaphore.
*
*   @param initial value of the semaphore
*   @return Returns the created semaphore.
*   @remarks \b Usage: @n A semaphore is created by declaring a variable of type Sem_t
*   and then assigning the sem_bin_create(value) return value to that variable.
*
*   @code
*   Sem_t mySem;
*   mySem = sem_bin_create(0);
*
*   @endcode
*       
*		 */
/*********************************************************************************/
Sem_t sem_bin_create( uint8_t initial ) {
    initial = initial > 0 ? 1 : 0;
    return os_sem_create( 1, initial );
}


/*********************************************************************************/
/*  Sem_t sem_counting_create(uint8_t max, uint8_t initial)                                              *//**
*   
*   Creates and initializes a new counting semaphore.
*
*   @param max value of the semaphore
*   @param initial value of the semaphore
*   @return Returns the created semaphore.
*   @remarks \b Usage: @n A semaphore is created by declaring a variable of type Sem_t
*   and then assigning the sem_create(max, 0) return value to that variable.
*
*   @code
*   Sem_t mySem;
*   mySem = sem_counting_create(5,0);
*
*   @endcode
*       
*		 */
/*********************************************************************************/
Sem_t sem_counting_create( uint8_t max, uint8_t initial ) {
    return os_sem_create( max, initial );
}


uint8_t os_sem_larger_than_zero( Sem_t sem) {
#if ( N_TOTAL_SEMAPHORES > 0 )
    return ( semList[ sem ].value > 0 );
#else
    return 0;
#endif
}


void os_sem_decrement( Sem_t sem ) {
#if ( N_TOTAL_SEMAPHORES > 0 )
    if ( semList[ sem ].value > 0 ) {
        --semList[ sem ].value;
    }
#endif
}


void os_sem_increment( Sem_t sem ) {
#if ( N_TOTAL_SEMAPHORES > 0 )
    if ( semList[ sem ].value < semList[ sem ].maxValue ) {
        ++semList[ sem ].value;
    }
#endif
}



