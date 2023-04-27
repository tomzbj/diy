#ifndef _NRF24L01_H
#define _NRF24L01_H

enum {
    NRF24L01_DATA_RATE_250KBPS, NRF24L01_DATA_RATE_1MBPS,
    NRF24L01_DATA_RATE_2MBPS,
    NRF24L01_DATA_RATE_MAX = NRF24L01_DATA_RATE_2MBPS,
    //
    NRF24L01_STATE_RX, NRF24L01_STATE_TX
};

enum {
    NRF24L01_TX_ADDR, NRF24L01_RX_ADDR_P0, NRF24L01_RX_ADDR_P1
};

enum {
    NRF24L01_TX_POWER_N18 = 0, NRF24L01_TX_POWER_N12 = 1,
    NRF24L01_TX_POWER_N6 = 2, NRF24L01_TX_POWER_0 = 3,
    NRF24L01_TX_POWER_MAX = NRF24L01_TX_POWER_0
};

typedef struct {
    unsigned char (*readwrite_f)(unsigned char);
    void (*cs_f)(int);
    void (*ce_f)(int);
    void (*delay_us_f)(volatile int);
} nrf24l01_cbs_t;

class nrf24l01 {
public:
    nrf24l01(nrf24l01_cbs_t& cbs);

    void init(void);
    void set_addr(int type, unsigned long addr);
    void set_addr(int type, const unsigned char* addr);
    void get_addr(int type, unsigned char* addr);
    void set_data_rate(int data_rate);
    void set_en_autoack(int ch, int state);
    void set_en_rxaddr(int ch, int state);
    void set_retry(int retry_delay, int retry_count);    // retry_delay unit: 250us
    void set_rf_ch(int rf_ch);
    void set_addr_width(int addr_width);
    void set_crc(int state);
    void set_tx_power(int power);

    void read_regs(void (*view_f)(unsigned char reg, void* vals, int width));
    void read_rx_payload(void* rxpl_buf, int len);

    void read_rx_pl_width(int length, unsigned char* txpl_width);
    void write_noack_tx_pl(int length, unsigned char* txpl_buf);
    void write_ack_tx_pl(int pipe, int len, unsigned char* txpl_buf);
    unsigned char get_fifo_status(void);
    unsigned char get_status(void);
    void clear_irq(unsigned char irq);
    unsigned char get_observe_tx(void);
    void tx(const void* buf, int len);
    void rx(void);

    void set_rx_pw(int pipe, int pw);
    void set_rx_addr2345(int pipe, unsigned char rxaddr);

    void pwr_up(void);
    void pwr_down(void);

    void flush_tx_fifo(void);
    void flush_rx_fifo(void);
    void reuse_tx_pl(void);

    void set_as_tx(void);
    void set_as_rx(void);

private:
    nrf24l01_cbs_t _cbs;
    unsigned char _reg_config;

    void write_reg(unsigned char reg, unsigned char val);
    void write_data(unsigned char reg, const void* pbuf, int len);
    unsigned char read_reg(unsigned char reg);
    void read_data(unsigned char reg, void* buf, int len);
};

#endif
