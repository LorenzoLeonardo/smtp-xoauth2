#include "pch.h"

#include "TcpClient.h"

TcpClient::TcpClient(const char *serverIp, int serverPort)
    : _serverIp(serverIp), _serverPort(serverPort) {

    _clientSocket = INVALID_SOCKET;
    if (WSAStartup(MAKEWORD(2, 2), &_wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return;
    }
}

TcpClient::~TcpClient() { WSACleanup(); }

void TcpClient::Close() { closesocket(_clientSocket); }

bool TcpClient::Connect() {
    _clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_clientSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket: " << WSAGetLastError()
                  << std::endl;
        return false;
    }

    sockaddr_in serverAddress;

    ZeroMemory(&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(_serverPort);

    if (inet_pton(AF_INET, _serverIp, &(serverAddress.sin_addr)) != 1) {
        std::cerr << "Invalid IP address: " << _serverIp << std::endl;
        closesocket(_clientSocket);
        return false;
    }

    if (connect(_clientSocket, reinterpret_cast<SOCKADDR *>(&serverAddress),
                sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Error connecting to the server: " << WSAGetLastError()
                  << std::endl;
        closesocket(_clientSocket);
        return false;
    }

    return true;
}

int TcpClient::Send(const char *data, int length) {
    if (UINT16_MAX < length) {
        std::cerr << "Error max data length overflow" << std::endl;
        return SOCKET_ERROR;
    }
    int bytesSent = send(_clientSocket, data, length, 0);
    if (bytesSent == SOCKET_ERROR) {
        std::cerr << "Error sending data: " << WSAGetLastError() << std::endl;
        return SOCKET_ERROR;
    }
    return bytesSent;
}

int TcpClient::Receive(char *buffer, int length) {

    if (UINT16_MAX < length) {
        std::cerr << "Error max data length overflow" << std::endl;
        return SOCKET_ERROR;
    }
    int bytesRead = recv(_clientSocket, buffer, length, 0);
    if (bytesRead == SOCKET_ERROR) {
        std::cerr << "Error receiving data: " << WSAGetLastError() << std::endl;
        return SOCKET_ERROR;
    }
    return bytesRead;
}
