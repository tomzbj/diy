#include "nrf24l01.h"

#include "misc.h"
#include <string.h>

enum {
    CMD_READ_REG = 0x00,    // 读配置寄存器,低5位为寄存器地址
    CMD_WRITE_REG = 0x20,    // 写配置寄存器,低5位为寄存器地址
    CMD_READ_RX_PLD = 0x61,    // 读RX有效数据,1~32字节
    CMD_WRITE_TX_PLD = 0xa0,    // 写TX有效数据,1~32字节
    CMD_WRITE_TX_PLD_NO_ACK = 0xb0,    // 写TX有效数据,1~32字节
    CMD_FLUSH_TX = 0xe1,    // 清除TX FIFO寄存器.发射模式下用
    CMD_FLUSH_RX = 0xe2,    // 清除RX FIFO寄存器.接收模式下用
    CMD_REUSE_TX_PLD = 0xe3,    // 重新使用上一包数据,CE为高,数据包被不断发送.
    CMD_READ_RX_PLD_WIDTH = 0x60,    // R_RX_PL_WID
    CMD_WRITE_ACK_PLD = 0xa8,    // W_ACK_PAYLOAD, +3bit ACK pipe no(000 to 101)
    CMD_NOP = 0xff,    // 空操作,可以用来读状态寄存器
    REG_CONFIG = 0x00,    // 配置寄存器地址
    REG_EN_AA = 0x01,    // 使能自动应答功能
    REG_EN_RXADDR = 0x02,    // 接收地址允许
    REG_SETUP_AW = 0x03,    // 设置地址宽度(所有数据通道)
    REG_SETUP_RETR = 0x04,    // 建立自动重发
    REG_RF_CH = 0x05,    // RF通道
    REG_RF_SETUP = 0x06,    // RF寄存器
    REG_STATUS = 0x07,    // 状态寄存器
    REG_OBSERVE_TX = 0x08,    // 发送检测寄存器
    REG_CD = 0x09,    // 载波检测寄存器
    REG_RX_ADDR_P0 = 0x0a,    // 数据通道0接收地址
    REG_RX_ADDR_P1 = 0x0b,    // 数据通道1接收地址
    REG_RX_ADDR_P2 = 0x0c,    // 数据通道2接收地址
    REG_RX_ADDR_P3 = 0x0d,    // 数据通道3接收地址
    REG_RX_ADDR_P4 = 0x0e,    // 数据通道4接收地址
    REG_RX_ADDR_P5 = 0x0f,    // 数据通道5接收地址
    REG_TX_ADDR = 0x10,    // 发送地址寄存器
    REG_RX_PW_P0 = 0x11,    // 接收数据通道0有效数据宽度(1~32字节)
    REG_RX_PW_P1 = 0x12,    // 接收数据通道1有效数据宽度(1~32字节)
    REG_RX_PW_P2 = 0x13,    // 接收数据通道2有效数据宽度(1~32字节)
    REG_RX_PW_P3 = 0x14,    // 接收数据通道3有效数据宽度(1~32字节)
    REG_RX_PW_P4 = 0x15,    // 接收数据通道4有效数据宽度(1~32字节)
    REG_RX_PW_P5 = 0x16,    // 接收数据通道5有效数据宽度(1~32字节)
    REG_FIFO_STATUS = 0x17,    // FIFO状态寄存器
    REG_DYNPD = 0x1c,    // enable dynamic payload length in data pipes
    REG_FEATURE = 0x1d,    // global feature reg
    CMD_CE_ON = 0x70, CMD_CE_OFF = 0x71
};

//#define AD_LEN                  5
//#define TX_PLD_WIDTH        30
//#define _cbs.readwrite_f(x) NRF24L01__cbs.readwrite_f(x)

static void _saturate(int& val, int min, int max)
{
    val = (val < min) ? min : val;
    val = (val > max) ? max : val;
//    return val;
}

NRF24L01::NRF24L01(nrf24l01_cbs_t& cbs) : _cbs(cbs)
{
    _reg_config = 0;
}

void NRF24L01::set_rx_pw(int pipe, int pw)
{
    _saturate(pipe, 0, 5);
    _saturate(pw, 1, 32);
    write_reg(REG_RX_PW_P0 + pipe, pw);
}
void NRF24L01::NRF24L01::set_rx_addr2345(int pipe, uint8_t rxaddr)
{
    write_reg(REG_RX_ADDR_P0 + pipe, rxaddr);    //pipe>=2
}

