#pragma once
#include "TcpClient.h"
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

typedef void (*EventCallback)(json param);

class RemoteCaller {

  public:
    RemoteCaller();
    ~RemoteCaller();

    bool connect();
    json remoteCall(std::string &object, std::string &method, json &param);

  private:
    TcpClient _client;
};

class RemoteListener {

  public:
    RemoteListener();
    ~RemoteListener();

    bool connect();
    json remoteListen(std::string &object, std::string &method,
                      EventCallback callback);

  private:
    TcpClient _client;
};
