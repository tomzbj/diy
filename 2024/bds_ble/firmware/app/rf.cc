#include "misc.h"
#include "nrf24l01_ble.h"
#include "platform.h"

#define  PIN_SCK  PB13
#define  PIN_CS  PB14
#define  PIN_DATA  PB15

static void set_cs(int s);
static void set_ce(int s);
static uint8_t SPI0_ReadWrite_t(uint8_t data);

nrf24l01_cbs_t cbs = {SPI0_ReadWrite_t, set_cs, set_ce, _delay_us};
NRF24L01 nrf(cbs);
NRF24L01_BLE ble(nrf);

static struct {
    uint8_t data[16];
    int data_size;
} g;

static uint8_t to_bcd(uint8_t data)
{
    data = (data > 99) ? 99 : data;
    data = ((data / 10) << 4) | (data % 10);
    return data;
}

uint32_t RF_GetUID(void)
{
    auto uid_addr = (uint8_t*)0x1ffff7ac;
    return ZCRC32::calc(uid_addr, 12);
}

static void set_cs(int s)
{
    _delay_us(2);
    ZPin::write(PIN_CS, s);
    _delay_us(2);
}

static void set_ce(int s)
{
}

static uint8_t SPI0_ReadWrite_t(uint8_t data)
{
    uint8_t ret = 0;

    _delay_us(10);
    ZPin::write(PIN_SCK, 0);
    _delay_us(10);
    for(int i = 0; i < 8; i++) {
        _delay_us(10);
        ZPin::write(PIN_DATA, (data & 0x80) ? 1 : 0);
        _delay_us(10);
        ZPin::write(PIN_SCK, 1);
        _delay_us(10);
        data <<= 1;
        ret <<= 1;
        if(ZPin::read (PIN_DATA))
            ret |= 1;
        _delay_us(10);
        ZPin::write(PIN_SCK, 0);
        _delay_us(10);
    }
    return ret;
}

void RF_Config(void)
{
    {
        // PB13-15 as SPI for Ci24R1
        __HAL_RCC_GPIOB_CLK_ENABLE();
        GPIO_InitTypeDef gis;
        gis.Pin = GPIO_PIN_13 | GPIO_PIN_14;
        gis.Mode = GPIO_MODE_OUTPUT_PP;
        gis.Pull = GPIO_PULLUP;
        gis.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOB, &gis);
        gis.Pin = GPIO_PIN_15;
        gis.Mode = GPIO_MODE_OUTPUT_OD;
        HAL_GPIO_Init(GPIOB, &gis);
    }
    {
        // PA8 as MCO Ci24R1使用16M MCO时钟
        __HAL_RCC_GPIOA_CLK_ENABLE();
        GPIO_InitTypeDef gis;
        gis.Pin = GPIO_PIN_8;
        gis.Mode = GPIO_MODE_AF_PP;
        gis.Pull = GPIO_PULLUP;
        gis.Speed = GPIO_SPEED_FREQ_HIGH;
        gis.Alternate = GPIO_AF0_MCO;
        HAL_GPIO_Init(GPIOA, &gis);
        HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);
    }
}

void RF_SetData(uint8_t* data, int size)
{
    if(size > 16)
        size = 16;
    memcpy(g.data, data, size);
    g.data_size = size;
}

void RF_Poll(void)
{
    _delay_ms(rand() % 10);

    char name[8];
    memset(name, 0, sizeof(name));
    sprintf(name, "SHT_%02d", int(RF_GetUID() % 100));    // 从芯片UID crc32后取余数作为蓝牙广播名后缀
    uint64_t mac = RF_GetUID() | 0xc00000000000ull;     // 蓝牙广播设备类型, 高两位为1

    {
        uint16_t vbat = adc.read() * 330 / 4096 * 2;    // 读取电池电压
        uint8_t data[8];
        if(bds.sync == true) {
            int h, m, s;
            rtc.get_time(&h, &m, &s);
            data[0] = to_bcd((h + 8) % 24);     // 调整为北京时间
            data[1] = to_bcd(m);
            data[2] = to_bcd(s);
        }
        else {
            data[0] = data[1] = data[2] = 0xff;
        }
        data[3] = to_bcd(sensor.RH / 100);
        data[4] = to_bcd(sensor.T / 100);
        data[5] = to_bcd(vbat / 100);
        data[6] = to_bcd(vbat % 100);
        data[7] = 0;
        uint8_t sum = 0;
        for(auto& n : data) {
            sum += n;
        }
        data[7] = ~sum + 1;     // 校验和
        RF_SetData(data, 8);
        for(auto& n : data)
            printf("%02X ", n);
        printf("\n");
    }

    ble.advertise(0x40, mac, name, g.data, g.data_size);
}
