#include "misc.h"
#include "udp_server.h"

int UDP_SERVER::init(void)
{
  WSADATA wsa;

  // 初始化Winsock
  printf("Initializing Winsock...\n");
  if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
    printf("Failed. Error Code: %d\n", WSAGetLastError());
    return 1;
  }
  printf("Initialized.\n");

  // 创建socket
  if((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
    printf("Could not create socket: %d\n", WSAGetLastError());
    return 1;
  }
  printf("Socket created.\n");

  // 准备sockaddr_in结构
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(_port);

  // 绑定
  if(bind(server_socket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
    printf("Bind failed with error code: %d\n", WSAGetLastError());
    closesocket(server_socket);
    WSACleanup();
    return 1;
  }
  printf("Bind done.\n");

  // 持续接收数据
  printf("Waiting for data...\n");

  return 0;
}

void UDP_SERVER::write(uint8_t* data, int size)
{
  sendto(server_socket, (const char*)data, size, 0, (struct sockaddr*)&client,
    sizeof(client));
}

void UDP_SERVER::poll(void)
{
  FD_ZERO(&readfds);
  FD_SET(server_socket, &readfds);

  // 设置超时时间为100毫秒
  struct timeval timeout;

  timeout.tv_sec = 0;
  timeout.tv_usec = 1000;

  // 使用select检查socket是否可读
  int result = select(0, &readfds, NULL, NULL, &timeout);
  if(result == SOCKET_ERROR) {
//    printf("select() failed: %d\n", WSAGetLastError());
    return;
  }
  else if(result == 0) {
    return;    // 超时，没有数据可读
  }

  // 有数据可读
  if(FD_ISSET(server_socket, &readfds)) {
    memset(buffer, 0, BUFFER_SIZE);    // 清除缓冲区

    int client_len = sizeof(client);    // 接收数据
    int recv_len = recvfrom(server_socket, buffer, BUFFER_SIZE, 0,
      (struct sockaddr*)&client, &client_len);

    if(recv_len == SOCKET_ERROR) {
//      printf("recvfrom() failed with error code: %d\n", WSAGetLastError());
      return;
    }

    extern class ZUART4 udp;
    udp.push((uint8_t*)buffer, recv_len);
  }
}

void UDP_SERVER::quit(void)
{
  closesocket(server_socket);
  WSACleanup();
}
