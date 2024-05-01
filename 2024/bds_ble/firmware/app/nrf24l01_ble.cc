//Special thanks to Dmitry Grinberg for completely inspiring this project

#include "NRF24L01_BLE.h"

#include "misc.h"

#include <string.h>
#include <stdio.h>

void NRF24L01_BLE::crc(const uint8_t* data, uint8_t dataLen, uint8_t* outputcrc)
{
    uint8_t i, tmp, tmp_data;    // calculating the CRC based on a LFSR

    while(dataLen--) {
        tmp_data = *data++;
        for(i = 0; i < 8; i++, tmp_data >>= 1) {
            tmp = outputcrc[0] >> 7;

            outputcrc[0] <<= 1;
            if(outputcrc[1] & 0x80) {
                outputcrc[0] |= 1;
            }
            outputcrc[1] <<= 1;
            if(outputcrc[2] & 0x80) {
                outputcrc[1] |= 1;
            }
            outputcrc[2] <<= 1;

            if(tmp != (tmp_data & 1)) {
                outputcrc[2] ^= 0x5B;
                outputcrc[1] ^= 0x06;
            }
        }
    }
}

uint8_t NRF24L01_BLE::check_crc(uint8_t* input, uint8_t len)
{
    uint8_t crc[3] = {0x55, 0x55, 0x55};    //initial value for bluetooth crc
    len -= 3;
    this->crc(input, len, crc);
    if(crc[0] == input[len - 3] && crc[1] == input[len - 2]
        && crc[2] == input[len - 1]) {
        return 1;
    }
    return 0;
}

void NRF24L01_BLE::whiten(uint8_t* data, uint8_t len, uint8_t whitenCoeff)
{
    uint8_t m;    // Implementing whitening with LFSR
    while(len--) {
        for(m = 1; m; m <<= 1) {
            if(whitenCoeff & 0x80) {
                whitenCoeff ^= 0x11;
                (*data) ^= m;
            }
            whitenCoeff <<= 1;
        }
        data++;
    }
}
void NRF24L01_BLE::packet_encode(uint8_t len, uint8_t chan)
{
    // Assemble the packet to be transmitted
    // Packet length includes pre-populated crc
    uint8_t i, dataLen = len - 3;
    crc(_packet, dataLen, &_packet[dataLen]);
    for(i = 0; i < 3; i++, dataLen++)
        _packet[dataLen] = byte_reverse(_packet[dataLen]);
    whiten(_packet, len, whiten_start(chan));
    for(i = 0; i < len; i++)
        _packet[i] = byte_reverse(_packet[i]);    // the byte order of the packet should be reversed as well
}

void NRF24L01_BLE::begin()
{
    _nrf.pwr_up();
    _nrf.set_crc(0);
    _nrf.set_en_autoack(0, 0);
    _nrf.set_en_autoack(1, 0);
    _nrf.set_en_autoack(2, 0);
    _nrf.set_en_autoack(3, 0);
    _nrf.set_en_autoack(4, 0);
    _nrf.set_en_autoack(5, 0);

    _nrf.set_en_rxaddr(0, 0);
    _nrf.set_en_rxaddr(1, 0);
    _nrf.set_en_rxaddr(2, 0);
    _nrf.set_en_rxaddr(3, 0);
    _nrf.set_en_rxaddr(4, 0);
    _nrf.set_en_rxaddr(5, 0);

    _nrf.set_addr_width(4);
    _nrf.set_rx_pw(0, 32);
    _nrf.set_retry(0, 0);
    _nrf.set_data_rate(NRF24L01_DATA_RATE_1MBPS);
    _nrf.set_tx_power(NRF24L01_TX_POWER_MAX);

    _nrf.set_addr(NRF24L01_TX_ADDR, ble_address);
    _nrf.set_addr(NRF24L01_RX_ADDR_P0, ble_address);
    _nrf.set_addr(NRF24L01_RX_ADDR_P1, ble_address);
}

