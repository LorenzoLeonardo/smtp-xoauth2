#pragma once
#include <nlohmann/json.hpp>
#include <optional>

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
