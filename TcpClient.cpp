#include "pch.h"

#include "Error.h"
#include "TcpClient.h"
#include <format>

constexpr auto MAX_CHUNK = 4096;

TcpClient::TcpClient(const char *serverIp, int serverPort)
    : _serverIp(serverIp), _serverPort(serverPort) {

    _clientSocket = INVALID_SOCKET;
    if (WSAStartup(MAKEWORD(2, 2), &_wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return;
    }
}

TcpClient::~TcpClient() {
    this->Close();
    WSACleanup();
}

void TcpClient::Close() {
    if (_clientSocket != INVALID_SOCKET) {
        closesocket(_clientSocket);
        _clientSocket = INVALID_SOCKET;
    }
}

bool TcpClient::Connect() {
    _clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_clientSocket == INVALID_SOCKET) {
        std::string err =
            std::format("Error creating socket: {}", WSAGetLastError());
        throw SmtpError(err);
    }

    sockaddr_in serverAddress;

    ZeroMemory(&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(_serverPort);

    if (inet_pton(AF_INET, _serverIp, &(serverAddress.sin_addr)) != 1) {
        std::string err = std::format("Invalid IP address: {}", _serverIp);

        throw SmtpError(err);
    }

    if (connect(_clientSocket, reinterpret_cast<SOCKADDR *>(&serverAddress),
                sizeof(serverAddress)) == SOCKET_ERROR) {
        std::string err = std::format("Error connecting to the server: {}",
                                      WSAGetLastError());

        throw SmtpError(err);
    }
    return true;
}

int TcpClient::Send(const char *data, int length) {
    if (UINT16_MAX < length) {
        std::string err = std::format("Error max data length overflow");

        throw SmtpError(err);
    }
    int bytesSent = send(_clientSocket, data, length, 0);
    if (bytesSent == SOCKET_ERROR) {
        std::string err =
            std::format("Error sending data: {}", WSAGetLastError());

        throw SmtpError(err);
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
        std::string err =
            std::format("Error receiving data: {}", WSAGetLastError());

        throw SmtpError(err);
    }
    return bytesRead;
}

size_t TcpClient::ReceiveString(std::string &buffer) {

    char chunk[MAX_CHUNK] = {};
    int bytesRead = 0;
    do {

        bytesRead = recv(_clientSocket, chunk, MAX_CHUNK, 0);
        if (bytesRead == SOCKET_ERROR) {
            std::string err =
                std::format("Error receiving data: {}", WSAGetLastError());

            throw SmtpError(err);
        } else if (bytesRead == 0) {
            std::string err = std::format("connection to server was closed: {}",
                                          WSAGetLastError());

            throw SmtpError(err);
        }
        buffer.append(chunk, bytesRead);
    } while (!(bytesRead < MAX_CHUNK));

    return buffer.length();
}

int TcpClient::Shutdown() { return closesocket(_clientSocket); }
