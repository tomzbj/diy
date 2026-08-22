#ifndef _UDP_SERVER_H
#define _UDP_SERVER_H

#include <WiFi.h>
#include <WiFiUdp.h>
#include <cstdint>

// Thin wrapper around WiFiUDP: bind, poll one datagram, reply to last peer.
class UDP_SERVER {
  public:
    bool begin(uint16_t port);
    // Receive one datagram into internal buffer. Returns payload length, 0 if empty, <0 on error.
    int poll(void);
    int read(void* msg, int size);
    // Reply to last peer (from poll)
    int write(const void* msg, int size);

    IPAddress peer_ip(void) const
    {
      return _peer_ip;
    }
    uint16_t peer_port(void) const
    {
      return _peer_port;
    }
    uint16_t local_port(void) const
    {
      return _port;
    }

  private:
    WiFiUDP _udp;
    uint16_t _port = 0;
    IPAddress _peer_ip;
    uint16_t _peer_port = 0;
    uint8_t _buf[512];
    int _len = 0;
};

#endif
