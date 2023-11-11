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
