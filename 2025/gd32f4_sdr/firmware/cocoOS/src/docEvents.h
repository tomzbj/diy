/** @file docEvents.h Events are used for task intercommunication and synchronization. For instance, 
you may want one task to run only when a certain character has been received on the UART. Or you may 
want one task to pause its execution and wait for two other tasks to execute up to a certain point.

Events are created at system startup in the main function with a call to event_create(). This function 
returns a handle (Evt_t) to the created event. This handle is used by the tasks to specify an event when
using the event API. When created, the event is in an unsignaled state. When the event is signaled, it
is immediately reset to the unsignaled state again. A task can wait for an event by calling event_wait(event).
This will immediately put the task in a waiting state until the next time the event is signaled. If a maximum 
wait time is desired, event_wait_timeout(event, timeout) can be used instead. If the event has not been
signaled within the timeout, the task exits the waiting state and continues its execution. The task can call
the function event_get_timeout() to find out if the event was signaled or if a timeout occurred. Further
event_signaling_taskId_get() can be called to find out which task signaled the event.



A task can also wait for multiple events with event_wait_multiple(). The first input parameter of this call,
specifies if the task shall wait for all events to be signalled or if it shall resume execution when at least
one of the events has been signalled. After this parameter follows a list of events which the task shall wait for.
When a task waiting for several events is resumed by a signaled event, the task can find out which event
was signaled by using the function event_last_signaled_get().

To signal an event, the task call event_signal(). This will make all tasks waiting for that event, ready to execute. 
The event itself is automatically reset to an unsignaled state. Note, that the signalling task can be suspended, if
any of the waiting tasks have higher priority.

Interrupt service routines should not call event_signal(), because task switching is not allowed during
interrupts. Instead an ISR can call event_ISR_signal() that will signal the event, but any task switch will
be done when both the ISR and the interrupted task has returned.


<b> Wait for multiple events Example </b> @n @n
In the example below, the button_task monitors a button every 20 ms and fires an event when the button 
is pressed. The com_task waits for a RX event to be signalled from the UART RX ISR. When the event is signalled
the task checks the received character, and if it is the updateDisplay character, an event is signalled.
The third task, display_task, waits for any of the button or com event to be signalled and updates the display
when an event has been signalled. 
@code

Evt_t ButtonEvent;
Evt_t UartRxEvent;
Evt_t ComEvent;


static void button_task( void ) {
  task_open();	
  
  for (;;) {
	if ( BUTTON_PRESSED == checkButton() ) {
		event_signal( ButtonEvent );
	}
  
	task_wait( 20 );
  }
  
  task_close();
} 



static void com_task( void ) {
  task_open();	
  
  for(;;) {
	event_wait( UartRxEvent );
  
	if ( UPDATE_DISP_CHAR == uart_getChar() ) {
		event_signal( ComEvent );
	}
  }
  
  task_close();
}


static void display_task( void ) {
  task_open();	
  
  for (;;) {
	event_wait_multiple( 0, ButtonEvent, ComEvent );
    Evt_t lastEvt = event_last_signaled_get();
    if ( lastEvt == ButtonEvent ) {
	    displayUpdate(BUTTON);
	}
	else if (lastEvt == ComEvent) {
	    displayUpdate(COM);
	}
  }
  
  task_close();
}


ISR (SIG_UART_RECV) {
  rx.data[ rx.head ] = UDR;
  rx.head = ( (rx.head + 1) & BUF_MASK );

  event_ISR_signal( UartRxEvent );
	
}


int main( void ) {
  system_init();
  uart_init();

  os_init();
  ButtonEvent = event_create();
  UartRxEvent = event_create();
  ComEvent = event_create();
	
  task_create( button_task, 1, NULL, 0,0 );
  task_create( com_task, 2, NULL, 0,0 );
  task_create( display_task, 3, NULL, 0,0 );
	
  os_start();
	
  return 1;
}

@endcode

<b>Event API</b> @n
The event API consists of the following functions and macros:
 - event_create()
 - event_signaling_taskId_get()
 - event_wait()
 - event_wait_timeout()
 - event_wait_multiple()
 - event_signal()
 - event_ISR_signal()
*/
