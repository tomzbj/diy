#include "zevent.h"

#define ze_is_empty() (g.front==g.rear)
#define ze_remove() do { if(!ze_is_empty()) {g.front++; g.front %= ZE_MAX_EVENT;}}while(0)
#define ze_is_full() ((g.rear + 1) % ZE_MAX_EVENT == g.front)
#define ze_get_head() (ze_is_empty() ? (-1) : (g.q[g.front]))

static struct {
    int front;
    int rear;
    int q[ZE_MAX_EVENT];
    ze_handler_t handler_f[ZE_MAX_HANDLER];
    int id;
} g;

void ze_init(void)
{
    g.front = g.rear = 0;
    g.id = 0;
}

void ze_post(int event)
{
    if(ze_is_full())
        return;
    g.q[g.rear] = event;
    g.rear++;
    g.rear %= ZE_MAX_EVENT;
}

void ze_poll(void)
{
//    if(ze_get_length() > 0) {
    if(!ze_is_empty()) {
        int i, event = ze_get_head();
        for(i = 0; i < g.id; i++) {
            g.handler_f[i](event);
        }
        ze_remove();
    }
}

void ze_bind(ze_handler_t handler)
{
    if(g.id < ZE_MAX_HANDLER) {
        g.handler_f[g.id] = handler;
        g.id++;
    }
}
