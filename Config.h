#pragma once
#include <string>
#include <vector>

class Config {
  protected:
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
    struct Field {
        std::string server;
        unsigned short port;
        std::vector<Provider> providers;
    };

    Field _config;

  public:
    bool loadConfigFromFile(const char *path);
};
