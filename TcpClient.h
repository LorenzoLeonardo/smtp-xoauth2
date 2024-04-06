#pragma once

#include <iostream>
#include <winsock2.h>

class TcpClient {
  public:
    TcpClient();
    TcpClient(const char *serverIp, int serverPort);
    ~TcpClient();

    bool Connect();
    int Send(const char *data, int length);
    size_t Receive(std::string &buffer);
    void Close();

  private:
    const char *_serverIp;
    int _serverPort;
    WSADATA _wsaData;
    SOCKET _clientSocket;
};
