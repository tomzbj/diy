#include "decodermanager.h"

AUDIO_SYS::FILE_TYPE DECODER_MANAGER::detect_file_type(const char* path)
{
  auto* p = strrchr(path, '.');
  if(p != nullptr) {
//    printf("%s\n", p + 1);
    if(strcasecmp(p + 1, "mp3") == 0)
      return AUDIO_SYS::FILE_TYPE::MP3;
    else if(strcasecmp(p + 1, "wav") == 0)
      return AUDIO_SYS::FILE_TYPE::WAV;
    else if(strcasecmp(p + 1, "txt") == 0)
      return AUDIO_SYS::FILE_TYPE::TXT;
    else if(strcasecmp(p + 1, "flac") == 0)
      return AUDIO_SYS::FILE_TYPE::FLAC;
  }
  return AUDIO_SYS::FILE_TYPE::NONE;
}

IDECODER* DECODER_MANAGER::get_decoder(const char* path)
{
  auto file_type = detect_file_type(path);

//  printf("%d\n", static_cast<int>(file_type));
  switch(file_type) {
    case AUDIO_SYS::FILE_TYPE::MP3:
      return &_mp3_decoder;
    case AUDIO_SYS::FILE_TYPE::WAV:
      return &_wav_decoder;
    case AUDIO_SYS::FILE_TYPE::TXT:
      return &_txt_decoder;
    case AUDIO_SYS::FILE_TYPE::FLAC:
      return &_flac_decoder;
    default:
      return nullptr;
  }
}
