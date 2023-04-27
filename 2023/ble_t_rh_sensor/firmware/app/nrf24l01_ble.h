#ifndef __RF24BLE_H__
#define __RF24BLE_H__

#include <stdint.h>
#include "nrf24l01.h"

#define DEBUG 0
#define RECV_TIMEOUT 200
#define RF24BLE_VALID 1
#define RF24BLE_CORRUPT 0
#define RF24BLE_TIMEOUT 0xFF

//using an enum might be good if we have more types
#define PHONE_ANDROID   0x42
#define PHONE_IPHONE    0x40

typedef unsigned char byte;

class nrf24l01_ble {

    static const byte rf_ch[];
    static const byte le_ch[];

protected:
    void packet_encode(uint8_t* packet, uint8_t len, uint8_t chan);
    void whiten(uint8_t* data, uint8_t len, uint8_t whitenCoeff);
    void crc(const uint8_t* data, uint8_t len, uint8_t* output);
    uint8_t check_crc(uint8_t* input, uint8_t length);

public:
    nrf24l01_ble(nrf24l01_cbs_t& cbs);
    void begin();
    void transmit_begin();
    void recvBegin(uint8_t payloadSize, uint8_t channel, uint64_t pipeAddress =
        0xe76b7d9171);
    void set_mac(uint8_t m0, uint8_t m1, uint8_t m2, uint8_t m3, uint8_t m4, uint8_t m5);
    void set_mac(const unsigned char mac[]);
    void set_phone(uint8_t phone_type);
    void set_name(const char* name);
    void set_data(const void* data, uint8_t dataLen);
    /*
     @brief
     This function is a blocking function and it blocks for at least 4 ms
     */
    void advertise();
    void send_adv(uint8_t channel);
    void print_packet();
    uint8_t get_packet_length();    //returns the current length of packet
    uint8_t receive_packet(uint8_t* input, uint8_t length, uint8_t channel);

    void pwr_down(void);

private:
    nrf24l01 _nrf;
    uint8_t _packet[32];    //maximum size of payload handled by nrf24L01 is 32
    uint8_t _length = 0;    //length of packet filled
    uint8_t _dataFieldStartPoint = 0;

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
