#ifndef _AUDIO_TYPES_H
#define _AUDIO_TYPES_H

#include <cstdint>

// 使用命名空間防止與其他第三方庫（如 TinyUSB 或 FatFs）發生名稱衝突
namespace AUDIO_SYS {

  enum class FILE_TYPE {
    NONE, MP3, WAV, TXT, FLAC
  };

  // 核心數據結構：解碼後的 PCM 幀資訊
  struct FILE_INFO {
      uint8_t bit_depth;
      uint8_t channels;
      uint32_t sample_rate;
  };
  struct FRAME {
      uint8_t* data;          // 指向解碼器內部靜態快取的指針
      uint32_t size;        // 本幀有效的位元組長度
      uint32_t sample_rate;    // 採樣率（如 44100）
      uint8_t channels;      // 通道數（1:單聲道, 2:立體聲）
      FILE_TYPE file_type;        // 當前檔案格式
  };

}    // namespace AUDIO_SYS

#endif

