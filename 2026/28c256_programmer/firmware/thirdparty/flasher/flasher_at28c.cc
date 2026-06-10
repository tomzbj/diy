#include "flasher_at28c.h"
#include <cstring>

#define MAX_PACK_SIZE 64

void FLASHER_AT28C::uputc(uint8_t c)
{
  _cbs.usart_write_f(&c, 1);
}

uint32_t FLASHER_AT28C::crc32_soft(uint32_t crc, const void* pmsg, int size)
{
  const uint32_t polynormial = 0xedb88320;
  auto msg = (const uint8_t*)pmsg;
  for(int i = 0; i < size; i++) {
    crc ^= *(uint8_t*)(msg + i);
    for(int j = 0; j < 8; j++) {
      int lsb = crc % 2;
      crc >>= 1;
      if(lsb)
        crc ^= polynormial;
    }
  }
  return crc;
}

void FLASHER_AT28C::read(uint16_t pos, int size)
{
  uint8_t buf[MAX_PACK_SIZE];
//  if(size > MAX_PACK_SIZE) size = MAX_PACK_SIZE;
  while(size > 0) {
//printf("%d\n", size);
    int batch_size = MAX_PACK_SIZE;
    if(size < batch_size)
      batch_size = size;
    _cbs.read_f(pos, batch_size, buf);
    _cbs.usart_write_f(buf, batch_size);
    size -= batch_size;
    pos += batch_size;
  }
}

void FLASHER_AT28C::read_crc32(uint16_t pos, int total_size)
{
  uint8_t buf[MAX_PACK_SIZE];
  int size = MAX_PACK_SIZE;
  uint32_t crc;

  crc = 0xffffffff;
  while(total_size > 0) {
    if(total_size < size)
      size = total_size;
    _cbs.read_f(pos, size, buf);
    crc = crc32(crc, buf, size);
    pos += size;
    total_size -= size;
  }
  crc = ~crc;
  _cbs.usart_write_f(&crc, 4);
}

void FLASHER_AT28C::write(uint16_t pos, uint16_t size, const void* content)
{
  _cbs.write_f(pos, size, (void*)content);
  uint8_t tmp = 0;
  _cbs.usart_write_f(&tmp, 1);
}

void FLASHER_AT28C::erase_chip(void)
{
  const uint8_t buf[MAX_PACK_SIZE] = {0};
  for(int i = 0; i < 32768; i += MAX_PACK_SIZE)
    _cbs.write_f(i, MAX_PACK_SIZE, buf);
  uint8_t tmp = 0;
  _cbs.usart_write_f(&tmp, 1);
}

uint32_t FLASHER_AT28C::crc32(uint32_t crc, const uint8_t* msg, int size)
{
  const uint32_t polynormial = 0xedb88320;

  for(int i = 0; i < size; i++) {
    crc ^= msg[i];
    for(int j = 0; j < 8; j++) {
      bool lsb = crc & 1;
      crc >>= 1;
      if(lsb)
        crc ^= polynormial;
    }
  }
  return crc;
}

FLASHER_AT28C::RESULT FLASHER_AT28C::verify(const uint8_t* msg, int size)
{
  uint32_t crc1 = 0, crc2 = 0xffffffff;
  memcpy(&crc1, &msg[size - 4], 4);
  crc2 = ~crc32(crc2, msg, size - 4);
  return (crc1 == crc2) ? (OK) : (ERROR);
}

void FLASHER_AT28C::parse_binary(const uint8_t* msg, int msg_size)
{
  const uint8_t* content;
  uint16_t pos, size;
  if(verify(msg, msg_size) != RESULT::OK) {
    uputc(0x01);    // crc32 check failed
    return;
  }

  switch(static_cast<CMD>(msg[1])) {            // func code
    case CMD::READ_CRC32:        // 0xa2
      size = *(uint16_t*)(&msg[4]);
      pos = *(uint16_t*)(&msg[6]);
      read_crc32(pos, size);
      break;
    case CMD::READ:              // 0xac read full image
      size = *(uint16_t*)(&msg[4]);
      pos = *(uint16_t*)(&msg[6]);
      read(pos, size);
      break;
    case CMD::ERASE_CHIP:              // 0xaf erase chip
      erase_chip();
      break;
    case CMD::WRITE:              // 0xae write image
      size = *(uint16_t*)(&msg[4]);
      pos = *(uint16_t*)(&msg[6]);
      content = &msg[8];
      write(pos, size, content);
      break;
    case CMD::GET_ROM_SIZE:     // 0xa6
      _cbs.usart_write_f(&_rom_size, 2);
      break;
    case CMD::GET_MAX_PACKET_SIZE:    // 0xa9
      _cbs.usart_write_f(&_max_packet_size, 2);
      break;
  }
}
