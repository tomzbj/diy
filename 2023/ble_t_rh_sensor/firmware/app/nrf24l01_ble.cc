//Special thanks to Dmitry Grinberg for completely inspiring this project

#include "nrf24l01_ble.h"
#include "misc.h"

#include <string.h>
#include <stdio.h>

#define BYTELEN 8

const byte nrf24l01_ble::rf_ch[] = {2, 26, 80};
const byte nrf24l01_ble::le_ch[] = {37, 38, 39};

nrf24l01_ble::nrf24l01_ble(nrf24l01_cbs_t& cbs) : _nrf(cbs)
{
}

void nrf24l01_ble::crc(const uint8_t* data, uint8_t dataLen, uint8_t* outputcrc)
{
    // calculating the CRC based on a LFSR
    uint8_t i, temp, tempData;

    while(dataLen--) {
        tempData = *data++;
        for(i = 0; i < 8; i++, tempData >>= 1) {
            temp = outputcrc[0] >> 7;

            outputcrc[0] <<= 1;
            if(outputcrc[1] & 0x80) {
                outputcrc[0] |= 1;
            }
            outputcrc[1] <<= 1;
            if(outputcrc[2] & 0x80) {
                outputcrc[1] |= 1;
            }
            outputcrc[2] <<= 1;

            if(temp != (tempData & 1)) {
                outputcrc[2] ^= 0x5B;
                outputcrc[1] ^= 0x06;
            }
        }
    }
}

uint8_t nrf24l01_ble::check_crc(uint8_t* input, uint8_t len)
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

