/** @file docMessages.h If the tasks in an application needs to exchange more information than just simple events, they can 
use messages. The base type for messages is the Msg_t, which is a struct with three members:
@code
typedef struct {
    uint8_t signal;
    uint8_t reserved;
    uint16_t delay;
} Msg_t;
@endcode

The first member, <i>signal</i>, is used for message indentification by the application.@n
<i>reserved</i> is an alignment byte.@n
<i>delay</i> is used by the kernel when sending delayed messages. Should not be accessed by the application.

From this base type, the application can create its own message types containing the members needed:
@code 
enum {
    LED_ON_SIG,
    LED_OFF_SIG
};

typedef struct {
    Msg_t super;    /* All application defined message types should inherit from the Msg_t */
    uint8_t led;
} LedMsg_t;

void task( void ) {
    static LedMsg_t ledMsg;

    ledMsg.super.signal = LED_ON_SIG;
    ledMsg.led = 0;

    task_open();
    ...
    task_close();
}
@endcode

<b>Message passing</b> @n
Messages are passed between tasks by copying the message into a message queue. The posted message must
be of the same type that are held by the message queue. The application must allocate space for this 
message queue and pass the address of the queue, the queue length and the message size when creating the task:
@code
static LedMsg_t msgpool[ Q_SIZE ];

int main(void) {
    task_create( task, prio, msgpool, Q_SIZE, sizeof(LedMsg_t) );
}
@endcode


<b>Mutual exclusion and synchronization</b> @n
For each message queue, cocoOS creates a binary semaphore and an event intended to be used for mutual exclusion
and synchronization. @n Before accessing the queue for either posting or receiving a message, the semaphore
should is automatically acquired by the OS. If the semaphore is already taken by another task, the task will be put
into the waiting state until the semaphore is released. @n @n

The event associated with a message queue is automatically signaled when the queue contents changes, 
i.e. a message is posted to or removed from the queue. A task will be put to wait for the event when a 
message queue was full or empty when trying to post or receive a message respectively. Then when the 
event is signalled, the task makes a new post/receive retry to the message queue. @n
When a message has been received or posted, the queue semaphore is finally automatically released by the OS. @n

<b>Posting messages</b> @n
A message can be posted immediately into another task's message queue by using msg_post(). The message will be placed
in the receiving task's message queue fifo and will be processed as soon as possible. 
With msg_post_in() it is possible to specify a delay before the message will be delivered to the receiving task. 
See also <b>Asynchronous message posting</b> below.
@code

static Msg_t msgpool[ 16 ];

int main(void)
{
	...
    task_create( task1, 1, NULL, 0, 0 );
    taskId2 = task_create( task2, 2, msgpool, 16, sizeof(Msg_t) );
    task_create( task3, 3, NULL, 0, 0 );
    ...
    os_start();
    return 0;
}


static void task1(void) {
    static Msg_t msg;

    msg.signal = 10MS_SIG;
    
    task_open();
    
    for (;;) {
        
        task_wait( 10 );
        
        /* Post the message into task2's queue */
        msg_post( taskId2, msg) );

    }

    task_close();

}


static void task3(void) {
    static Msg_t msg;

    msg.signal = BUTTON_SIG;
    
    task_open();
    
    for (;;) {
        
        event_wait( buttonEvt );
        
        /* Post the message into task2's queue with 1000 ticks delay */
        msg_post_in( taskId2, msg, 1000) );

    }

    task_close();

}

@endcode



<b>Receiving messages</b>@n
Receiving messages is done in the same manner as posting messages, but using msg_receive() instead. 
See also <b>Asynchronous message receiving</b> below.@n
@n
@code
static Msg_t msgpool[ 16 ];

int main(void)
{
	...
    task_create( task1, 1, NULL, 0, 0 );
    taskId2 = task_create( task2, 2, msgpool, 16, sizeof(Msg_t) );
    ...
    os_start();
    return 0;
}


static void task2(void) {
    static Msg_t msg;
    
    task_open();
    
    for (;;) {
        
        /* Wait for a message */
        msg_receive( taskId2, &msg) );

    }

    task_close();

}

@endcode

<b>Periodic messages</b>@n
A message can also be setup to be delivered periodically to its receiver with msg_post_every(). This message will be put into
the message queue as a usual delayed message posted with the msg_post_in() method. When the message delay
expires, the message is delivered to the task and reposted to the queue automatically with the timer reloaded. 
A posted periodic message can not be stopped, it will be delivered periodically forever.
Note that this type of message should be posted outside the endless task loop. Otherwise the queue will quickly fill up and the 
system will be overloaded.

<b>Asynchronous message posting</b>@n
The task can also post messages using the msg_post_async() function. The only difference from the usual msg_post() is that, the 
task will not block waiting for the queue if it was full. The task will continue executing immediately instead. @n


<b>Asynchronous message receiving</b>@n
The msg_reveive_async() can be used to poll the queue for messages without blocking the task if the queue is empty. If there are no messages
waiting in queue, the function will return immediately and the signal member of the msg parameter will be set to the value NO_MSG_ID (0xff)

@code 
void ledTask( void ) {
    static LedMsg_t msg;

    task_open();

    for (;;) {
        msg_receive_async( ledTaskId, &msg );

        if ( msg.super.signal == LED_SIG ) {
            blinkLed();
        }
		else if ( msg.super.signal == NO_MSG_ID ) {
			/* Do something else */
		}

		/* Wait 200 ms before polling message queue again */
        task_wait(200);
    }

    task_close();
}
@endcode

<b>Message API</b>@n
The message API consists of the following macros and functions:@n
 - msg_post()
 - msg_post_async()
 - msg_post_in()
 - msg_receive()
 - msg_receive_async()
 - msg_post_every()
 

*/
