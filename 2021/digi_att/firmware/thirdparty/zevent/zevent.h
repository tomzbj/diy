#ifndef _ZE_H
#define _ZE_H

#define ZE_MAX_EVENT 8
#define ZE_MAX_HANDLER 8

typedef void (*ze_handler_t)(int event);

void ze_init(void);
void ze_post(int event);
void ze_bind(ze_handler_t handler);
void ze_poll(void);

#endif
