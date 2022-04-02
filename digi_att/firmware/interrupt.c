#include "platform.h"
#include "misc.h"

ISR( TIMER2_OVF_vect)
{
    TCNT2 = 0x6;
    zt_tick();
}
