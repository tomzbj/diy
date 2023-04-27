#include "misc.h"
#include "platform.h"
#include "nrf24l01_ble.h"

static void set_cs(int s)
{
//    _delay_us(2);
    s ? (GPIO_BOP(GPIOB) = GPIO_PIN_6) : (GPIO_BC(GPIOB) = GPIO_PIN_6);
//    _delay_us(2);
}

static void set_ce(int s)
{
//    _delay_us(2);
    s ? (GPIO_BOP(GPIOB) = GPIO_PIN_7) : (GPIO_BC(GPIOB) = GPIO_PIN_7);
//    _delay_us(2);
}

nrf24l01_cbs_t cbs = {SPI0_ReadWrite, set_cs, set_ce, _delay_us};

nrf24l01_ble ble(cbs);

void RF_Config(void)
{
    ble.begin();
    _dbg();
}

unsigned char to_bcd(unsigned char data)
{
    data = (data > 99) ? 99 : data;
    data = ((data / 10) << 4) | (data % 10);
    return data;
}

void RF_Poll(void)
{
    uint8_t data[4];    // = {"RABBIT"};
    data[0] = to_bcd(dc.data.RH / 100);
    data[1] = to_bcd(dc.data.RH % 100);
    data[2] = to_bcd(dc.data.T / 100);
    data[3] = to_bcd(dc.data.T % 100);


    ble.set_phone(PHONE_ANDROID);
    ble.set_mac((const unsigned char*)"\xaa\xbb\xcc\xdd\xee\xff");
    ble.set_name("SHT");
    ble.set_data(data, sizeof(data));
    ble.advertise();

    ble.pwr_down();
}
