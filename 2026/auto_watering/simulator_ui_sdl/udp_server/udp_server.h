#ifndef _UDP_SERVER_H
#define _UDP_SERVER_H

#include <winsock2.h>
#include <cstdint>

#define BUFFER_SIZE 1024

class UDP_SERVER {
  public:

    UDP_SERVER(int port) : _port(port)
    {
    }

    int init(void);
    void poll(void);
    void quit(void);
    void write(uint8_t* data, int size);

  private:
    const int _port;
    char buffer[BUFFER_SIZE];
    SOCKET server_socket;
    struct sockaddr_in server, client;

    fd_set readfds;

};

#endif
