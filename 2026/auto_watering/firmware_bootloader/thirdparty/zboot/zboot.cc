#include "zboot.h"
#include "misc.h"
#include <cstring>

ZBOOT::ZBOOT(const CBS& cbs, uint32_t flash_base, uint32_t flash_size,
  uint32_t page_size, uint32_t packet_size) : _cbs(cbs)
{
  extern uint32_t _edata, _sdata, __fini_array_end;

  _inf.flash_base = flash_base;
  _inf.flash_size = flash_size;
  _inf.page_size = page_size;
  _inf.max_packet_size = packet_size;
  _inf.bootloader_size = ((uint32_t)&__fini_array_end - _inf.flash_base)
    + ((uint32_t)&_edata - (uint32_t)&_sdata);
  _inf.app_base = _inf.flash_base + _inf.bootloader_size;
  if(_inf.app_base % _inf.page_size != 0)
    _inf.app_base += (_inf.page_size - _inf.app_base % _inf.page_size);
  _inf.max_app_size = _inf.flash_size - _inf.bootloader_size;
  if(_inf.max_app_size % _inf.page_size != 0)
    _inf.max_app_size -= _inf.max_app_size % _inf.page_size;
}

void ZBOOT::erase(bool echo)
{
  for(uint32_t ofs = 0; ofs < _inf.max_app_size; ofs += _inf.page_size) {
    if(!check_empty((void*)(_inf.app_base + ofs), _inf.page_size))
      _cbs.page_erase_f(_inf.app_base + ofs);
    if(echo) {
      uint8_t c = '.';
      _cbs.usart_write_f(&c, 1);
    }
  }
}

void ZBOOT::parse_cmd(const char* cmd, int len)
{
  const char* str_help =
    "empty: empty check.\n\
erase: erase whole app.\n\
reboot: reboot system.\n\
help: show this help.\n\
sysinfo: show system info.\n\n";

  while(*cmd == '#' || *cmd == ' ')
    cmd++;

  if(strcasecmp(cmd, "empty") == 0) {
    if(check_empty((void*)_inf.app_base, _inf.max_app_size))
      _cbs.printf_f("app empty.\n");
    else
      _cbs.printf_f("app not empty.\n");
  }
  else if(strcasecmp(cmd, "erase") == 0) {
    erase(true);
    _cbs.printf_f("done.\n");
  }
  else if(strcasecmp(cmd, "test") == 0) {
    _cbs.printf_f("ok\n");
  }
  else if(strcasecmp(cmd, "reboot") == 0) {
    _cbs.printf_f("done.\n");
    _cbs.reboot_f();
  }
  else if(strcasecmp(cmd, "help") == 0) {
    _cbs.printf_f(str_help);
  }
  else if(strcasecmp(cmd, "sysinfo") == 0) {
    _cbs.printf_f("FLASH SIZE:      %7u/%4uKB\n", _inf.flash_size,
      _inf.flash_size >> 10);
    _cbs.printf_f("PAGE SIZE:       %7u/%4uKB\n", _inf.page_size, _inf.page_size >> 10);
    _cbs.printf_f("BOOTLOADER SIZE: %7u/%4uKB\n", _inf.bootloader_size,
      _inf.bootloader_size >> 10);
    uint32_t app_size = get_app_size();
    _cbs.printf_f("APP SIZE:        %7u/%4uKB\n", app_size, app_size >> 10);
    _cbs.printf_f("MAX APP SIZE:    %7u/%4uKB\n", _inf.max_app_size,
      _inf.max_app_size >> 10);
    _cbs.printf_f("MAX PACKET SIZE: %7u/%4uKB\n", _inf.max_packet_size,
      _inf.max_packet_size >> 10);
    _cbs.printf_f("APP BASE ADDR:   0x%08lx\n", _inf.app_base);
    _cbs.printf_f("\n");
  }
}

