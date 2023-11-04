#pragma once

#include <iostream>
#include <winsock2.h>

class TcpClient {
public:
  TcpClient() {
    _serverIp = NULL;
    _clientSocket = INVALID_SOCKET;
  }
  TcpClient(const char *serverIp, int serverPort);
  ~TcpClient();

  bool Connect();

  int Send(const char *data, int length);

  int Receive(char *buffer, int length);

  void Close();

private:
  const char *_serverIp;
  int _serverPort;
  WSADATA _wsaData;
  SOCKET _clientSocket;
};