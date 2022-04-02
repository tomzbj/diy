#ifndef _ZKEY_H
#define _ZKEY_H

typedef enum {
    ZK_NONE, ZK_CLICK, ZK_PRESS, ZK_LONGPRESS, ZK_MAX
} zk_action_t;

typedef struct {
    int key_num;
    int* key_count;
    int* key_state;
    int longpress_count;
    int (*read_f)(int key_id);
    void (*action_f)(int key_id, zk_action_t action);
} zk_t;

void zk_init(zk_t* zk);
void zk_poll(void);

#endif
