#include "idecoder.h"
#include "fs_lock.h"

bool IDECODER::running(void)
{
  return _running;
}

void IDECODER::close(void)
{
  FS_LOCK lock;
  if(_file.obj.fs != nullptr) {
    f_close(&_file);
  }
}

AUDIO_SYS::FILE_INFO IDECODER::get_info(void)
{
  return _info;
}

IDECODER::RESULT IDECODER::open(const char* path)
{
  FS_LOCK lock;
  auto ret = f_open(&_file, path, FA_READ);
  return (ret == FR_OK) ? IDECODER::SUCCESS : IDECODER::FAILED;
}
