#include "at28cxx.h"

uint8_t AT28CXX::read_byte(uint32_t addr)
{
//  _cbs.set_data_dir_output_f(0);    // 确保数据线为输入模式
  _cbs.write_addr_f(addr);
  _cbs.setce_f(0);
  _cbs.setoe_f(0);
  _cbs.delay_us_f(1);    // 芯片要求至少100ns左右，1us绰绰有余
  uint8_t val = _cbs.read_data_f();
  _cbs.setoe_f(1);
  _cbs.setce_f(1);
  return val;
}

// 仅仅完成总线上的物理写时序（不包含内部擦写等待）
void AT28CXX::write_byte_raw(uint32_t addr, uint8_t data)
{
//  _cbs.set_data_dir_output_f(1);    // 关键：确保数据线为输出模式
  _cbs.write_addr_f(addr);       // 1. 先送地址
  _cbs.write_data_f(data);       // 2. 先送数据 (修正错误1)
  _cbs.setce_f(0);
  _cbs.setoe_f(1);               // 确保OE为高
  _cbs.delay_us_f(1);

  _cbs.setwe_f(0);               // 3. 拉低WE开始写入脉冲
  _cbs.delay_us_f(1);            // 满足 tWP 脉冲宽度(最小100ns)
  _cbs.setwe_f(1);               // 4. 拉高WE锁存数据

  _cbs.setce_f(1);
  _cbs.delay_us_f(1);
}

// DATA Polling 硬件忙检测：避免死等10ms，大幅提高效率
void AT28CXX::wait_ready(uint32_t addr, uint8_t expect_data)
{
  uint8_t last_bit = expect_data & 0x80;
  // 限制最大超时时间，防止死循环 (约15ms)
  for(int timeout = 0; timeout < 15000; timeout++) {
    if((read_byte(addr) & 0x80) == last_bit) {
      return;    // 读出的D7等于写入的D7，说明芯片内部擦写完成
    }
    _cbs.delay_us_f(1);
  }
}

// 解锁软件数据保护 (SDP) 序列
void AT28CXX::unlock_sdp(void)
{
  write_byte_raw(0x5555, 0xAA);
  write_byte_raw(0x2AAA, 0x55);
  write_byte_raw(0x5555, 0xA0);
  _cbs.delay_us_f(10);    // 短暂延时
}

void AT28CXX::read(uint32_t addr, uint8_t* data, int count)
{
  for(int i = 0; i < count; i++) {
    data[i] = read_byte(addr + i);
  }
}

// 支持页写入(Page Write)的高效写入函数
void AT28CXX::write(uint32_t addr, const uint8_t* data, int count)
{
  while(count > 0) {
//    uint32_t page_start = addr;
    uint8_t last_written_data = 0;

    // 解锁SDP序列（如果每次写都带上这3步，可以防止中途写保护生效，这也是标准推荐做法）
    unlock_sdp();

    // AT28C256 每页 64 字节，页边界地址为 (addr & 0x3F)
    // 在一页内连续写入，字节间距必须小于 150us
    while(count > 0) {
      last_written_data = *data;
      write_byte_raw(addr, last_written_data);

      addr++;
      data++;
      count--;

      // 如果到达 64 字节页边界，必须断开，等待芯片内部烧写
      if((addr % 64) == 0) {
        break;
      }
    }
    // 这一页写完了，启动 DATA Polling 硬件等待擦写完成 (修正错误2)
    wait_ready(addr - 1, last_written_data);
  }
}
