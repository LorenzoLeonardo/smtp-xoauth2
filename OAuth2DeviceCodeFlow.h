#pragma once
#include <nlohmann/json.hpp>
#include <optional>

using json = nlohmann::json;

struct DeviceCodeFlow {
    std::string object;
    std::string method;
    struct Param {
        std::string process;
        std::string provider;
        std::vector<std::string> scopes;
        std::string authorization_endpoint;
        std::string token_endpoint;
        std::string device_auth_endpoint;
        std::string client_id;
        std::optional<std::string> client_secret;
    } param;
};
namespace OAuth2DeviceCodeFlow {
DeviceCodeFlow generateDeviceCodeFlow(std::string method);
std::string login();
std::string requestToken();
std::string logout();
std::string subscribeToEvent();
std::string cancel();
std::string toJson(DeviceCodeFlow login);
}; // namespace OAuth2DeviceCodeFlow