void NRF24L01_BLE::rx_begin(uint8_t payloadSize, uint8_t channel)
{
    begin();
    _nrf.set_rf_ch(NRF24L01_BLE::rf_ch[channel]);
    _nrf.set_rx_pw(0, payloadSize);
    _nrf.set_en_rxaddr(0, 1);
    _nrf.set_addr_width(4);
    _nrf.set_addr(NRF24L01_RX_ADDR_P0, ble_address);
    _nrf.set_addr(NRF24L01_RX_ADDR_P1, ble_address);
    _delay_ms(5);
    _nrf.rx();
    _delay_ms(5);
}

void NRF24L01_BLE::set_phone(uint8_t phone_type)
{
    _packet[0] = phone_type;
}

void NRF24L01_BLE::set_mac(uint64_t mac)
{
    _length = 2;

    memcpy(&_packet[2], &mac, 6);
    _length += 6;
    //length should be 8 by now
    //flags (LE-only, limited discovery mode)
    _packet[_length++] = 2;    //flag length
    _packet[_length++] = 0x01;    //data type
    _packet[_length++] = 0x05;    //actual flag
}

void NRF24L01_BLE::set_name(const char* name)
{
    // name must be set only once
    //8,9,10 bytes are for flags
    //name field starts from 11th byte
    int len = strlen(name);
    if(len != 0) {
        //length of name including the terminating null character
        _packet[_length++] = len + 1;
        _packet[_length++] = 0x08;    //name type short name
        for(uint8_t i = 0; i < len; i++) {
            _packet[_length++] = name[i];
        }
    }
    //else no name to be sent
    //directly send manufacturer specific data 0xFF
}

void NRF24L01_BLE::set_data(const void* data, uint8_t dataLen)
{
//    _dataFieldStartPoint = _dataFieldStartPoint == 0 ? _length : _dataFieldStartPoint;
//    _length = _dataFieldStartPoint;
//    const uint8_t* current = reinterpret_cast<const uint8_t*>(data);

#if DEBUG == 1 
	Serial.print("data "); Serial.println(dataLen);
#endif
    _packet[_length++] = dataLen + 1;
    _packet[_length++] = 0xFF;		//data type
    memcpy(&_packet[_length], data, dataLen);
    _length += dataLen;
    _packet[_length++] = 0x55;
    _packet[_length++] = 0x55;
    _packet[_length++] = 0x55;
}

/* This function blocks the calling thread for atleast 4 milliseconds */
//void NRF24L01_BLE::advertise(void) { }
void NRF24L01_BLE::send_adv(uint8_t channel)
{
    if(_length > 32) {
        printf("ADV FAIL! Packet too Long %d\n", _length);
        return;
    }
    if(channel > 2 || channel < 0) {
        printf("Incorrect channel value, use RF24BLE.advertise()\n");
        return;
    }

    _nrf.set_rf_ch(NRF24L01_BLE::rf_ch[channel]);

    _packet[1] = _length - 5;    //subtract checksum bytes and the 2 bytes including the length byte and the first byte
    packet_encode(_length, NRF24L01_BLE::le_ch[channel]);
    _nrf.tx(_packet, _length);
}

void NRF24L01_BLE::print_packet(void)
{
    for(uint8_t i = 0; i < _length; i++) {
        printf("%02x ", _packet[i]);
    }
    printf("\n");
}
uint8_t NRF24L01_BLE::get_packet_length(void)
{
    return _length;
}

uint8_t NRF24L01_BLE::receive_packet(uint8_t* input, uint8_t len,
    uint8_t channel)
{
    if((_nrf.get_status() & 0x40) && !(_nrf.get_fifo_status() & 0x1)) {
        _nrf.read_rx_payload(input, len);
        _nrf.flush_rx_fifo();
        _nrf.clear_irq(0x70);
    }
    uint8_t i, dataLen = len - 3;
    for(i = 0; i < len; i++) {
        input[i] = byte_reverse(input[i]);
    }
    whiten(input, len, whiten_start(NRF24L01_BLE::le_ch[channel]));
    for(i = 0; i < 3; i++, dataLen++) {
        input[dataLen] = byte_reverse(input[dataLen]);
    }
    return check_crc(input, len);
}

void NRF24L01_BLE::pwr_down(void)
{
    _nrf.pwr_down();
}