//刷新24L01的TX FIFO
void NRF24L01::flush_tx_fifo(void)
{
    _cbs.cs_f(0);
    _cbs.readwrite_f(CMD_FLUSH_TX);
    _cbs.readwrite_f(0);
    _cbs.cs_f(1);
}
//刷新24L01的RX FIFO
void NRF24L01::flush_rx_fifo(void)
{
    _cbs.cs_f(0);
    _cbs.readwrite_f(CMD_FLUSH_RX);
    _cbs.readwrite_f(0);
    _cbs.cs_f(1);
}
//重用24L01的TX Payload
void NRF24L01::reuse_tx_pl(void)
{
    _cbs.cs_f(0);
    _cbs.readwrite_f(CMD_REUSE_TX_PLD);
    _cbs.cs_f(1);
}
//读取24L01的RX Payload长度
void NRF24L01::read_rx_pl_width(int length, uint8_t* txpl_width)
{
    _cbs.cs_f(0);
    _cbs.readwrite_f(CMD_READ_RX_PLD_WIDTH);
    txpl_width[0] = _cbs.readwrite_f(0xff);
    _cbs.cs_f(1);
}
//写24L01的ACK Payload(对接收端)
//参数: 管道号pipe(0到5), 读出长度length, 缓冲区指针txpl_buf 返回值:    状态
void NRF24L01::write_ack_tx_pl(int pipe, int len, uint8_t* txpl_buf)
{
    _cbs.cs_f(0);
    _cbs.readwrite_f(CMD_WRITE_ACK_PLD + pipe);
    for(int i = 0; i < len; i++)
        _cbs.readwrite_f(txpl_buf[i]);
    _cbs.cs_f(1);
}
//写24L01的TX Payload, 不需要ACK(对发送端)
//参数:       读出长度length, 缓冲区指针txpl_buf 返回值:    状态
void NRF24L01::write_noack_tx_pl(int length, uint8_t* txpl_buf)
{
    _cbs.cs_f(0);
    _cbs.readwrite_f(CMD_WRITE_TX_PLD_NO_ACK);
    for(int i = 0; i < length; i++) {
        _cbs.readwrite_f(txpl_buf[i]);
    }
    _cbs.cs_f(1);
}
//读取24L01的状态. 通过写一个NOP指令实现.
//---------------------底层操作结束----------------------

void NRF24L01::write_reg(uint8_t reg, uint8_t val)
{
    _cbs.cs_f(0);
    _cbs.readwrite_f(reg | CMD_WRITE_REG);
    _cbs.readwrite_f(val);
    _cbs.cs_f(1);
}

uint8_t NRF24L01::read_reg(uint8_t reg)
{
    uint8_t val;
    _cbs.cs_f(0);
    _cbs.readwrite_f(reg | CMD_READ_REG);
    val = _cbs.readwrite_f(0xff);
    _cbs.cs_f(1);
    return val;
}

void NRF24L01::write_data(uint8_t reg, const void* pbuf, int len)
{
    const uint8_t* buf = (const uint8_t*)pbuf;
    _cbs.cs_f(0);
    _cbs.readwrite_f(CMD_WRITE_REG | reg);
    for(int i = 0; i < len; i++) {
        _cbs.readwrite_f(buf[i]);
    }
    _cbs.cs_f(1);
}

void NRF24L01::read_data(uint8_t reg, void* pbuf, int len)
{
    uint8_t* buf = (uint8_t*)pbuf;
    _cbs.cs_f(0);
    _cbs.readwrite_f(reg | CMD_READ_REG);
    for(int i = 0; i < len; i++)
        buf[i] = _cbs.readwrite_f(0xff);
    _cbs.cs_f(1);
}

// 读24L01的RX:5Payload // 参数: 读出长度length, 缓冲区指针rxpl_buf 返回值:    状态
void NRF24L01::read_rx_payload(void* rx_pld, int len)
{
    uint8_t* p = (uint8_t*)rx_pld;
    _cbs.cs_f(0);
    _cbs.readwrite_f(CMD_READ_RX_PLD);
    for(int i = 0; i < len; i++) {
        p[i] = _cbs.readwrite_f(0xff);
    }
    _cbs.cs_f(1);
}

void NRF24L01::set_as_tx(void)
{
//    write_reg(REG_CONFIG, 0x0e);
    _reg_config = (_reg_config & 0xfc) | 0x2;
    write_reg(REG_CONFIG, _reg_config);
}
void NRF24L01::set_as_rx(void)
{
//    write_reg(REG_CONFIG, 0x0f);
    _reg_config = (_reg_config & 0xfc) | 0x3;
    write_reg(REG_CONFIG, _reg_config);
}

static const uint8_t addr_reg[] = {REG_TX_ADDR, REG_RX_ADDR_P0,
    REG_RX_ADDR_P1};

void NRF24L01::set_crc(int state)
{
//    uint8_t val = read_reg(REG_CONFIG);
//    if(state) val |= 0x08;
//    val = (state) ? (val | 0x08) : (val & ~0x08);
//    write_reg(REG_CONFIG, val);
//    if(state) _reg_config |= 0x8; else _reg_config
    _reg_config = (state) ? (_reg_config | 0x8) : (_reg_config & ~0x8);
//    _reg_config = (_reg_config & 0xfc) | 0x3;
    write_reg(REG_CONFIG, _reg_config);
}

