#include <sys/stat.h>
#include "platform.h"
#include "misc.h"

#ifdef __cplusplus
extern "C" {
#endif

#undef errno
  extern int errno;
  extern int _end;

  caddr_t _sbrk(int incr)
  {
    register uint8_t* pStack asm("sp");
    static uint8_t* s_pHeapEnd;

    if(!s_pHeapEnd)
      s_pHeapEnd = (uint8_t*)&_end;

    if(s_pHeapEnd + incr > pStack)     // conflict, failed to alloc
      return (caddr_t) - 1;

    uint8_t* pOldHeapEnd = s_pHeapEnd;
    s_pHeapEnd += incr;
    return (caddr_t)pOldHeapEnd;
  }

  int link(char* old, char* new1)
  {
    return -1;
  }

  int _close(int file)
  {
    return -1;
  }

  int _fstat(int file, struct stat* st)
  {
    st->st_mode = S_IFCHR;
    return 0;
  }

  int _isatty(int file)
  {
    return 1;
  }

  int _lseek(int file, int ptr, int dir)
  {
    return 0;
  }

  int _read(int file, char* ptr, int len)
  {
    return 0;
  }

  void abort(void)
  {
    while(1);
  }

  int _write(int file, char* ptr, int len)
  {
    u1.write(ptr, len);
    return len;
  }

//int _init(void) { return 0; }

#ifdef __cplusplus
}
#endif

