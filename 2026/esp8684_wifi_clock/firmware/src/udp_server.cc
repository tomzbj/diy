#include "udp_server.h"

bool UDP_SERVER::begin(uint16_t port)
{
  _port = port;
  _len = 0;
  return _udp.begin(port) != 0;
}

int UDP_SERVER::poll(void)
{
  int n = _udp.parsePacket();
  if(n <= 0)
    return 0;

  if(n > (int)sizeof(_buf))
    n = sizeof(_buf);

  _peer_ip = _udp.remoteIP();
  _peer_port = _udp.remotePort();
  _len = _udp.read(_buf, n);
  return _len;
}

int UDP_SERVER::read(void* msg, int size)
{
  if(_len <= 0 || msg == nullptr || size <= 0)
    return 0;

  int n = (_len < size) ? _len : size;
  memcpy(msg, _buf, n);
  _len = 0;
  return n;
}

int UDP_SERVER::write(const void* msg, int size)
{
  if(msg == nullptr || size <= 0 || _peer_port == 0)
    return -1;

  if(!_udp.beginPacket(_peer_ip, _peer_port))
    return -1;
  int n = _udp.write((const uint8_t*)msg, size);
  if(!_udp.endPacket())
    return -1;
  return n;
}