// addr width must be 4
void NRF24L01::set_addr(int type, uint32_t addr)
{
    _saturate(type, 0, 2);
    write_data(addr_reg[type], &addr, 4);
}

void NRF24L01::set_addr(int type, const uint8_t* addr)
{
    _saturate(type, 0, 2);
    write_data(addr_reg[type], addr, 5);
}

void NRF24L01::get_addr(int type, uint8_t* addr)
{
    _saturate(type, 0, 2);
    read_data(addr_reg[type], addr, 5);
}

void NRF24L01::set_tx_power(int power)
{
    _saturate(power, 0, NRF24L01_TX_POWER_MAX);
    uint8_t val = read_reg(REG_RF_SETUP);
    val &= 0xf9;
    val |= (power << 1);
    write_reg(REG_RF_SETUP, val);
}

void NRF24L01::set_data_rate(int data_rate)
{
    _saturate(data_rate, 0, NRF24L01_DATA_RATE_MAX);
    uint8_t val = read_reg(REG_RF_SETUP);
    const uint8_t mask[] = {
        //
        [NRF24L01_DATA_RATE_250KBPS] = 0x20, [NRF24L01_DATA_RATE_1MBPS] = 0x00,
        [NRF24L01_DATA_RATE_2MBPS]= 0x08};
    val &= 0xd7;
    val |= mask[data_rate];
    write_reg(REG_RF_SETUP, val);
}

void NRF24L01::set_en_autoack(int ch, int state)
{
    _saturate(ch, 0, 5);
    uint8_t val = read_reg(REG_EN_AA);
    val = (state) ? (val | (1 << ch)) : (val & ~(1 << ch));
//    if(state) val |= (1 << ch); else val &= ~(1 << ch);
    write_reg(REG_EN_AA, val);
}

void NRF24L01::set_en_rxaddr(int ch, int state)
{
    _saturate(ch, 0, 5);
    uint8_t val = read_reg(REG_EN_RXADDR);
    val = (state) ? (val | (1 << ch)) : (val & ~(1 << ch));
//    if(state) val |= (1 << ch); else val &= ~(1 << ch);
    write_reg(REG_EN_RXADDR, val);
}

void NRF24L01::set_retry(int retry_delay, int retry_count)
{
    _saturate(retry_delay, 0, 15);
    _saturate(retry_count, 0, 15);
    write_reg(REG_SETUP_RETR, (retry_delay << 4) | retry_count);
}

void NRF24L01::set_rf_ch(int rf_ch)
{
    _saturate(rf_ch, 0, 125);
    write_reg(REG_RF_CH, rf_ch);
}

void NRF24L01::set_addr_width(int addr_width)
{
    _saturate(addr_width, 3, 5);
    write_reg(REG_SETUP_AW, addr_width - 2);
}

uint8_t NRF24L01::get_status(void)
{
    return read_reg(REG_STATUS);
}

uint8_t NRF24L01::get_fifo_status(void)
{
    return read_reg(REG_FIFO_STATUS);
}

uint8_t NRF24L01::get_observe_tx(void)
{
    return read_reg(REG_OBSERVE_TX);
}

void NRF24L01::clear_irq(uint8_t irq)
{
    write_reg(REG_STATUS, irq);
}

void NRF24L01::read_regs(
    void (*view_f)(uint8_t addr_reg, void* vals, int width))
{
    uint8_t reg, buf[5];
    int len;
    for(reg = REG_CONFIG; reg <= REG_FEATURE; reg++) {
        if(reg == REG_RX_ADDR_P0 || reg == REG_RX_ADDR_P1 || reg == REG_TX_ADDR)
            len = 5;
        else
            len = 1;
        read_data(reg, buf, len);
        view_f(reg, buf, len);
    }
}

void NRF24L01::set_soft_ce(bool state)
{
    _cbs.cs_f(0);
    _cbs.readwrite_f(state ? CMD_CE_ON : CMD_CE_OFF);
    _cbs.cs_f(1);
    _cbs.delay_us_f(1000UL);
}

void NRF24L01::tx(const void* buf, int len)
{
    set_ce(0);
    flush_tx_fifo();
    write_data(CMD_WRITE_TX_PLD, buf, len);
    set_as_tx();
    set_ce(1);
    _cbs.delay_us_f(50UL);
    set_ce(0);
}

void NRF24L01::rx(void)
{
    flush_rx_fifo();
    clear_irq(0x70);

//    set_rx_pw(0, 32);
    set_as_rx();
//    _cbs.ce_f(0);
    set_ce(1);
}

void NRF24L01::pwr_up(void)
{
    _reg_config |= 0x2;
    write_reg(REG_CONFIG, _reg_config);
}

void NRF24L01::pwr_down(void)
{
    _reg_config &= ~0x2;
    write_reg(REG_CONFIG, _reg_config);
}
