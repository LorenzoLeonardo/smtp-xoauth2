#include "pch.h"

#include "OAuth2DeviceCodeFlow.h"

DeviceCodeFlow
OAuth2DeviceCodeFlow::generateDeviceCodeFlow(std::string method) {
    DeviceCodeFlow flow;

    flow.object = "oauth2.device.code.flow";
    flow.method = method;
    flow.param.authorization_endpoint =
        "https://login.microsoftonline.com/common/oauth2/v2.0/authorize";
    flow.param.device_auth_endpoint =
        "https://login.microsoftonline.com/common/oauth2/v2.0/devicecode";
    flow.param.token_endpoint =
        "https://login.microsoftonline.com/common/oauth2/v2.0/token";
    flow.param.client_id = "64c5d510-4b7e-4a18-8869-89778461c266";
    flow.param.process = "smtp-xoauth2";
    flow.param.provider = "Microsoft";
    flow.param.scopes.push_back("offline_access");
    flow.param.scopes.push_back("https://outlook.office.com/SMTP.Send");
    flow.param.scopes.push_back("https://outlook.office.com/User.Read");

    return flow;
}

std::string OAuth2DeviceCodeFlow::toJson(DeviceCodeFlow login) {
    json flowJson = {
        {"object", login.object},
        {"method", login.method},
        {"param",
         {{"process", login.param.process},
          {"provider", login.param.provider},
          {"scopes", login.param.scopes},
          {"authorization_endpoint", login.param.authorization_endpoint},
          {"token_endpoint", login.param.token_endpoint},
          {"device_auth_endpoint", login.param.device_auth_endpoint},
          {"client_id", login.param.client_id}}}};

    return flowJson.dump();
}

std::string OAuth2DeviceCodeFlow::login() {
    return toJson(generateDeviceCodeFlow("login"));
}
std::string OAuth2DeviceCodeFlow::requestToken() {
    return toJson(generateDeviceCodeFlow("requestToken"));
}
std::string OAuth2DeviceCodeFlow::logout() {
    return toJson(generateDeviceCodeFlow("logout"));
}
std::string OAuth2DeviceCodeFlow::subscribeToEvent() {
    json listen_event = {{"event_name", "oauth2"}};
    return listen_event.dump();
}
std::string OAuth2DeviceCodeFlow::cancel() {
    return toJson(generateDeviceCodeFlow("cancel"));
}
