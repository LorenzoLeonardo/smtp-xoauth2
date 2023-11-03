#pragma once

#include <iostream>
#include <winsock2.h>

class TcpClient {
public:
  TcpClient(const char *serverIp, int serverPort);
  ~TcpClient();

  bool Connect();

  bool Send(const char *data, int length);

  bool Receive(char *buffer, int length);

  void Close();

private:
  const char *_serverIp;
  int _serverPort;
  WSADATA wsaData_;
  SOCKET _clientSocket;
};