void nrf24l01_ble::whiten(uint8_t* data, uint8_t len, uint8_t whitenCoeff)
{
    // Implementing whitening with LFSR
    uint8_t m;
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
void nrf24l01_ble::packet_encode(uint8_t* packet, uint8_t len, uint8_t chan)
{
    // Assemble the packet to be transmitted
    // Packet length includes pre-populated crc
    uint8_t i, dataLen = len - 3;
    crc(packet, dataLen, packet + dataLen);
    for(i = 0; i < 3; i++, dataLen++)
        packet[dataLen] = byte_reverse(packet[dataLen]);
    whiten(packet, len, whiten_start(chan));
    for(i = 0; i < len; i++)
        packet[i] = byte_reverse(packet[i]);    // the byte order of the packet should be reversed as well
}
void nrf24l01_ble::begin()
{
//    _radio.begin();
    transmit_begin();
}

void nrf24l01_ble::transmit_begin()
{
    _nrf.pwr_up();
    _nrf.set_crc(0);
    _nrf.set_en_autoack(0, 0);
    _nrf.set_en_autoack(1, 0);
    _nrf.set_en_autoack(2, 0);
    _nrf.set_en_autoack(3, 0);
    _nrf.set_en_autoack(4, 0);
    _nrf.set_en_autoack(5, 0);
//    _radio.stopListening();
    _nrf.set_addr_width(4);
    _nrf.set_rx_pw(0, 32);
    _nrf.set_retry(0, 0);
    _nrf.set_data_rate(NRF24L01_DATA_RATE_1MBPS);
    _nrf.set_tx_power(NRF24L01_TX_POWER_MAX);

    // Set access addresses (TX address in nRF24L01) to BLE advertising 0x8E89BED6
    // Both bit and byte orders are reversed for BLE packet format
    unsigned long address;
    address = byte_reverse(0xD6);
    address <<= BYTELEN;
    address |= byte_reverse(0xBE);
    address <<= BYTELEN;
    address |= byte_reverse(0x89);
    address <<= BYTELEN;
    address |= byte_reverse(0x8E);

    _nrf.set_addr(NRF24L01_TX_ADDR, address);
    _nrf.set_addr(NRF24L01_RX_ADDR_P0, address);
    _nrf.set_addr(NRF24L01_RX_ADDR_P1, address);
}
void nrf24l01_ble::recvBegin(uint8_t payloadSize, uint8_t channel,
    uint64_t pipeAddress)
{
    begin();
    _nrf.set_rf_ch(nrf24l01_ble::rf_ch[channel]);
    _nrf.set_rx_pw(0, payloadSize);
    _nrf.rx();
}

void nrf24l01_ble::set_phone(uint8_t phone_type)
{
    //byte no.0 PDU
    _packet[0] = phone_type;
}

void nrf24l01_ble::set_mac(const unsigned char mac[])
{
    _length = 2;

    memcpy(&_packet[2], mac, 6);
    _length += 6; //length should be 8 by now
    //flags (LE-only, limited discovery mode)
    _packet[_length++] = 2;    //flag length
    _packet[_length++] = 0x01;    //data type
    _packet[_length++] = 0x05;    //actual flag
}

void nrf24l01_ble::set_mac(uint8_t m0, uint8_t m1, uint8_t m2, uint8_t m3,
    uint8_t m4, uint8_t m5)
{
    //length of payload is entered in byte no.1
    _length = 2;

    _packet[_length++] = m0;
    _packet[_length++] = m1;
    _packet[_length++] = m2;
    _packet[_length++] = m3;
    _packet[_length++] = m4;
    _packet[_length++] = m5;
    //length should be 8 by now
    //flags (LE-only, limited discovery mode)
    _packet[_length++] = 2;    //flag length
    _packet[_length++] = 0x01;    //data type
    _packet[_length++] = 0x05;    //actual flag
}

void nrf24l01_ble::set_name(const char* name)
{
    // name must be set only once
    //8,9,10 bytes are for flags
    //name field starts from 11th byte
    if(strlen(name) != 0) {
        //length of name including the terminating null character
        _packet[_length++] = strlen(name) + 1;
        _packet[_length++] = 0x08;    //name type short name
        for(uint8_t i = 0; i < strlen(name); i++) {
            _packet[_length++] = name[i];
        }
    }
    //else
    //no name to be sent
    //directly send manufacturere specific data 0xFF
    //do nothing here
}

void nrf24l01_ble::set_data(const void* data, uint8_t dataLen)
{
    _dataFieldStartPoint =
        _dataFieldStartPoint == 0 ? _length : _dataFieldStartPoint;
    _length = _dataFieldStartPoint;
//    const uint8_t* current = reinterpret_cast<const uint8_t*>(data);

#if DEBUG == 1 
	Serial.print("data "); Serial.println(dataLen);
#endif
    _packet[_length++] = dataLen + 1;
    _packet[_length++] = 0xFF;		//data type
//    for(uint8_t i = 0; i < dataLen; i++) { _packet[_length++] = *(current); current++; }
    memcpy(&_packet[_length], data, dataLen);
    _length += dataLen;
    //CRC is appended to the data
    //CRC starting val 0x555555 acc. to spec
    _packet[_length++] = 0x55;
    _packet[_length++] = 0x55;
    _packet[_length++] = 0x55;

}

/* This function blocks the calling thread for atleast 4 milliseconds */
void nrf24l01_ble::advertise(void)
{
    transmit_begin();
    for(uint8_t channel = 0; channel < 3; channel++)
        send_adv(channel);
}

void nrf24l01_ble::send_adv(uint8_t channel)
{
    if(_length > 32) {
        printf("ADV FAIL! Packet too Long %d\n", _length);
        return;
    }
    if(channel > 2 || channel < 0) {
        printf("Incorrect channel value, use RF24BLE.advertise()\n");
        return;
    }

    _nrf.set_rf_ch(nrf24l01_ble::rf_ch[channel]);

    _packet[1] = _length - 5;    //subtract checksum bytes and the 2 bytes including the length byte and the first byte
    packet_encode(_packet, _length, nrf24l01_ble::le_ch[channel]);
    _nrf.tx(_packet, _length);
}

void nrf24l01_ble::print_packet(void)
{
    for(uint8_t i = 0; i < _length; i++) {
        printf("%02x ", _packet[i]);
    }
    printf("\n");
}
uint8_t nrf24l01_ble::get_packet_length(void)
{
    return _length;
}

uint8_t nrf24l01_ble::receive_packet(uint8_t* input, uint8_t len,
    uint8_t channel)
{
    uint8_t i, dataLen = len - 3;
    //reversing the bits of the complete packet
    for(i = 0; i < len; i++) {
        input[i] = byte_reverse(input[i]);
    }
    //de-whiten the packet using the same polynomial
    whiten(input, len, whiten_start(nrf24l01_ble::le_ch[channel]));
    //reversing bits of the crc
    for(i = 0; i < 3; i++, dataLen++) {
        input[dataLen] = byte_reverse(input[dataLen]);
    }
    return check_crc(input, len);
}

void nrf24l01_ble::pwr_down(void)
{
    _nrf.pwr_down();
}
