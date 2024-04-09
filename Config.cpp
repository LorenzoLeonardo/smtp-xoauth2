#include "pch.h"

#include "Config.h"
#include "Error.h"
#include "Helpers.h"

void Config::loadConfigFromFile(const char *path) {

    json config = Helpers::loadJsonFromFile(path);

    server = config["Server"];
    port = config["Port"];
    json elem = config["Providers"];

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

    return;
}
