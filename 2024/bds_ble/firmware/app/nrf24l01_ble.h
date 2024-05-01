#ifndef __RF24BLE_H__
#define __RF24BLE_H__

#include <stdint.h>
//#include <stdio.h>
#include "nrf24l01.h"

//using an enum might be good if we have more types
#define PHONE_ANDROID   0x42
#define PHONE_IPHONE    0x40

class NRF24L01_BLE {

protected:
    typedef uint8_t byte;
    constexpr static const byte rf_ch[3] = {2, 26, 80};
    constexpr static const byte le_ch[3] = {37, 38, 39};

    static const uint32_t ble_address = 0x6b7d9171;

    void packet_encode(uint8_t len, uint8_t chan);
    void whiten(uint8_t* data, uint8_t len, uint8_t whitenCoeff);
    void crc(const uint8_t* data, uint8_t len, uint8_t* output);
    uint8_t check_crc(uint8_t* input, uint8_t length);

    void set_mac(uint64_t mac);
    void set_phone(uint8_t phone_type);
    void set_name(const char* name);
    void set_data(const void* data, uint8_t dataLen);
    /* This function is a blocking function and it blocks for at least 4 ms */
//    void advertise();
public:
    NRF24L01_BLE(NRF24L01& nrf) : _nrf(nrf)
    {
    }

    void begin();
    void advertise(uint8_t phone_type, uint64_t mac, const char* name,
        const void* data, int len)
    {
        set_phone(phone_type);
        set_mac(mac);
        set_name(name);
        set_data(data, len);
        begin();
        for(int channel = 0; channel < 3; channel++)
            send_adv(channel);
    }
    void rx_begin(uint8_t payloadSize, uint8_t channel);
    uint8_t receive_packet(uint8_t* input, uint8_t length, uint8_t channel);

private:
    NRF24L01 _nrf;
    uint8_t _packet[32];    //maximum size of payload handled by nrf24L01 is 32
    uint8_t _length = 0;    //length of packet filled
    uint8_t _dataFieldStartPoint = 0;

    void print_packet();
    void send_adv(uint8_t channel);
    uint8_t get_packet_length();    //returns the current length of packet
    void pwr_down(void);

    static uint8_t byte_reverse(uint8_t byte)
    {
        byte = (byte >> 4) | (byte << 4);
        byte = ((byte & 0x33) << 2) | ((byte & 0xcc) >> 2);
        byte = ((byte & 0x55) << 1) | ((byte & 0xaa) >> 1);
        return byte;
    }
    static inline uint8_t whiten_start(uint8_t chan)
    {
        //use left shifted one
        return byte_reverse(chan) | 2;
    }

};

#endif
