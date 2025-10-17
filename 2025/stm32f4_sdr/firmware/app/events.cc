#include "misc.h"
#include "platform.h"
#include "events.h"
#include "mui.h"

EVENTS events;

ZEVENT ze(64);

static void ms_event(void)
{
  static int count = 0;
  ++count %= 250;
  if(count == 0) {
//    printf("500ms\n");
//    DISP_Update();
  }
}

void EVENTS::poll(void)
{
  ZEVENT::evt_t e;
  extern mui_t ui;

  if(ze.poll(&e)) {
    if(e.id == EVENTS::MILLISECOND) {
      ms_event();
    }
    else if(e.id == EVENTS::KEY_CLICKED) {
      if(e.data == 2) {
        mui_PrevField(&ui);
      }
      else if(e.data == 4) {
        mui_NextField(&ui);
      }
      else if(e.data == 3) {
        mui_SendSelect(&ui);
      }
      {
        extern uint16_t tune;
        extern uint8_t bpf, vol;
        printf("%8d %8d %8d\n", tune * 9 + 522, bpf, vol + 3);
        extern class RF rf;
        rf.tune(tune * 9000 + 522000);
        rf.set_bpf(bpf);
        rf.set_volume(vol + 3);
      }
      DISP_Update();
    }
    else if(e.id != EVENTS::KEY_CLICKED)
      printf("%lu %lu\n", e.id, e.data);
//    switch(e.id) { printf("%lu %lu\n", e.id, e.data); }
  }
}
