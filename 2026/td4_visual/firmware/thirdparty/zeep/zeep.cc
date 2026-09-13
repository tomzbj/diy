/* Author: tomzbj  Updated: 2024-04-18 Rev: 0.02 */

#include "zeep.h"
#include "misc.h"
#include <memory>

int ZEEP::get_first_empty_addr(void) const
{
  uint16_t i;
  for(i = 0; i < _page_size; i += 4) {
    if(_cbs.word_read_f(_base_addr + i) == 0xffffffff)
//    if(*(size_t*)MAIN_ADDR(i) == 0xffffffff)
      return i;
  }
  return -1;    // Full
}

void ZEEP::rotate(void) const
{
//  auto buf = std::vector<uint16_t>(EEPROM_SIZE, 0);
  auto buf = std::make_unique<uint16_t[]>(EEPROM_SIZE);
  this->read(0, buf.get(), EEPROM_SIZE);

  _cbs.page_erase_f(_base_addr);
  for(size_t i = 0; i < EEPROM_SIZE; i += 2) {
    auto data = (uint32_t)buf.get()[i / 2];
    data <<= 16;
    _cbs.word_program_f(_base_addr + i * 2, i | data);
  }
}

void ZEEP::write_word(uint16_t dummy_addr, uint16_t data) const
{
  if(dummy_addr > MAX_DUMMY_ADDR) {
    return;     // do nothing
  }
  int ofs = get_first_empty_addr();
  if(ofs == -1) {            // Page full, rotate needed
    rotate();
    ofs = get_first_empty_addr();
  }
  uint16_t orig_data = read_word(dummy_addr);
  if(orig_data == data)
    return;            // skip writing if data unchanged

  _cbs.word_program_f(_base_addr + ofs, dummy_addr | (((uint32_t)data) << 16));
}

uint16_t ZEEP::read_word(uint16_t dummy_addr) const
{
  int ofs = get_first_empty_addr();
  if(ofs == 0 || ofs == -1)
    ofs = _page_size - 4;    // position to the end
  while(ofs >= 0) {
    uint32_t data = _cbs.word_read_f(_base_addr + ofs);
    if((data & 0xffff) == dummy_addr)
      return data >> 16;
    ofs -= 4;
  }
  return 0xffff;              // Dummy_addr not found
}

void ZEEP::write(uint16_t addr, const void* data, int nbytes) const
{
  uint16_t* pdata = (uint16_t*)data;
  if(nbytes % 2 != 0)
    nbytes++;
  while(nbytes > 0) {
    write_word(addr, *pdata);
    addr += 2;
    pdata++;
    nbytes -= 2;
  }
}

void ZEEP::read(uint16_t addr, void* data, int nbytes) const
{
  uint16_t* pdata = (uint16_t*)data;
  addr &= (~1);
  if(nbytes % 2 != 0)
    nbytes++;
  while(nbytes > 0) {
    *pdata = read_word(addr);
    addr += 2;
    pdata++;
    nbytes -= 2;
  }
}

uint16_t ZEEP::get_size(void) const
{
  return EEPROM_SIZE;
}

void ZEEP::erase_all(void) const
{
  _cbs.page_erase_f(_base_addr);
}
