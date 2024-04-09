#pragma once
#include <string>
#include <vector>

class Config {
  public:
    static class Config &getInstance() {
        static Config instance;
        return instance;
    }

    Config() { loadConfigFromFile("config.json"); }

    struct Provider {
        std::string name;
        std::string profile_endpoint;
        std::string auth_endpoint;
        std::string device_auth_endpoint;
        std::string token_endpoint;
        std::string contacts_endpoint;
        std::string client_id;
        std::vector<std::string> scopes;
    };
    std::string server;
    unsigned short port;
    Provider provider;

  private:
    void loadConfigFromFile(const char *path);
};
