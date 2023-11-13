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

std::string Emailer::toJson(EmailInfo email) {

    // Convert the email structure to JSON
    json emailJson;
    emailJson["smtp_server"] = email.smtp_server;
    emailJson["smtp_port"] = email.smtp_port;
    emailJson["sender"] = {{"name", email.sender.name},
                           {"email", email.sender.email}};

    // Convert recipients to JSON array
    json recipientsJson;
    for (const auto &recipient : email.recipients) {
        recipientsJson.push_back(
            {{"name", recipient.name}, {"email", recipient.email}});
    }
    emailJson["recipients"] = recipientsJson;
    emailJson["subject"] = email.subject;
    emailJson["text_body"] = email.text_body;
    emailJson["access_token"] = email.access_token;

    json flowJson;
    flowJson["object"] = "applications.email";
    flowJson["method"] = "sendMail";
    flowJson["param"] = emailJson;

    return flowJson.dump();
}
