#include "pch.h"

#include "Config.h"
#include "Error.h"
#include "Helpers.h"

bool Config::loadConfigFromFile(const char *path) {
    try {

        json config = Helpers::loadJsonFromFile(path);

        _config.server = config["Server"];
        _config.port = config["Port"];

        for (auto &elem : config["Providers"]) {
            struct Provider provider = {};

            provider.auth_endpoint = elem["AuthorizationEndpoint"];
            provider.client_id = elem["ClientId"];
            provider.contacts_endpoint = elem["ContactsEndpoint"];
            provider.device_auth_endpoint = elem["DeviceAuthEndpoint"];
            provider.name = elem["Name"];
            provider.profile_endpoint = elem["ProfileEndpoint"];
            for (auto &scopes : elem["Scopes"]) {
                provider.scopes.push_back(scopes.get<std::string>());
            }
            provider.token_endpoint = elem["TokenEndpoint"];
            _config.providers.push_back(provider);
        }
        return true;
    } catch (const SmtpError e) {
        return false;
    }
}
