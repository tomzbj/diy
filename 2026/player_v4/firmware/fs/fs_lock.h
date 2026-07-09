#ifndef _FS_LOCK_H
#define _FS_LOCK_H

#include "ff.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void fs_lock(void);
void fs_unlock(void);

FRESULT fs_getfree_safe(const TCHAR* path, DWORD* nclst, FATFS** fatfs);

void fs_mtp_session_set(bool active);
bool fs_mtp_session_active(void);
bool fs_mtp_transfer_busy(void);
bool fs_playback_allowed(void);
void fs_playback_suspend(void);
void mtp_host_disconnected(void);

#ifdef __cplusplus
}

class FS_LOCK {
  public:
    FS_LOCK()
    {
      fs_lock();
    }
    ~FS_LOCK()
    {
      fs_unlock();
    }
    FS_LOCK(const FS_LOCK&) = delete;
    FS_LOCK& operator=(const FS_LOCK&) = delete;
};
#endif

#endif
