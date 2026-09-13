#include "rom.h"
#include <cstring>

ROM::ROM(const CBS& cbs) :
  _cbs(cbs), _data {0}
{
}

void ROM::init(void)
{
  _cbs.init_f();
  load();
}

void ROM::save(void)
{
  _cbs.save_f(_data, 16);
}

void ROM::load(void)
{
  uint8_t tmp[16];
  _cbs.load_f(tmp, 16);
  /* Virgin ZEEP page reads as 0xFFFF — treat as empty (zeros), same as init. */
  bool blank = true;
  for(int i = 0; i < 16; i++) {
    if(tmp[i] != 0xff) {
      blank = false;
      break;
    }
  }
  if(blank)
    memset(_data, 0, 16);
  else
    memcpy(_data, tmp, 16);
}

void ROM::update(void)
{
  auto addr = _cbs.addr_in_f();
  _cbs.data_out_f(_data[addr & 0xf]);

  if(0) {
    static uint8_t prev_addr = 0;
    if(addr != prev_addr) {
      _cbs.printf_f("%02X %02X\n", addr, _data[addr & 0xf]);
      prev_addr = addr;
    }
  }
}

void ROM::read_instructions(uint8_t* ins, int count)
{
  if(count > 16)
    count = 16;
  for(int i = 0; i < count; i++) {
    ins[i] = _data[i];
  }
}

void ROM::write_instructions(const uint8_t* ins, int count)
{
  if(count > 16)
    count = 16;
  for(int i = 0; i < count; i++)
    _data[i] = ins[i];
}
