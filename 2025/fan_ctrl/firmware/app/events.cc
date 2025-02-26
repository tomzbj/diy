#include "misc.h"
#include "platform.h"
#include "events.h"

EVENTS events;

static uint8_t events_fifo[16];
ZEVENT ze(events_fifo, sizeof(events_fifo));

static void timer_handler(void)
{
//  _delay_us(1);
  if(u1.idle()) {
    char buf[128];
    memset(buf, 0, sizeof(buf));
    char c = u1.peek();
    int len = u1.read(buf, sizeof(buf));
    if(c == '#')
      cli.parse(buf, len);
  }
  if(0) {
    static int count = 0;
    ++count %= 250;
    if(!count)
      printf("Hello, world.\n");
  }
}

void EVENTS::poll(void)
{
  extern class FAN fan;
  int e = ze.get();
  switch(e) {
    case KEY_PRESSED:
      fan.toggle_state();
      break;
    case KEY_LONGPRESSED:
      fan.off();
      break;
    case TIMER:
      timer_handler();
      break;
  }
}
