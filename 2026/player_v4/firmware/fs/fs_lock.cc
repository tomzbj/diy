#include "fs_lock.h"
#include "misc.h"
#include "player.h"

extern PLAYER player;

static volatile uint32_t fs_lock_depth = 0;
static uint32_t fs_lock_saved_primask = 0;
static volatile bool fs_mtp_active = false;

void fs_lock(void)
{
  const uint32_t primask = __get_PRIMASK();
  __disable_irq();
  if(fs_lock_depth++ == 0u) {
    fs_lock_saved_primask = primask;
  }
}

void fs_unlock(void)
{
  if(fs_lock_depth == 0u) {
    return;
  }
  if(--fs_lock_depth == 0u) {
    if(fs_lock_saved_primask == 0u) {
      __enable_irq();
    }
  }
}

FRESULT fs_getfree_safe(const TCHAR* path, DWORD* nclst, FATFS** fatfs)
{
  if(nclst == NULL) {
    return FR_INVALID_OBJECT;
  }

  fs_lock();
  FATFS* vol = NULL;
  FRESULT res = f_getfree(path, nclst, &vol);

  if(res == FR_OK && vol != NULL && *nclst == 0u) {
    vol->free_clst = 0xFFFFFFFFu;
    res = f_getfree(path, nclst, &vol);
  }

  if(res != FR_OK && vol != NULL) {
    vol->free_clst = 0xFFFFFFFFu;
    const FRESULT retry = f_getfree(path, nclst, &vol);
    if(retry == FR_OK) {
      res = retry;
    }
  }

  if(fatfs != NULL) {
    *fatfs = vol;
  }
  fs_unlock();
  return res;
}

void fs_mtp_session_set(bool active)
{
  fs_mtp_active = active;
}

bool fs_mtp_session_active(void)
{
  return fs_mtp_active;
}

bool fs_playback_allowed(void)
{
  return !fs_mtp_transfer_busy();
}

void fs_playback_suspend(void)
{
  player.stop();
}