bool ZBOOT::check_empty(void* addr, int size)
{
  uint32_t* paddr = (uint32_t*)addr;
  while(size > 0) {
    if(*paddr != 0xffff'ffff)
      return false;
    size -= 4;
    paddr++;
  }
  return true;
}

uint32_t ZBOOT::crc32(uint32_t crc, const uint8_t* msg, int size)
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

void ZBOOT::uputc(uint8_t c)
{
  _cbs.usart_write_f(&c, 1);
}

uint32_t ZBOOT::get_app_size(void)
{
  int ofs;
  for(ofs = _inf.max_app_size - 1; ofs >= 0; ofs--) {
    if(*(uint8_t*)(_inf.app_base + ofs) != 0xff)
      return ofs + 1;
  }
  return 0;
}

void ZBOOT::jump_to_app(void)
{
  __disable_irq();

  SysTick->CTRL = 0;
  SysTick->LOAD = 0;
  SysTick->VAL = 0;
  HAL_RCC_DeInit();
  for (int i = 0; i < 8; i++) 
    NVIC->ICPR[i] = 0xffffffff;
  static uint32_t stack_ptr = *(volatile uint32_t*)_inf.app_base;
  static uint32_t reset_handler = *(volatile uint32_t*)(_inf.app_base + 4);
  __asm volatile (
      "msr msp, %0 \n"    // 設置主堆棧
      "mov r0, #0  \n"
      "msr control, r0 \n"// 強制切換到 MSP
      "isb         \n"
      "bx %1       \n"// 跳轉到 Reset_Handler
      :
      : "r" (stack_ptr), "r" (reset_handler)
      : "r0", "memory"
      );

  typedef void (*func_app)(void);
  static func_app func = (func_app)(*(volatile uint32_t*)(_inf.app_base + 4));
  __set_MSP(*(volatile uint32_t*)(_inf.app_base));
  func();
}

void ZBOOT::parse_binary(const uint8_t* msg, int size)
{
  if(verify(msg, size) != RESULT::OK) {
    uputc(0x01);    // crc32 check failed
    return;
  }

  switch(static_cast<CMD>(msg[1])) {
    case CMD::GET_MAX_APPSIZE:    // 0x80
      _cbs.usart_write_f((uint8_t*)&_inf.max_app_size, 4);
      break;
    case CMD::READ_FLASH: {    // 0x81
                            uint32_t size = 0;
                            uint32_t ofs = 0;
                            memcpy(&ofs, msg + 4, 4);
                            memcpy(&size, msg + 8, 4);
                            if(ofs + size >= _inf.app_base + _inf.max_app_size)
                              uputc(0x02);    // overflow
                            else if((ofs & 0x3) || (size & 0x3))
                              uputc(0x03);    // both ofs & size need to bed aligned in 4 bytes
                            else
                              _cbs.usart_write_f((uint8_t*)(_inf.app_base + ofs), size);
                            break;
                          }
    case CMD::GET_APPSIZE: {    // 0x82
                             uint32_t app_size = get_app_size();
                             _cbs.usart_write_f((uint8_t*)&app_size, 4);
                             break;
                           }
    case CMD::READ_APP_CRC32: {    // 0x83
                                uint32_t size = 0;
                                uint32_t ofs = 0;
                                memcpy(&ofs, msg + 4, 4);
                                memcpy(&size, msg + 8, 4);

                                uint32_t crc = 0xffffffff;
                                crc = ~crc32(crc, (const uint8_t*)(_inf.app_base + ofs), size);
                                _cbs.usart_write_f((uint8_t*)&crc, 4);
                              }
                              break;
    case CMD::ERASE_APP:    // 0x84
                              erase(false);
                              uputc(0x00);
                              break;
    case CMD::WRITE_FLASH: {    // 0x85
                             uint16_t size = 0;
                             uint32_t ofs = 0;
                             memcpy(&size, msg + 2, 2);
                             memcpy(&ofs, msg + 4, 4);
                             size -= 12;    // header & crc
                             if(ofs + size >= _inf.app_base + _inf.max_app_size)
                               uputc(0x02);    // overflow
                             else if((ofs & 0x3) || (size & 0x3))
                               uputc(0x03);    // both ofs & size need to be aligned in 4 bytes
                             else {
                               _cbs.flash_program_f(_inf.app_base + ofs, msg + 8, size);
                               uputc(0x00);
                             }
                           }
                           break;
    case CMD::CHECK_EMPTY:    // 0x86
                           uputc((check_empty((void*)_inf.app_base, _inf.max_app_size) ? 0x00 : 0x01));
                           break;
    case CMD::JUMP_TO_APP: {    // 0x87
                             uputc(0x00);
                             jump_to_app();
                             break;
                           }
    case CMD::GET_MAX_PACKET_SIZE:    // 0x88
                           _cbs.usart_write_f((uint8_t*)&_inf.max_packet_size, 4);
                           break;
  }
}

ZBOOT::RESULT ZBOOT::verify(const uint8_t* msg, int size)
{
  uint32_t crc1 = 0, crc2 = 0xffffffff;
  memcpy(&crc1, &msg[size - 4], 4);
  crc2 = ~crc32(crc2, msg, size - 4);
  return (crc1 == crc2) ? (RESULT::OK) : (RESULT::ERROR);
}

bool ZBOOT::app_is_valid(void)
{
  extern uint32_t _estack;
  if(*(uint32_t*)_inf.app_base == (uint32_t)(&_estack))
    return true;
  else
    return false;
}
