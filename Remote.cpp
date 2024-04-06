#include "pch.h"

#include "Error.h"
#include "Helpers.h"
#include "Remote.h"

RemoteCaller::RemoteCaller() { _client = TcpClient("127.0.0.1", 1986); }
RemoteCaller::~RemoteCaller() { _client.Close(); }

bool RemoteCaller::connect() { return _client.Connect(); }
json RemoteCaller::remoteCall(std::string &object, std::string &method,
                              json &param) {

    json msg = {{"object", object}, {"method", method}, {"param", param}};

    json stream = {
        {"id", 0}, {"kind", 2}, {"msg", Helpers::string_to_vec(msg.dump())}};

    std::string bytes = stream.dump();
    size_t byte_sent = _client.Send(bytes.c_str(), (int)bytes.length());

    std::string jsonStr;

    size_t bytes_read = _client.ReceiveString(jsonStr);

    json raw = nlohmann::json::parse(jsonStr);

    std::vector<unsigned char> response =
        raw.at("msg").get<std::vector<unsigned char>>();
    std::string str(response.begin(), response.end());

    json result = nlohmann::json::parse(str);
    return result;
}

RemoteListener::RemoteListener() { _client = TcpClient("127.0.0.1", 1986); }
RemoteListener::~RemoteListener() { _client.Close(); }

bool RemoteListener::connect() { return _client.Connect(); }
json RemoteListener::remoteListen(std::string &object, std::string &method,
                                  EventCallback callback) {

    json result;
    json stream = {{"id", 0},
                   {"kind", 6},
                   {"msg", Helpers::string_to_vec(std::string("oauth2"))}};

    std::string bytes = stream.dump();
    size_t byte_sent = _client.Send(bytes.c_str(), (int)bytes.length());

    std::string jsonStr;

    while (true) {
        size_t bytes_read = _client.ReceiveString(jsonStr);

        json raw = nlohmann::json::parse(jsonStr);

        std::vector<unsigned char> msg =
            raw.at("msg").get<std::vector<unsigned char>>();
        std::string str(msg.begin(), msg.end());

        json result = nlohmann::json::parse(str);
        callback(result);
    }
    return result;
}
