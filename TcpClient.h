#pragma once

#include <iostream>
#include <winsock2.h>

class TcpClient {
  public:
    TcpClient() {
        _serverIp = nullptr;
        _serverPort = 0;
        _clientSocket = INVALID_SOCKET;
        _wsaData = {};
    }
    TcpClient(const char *serverIp, int serverPort);
    ~TcpClient();

    bool Connect();

    int Send(const char *data, int length);

    int Receive(char *buffer, int length);

    size_t ReceiveString(std::string &buffer);

    int Shutdown();

    void Close();

  private:
    const char *_serverIp;
    int _serverPort;
    WSADATA _wsaData;
    SOCKET _clientSocket;
};
