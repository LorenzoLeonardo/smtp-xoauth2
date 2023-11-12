#include "pch.h"

#include "Emailer.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::string RequestProfile::toJson(Profile request) {
    json flowJson = {{"object", "applications.email"},
                     {"method", "getProfile"},
                     {"param",
                      {{"access_token", request.access_token},
                       {"profile_endpoint", request.profile_endpoint}}}};

    return flowJson.dump();
}

std::string Emailer::toJson(EmailInfo info) {

    json flowJson = {
        {"object", "applications.email"},
        {"method", "sendMail"},
        {"param",
         {{"access_token", info.access_token},
          {"recipients",
           {{{"name", info.recipients[0].name},
             {"email", info.recipients[0].email}}}},
          {"sender",
           {{"name", info.sender.name}, {"email", info.sender.email}}},
          {"smtp_port", info.smtp_port},
          {"smtp_server", info.smtp_server},
          {"subject", info.subject},
          {"text_body", info.text_body}}}};

    return flowJson.dump();
}
