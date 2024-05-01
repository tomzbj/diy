/** @file docSemaphores.h cocoOS supports binary and counting semaphores. A semaphore is created in the 
main function using one of the create functions: sem_bin_create() or sem_counting_create().
@code
Sem_t bin_sem;
Sem_t count_sem;

int main( void ) {
    bin_sem = sem_bin_create( initValue );
    count_sem = sem_counting_create( maxCount, initValue );
    ...
}
@endcode
<i>initValue</i> is the initial value for the semaphore counter. For a binary semaphore this can
be 0 or 1, and for the counting semaphore 0-255. @n

<i>maxCount</i> is the maximum value the counting semaphore can reach. @n

A call to sem_wait() decrements the semaphore value by one if it is not 0. If it is 0, the value is not
decremented, but the task is put into the waiting state. @n

A call to sem_signal() increments the semaphore value by one if it is not 1 in case of a binary semaphore,
or maxCount in case of a counting semaphore. The semaphore is only incremented if no tasks are waiting
for the semaphore. If there are a waiting task, the semaphore value is left unchanged and the highest 
priority waiting task gets the semaphore and is put back to ready state again. 

Interrupt service routines should not call sem_signal(), because task switching is not allowed during
interrupts. Instead an ISR can call sem_ISR_signal() that will release the semaphore, but any task switch will
be done when both the ISR and the interrupted task has returned.

@n @n
<b> Task Synchronization Example </b> @n @n
In the example below, sem is a semaphore initialized with value 0. Task1 has higher priority than Task2.
Task 1 will execute down to the sem_wait( sem ) call, where it will block because the semaphore value is 0.
Then Task2 is scheduled to execute, and will run down to the line where the semaphore is signaled. This will 
cause the Task2 to block, and Task1 is scheduled to execute due to its higher priority.
@code
static void task1( void ) {
  task_open();	
  for (;;) {
    ...
    sem_wait( sem );
    ...
  }
  task_close();
} 

static void task2( void ) {
  task_open();	
  for (;;) {
    ...
    sem_signal( sem );
    ...
  }
  task_close();
}
@endcode

<b>Semaphore API</b>
The Semaphore API consists of the following functions and macros:
 - sem_bin_create()
 - sem_counting_create()
 - sem_wait()
 - sem_signal()
 - sem_ISR_signal()
*/
