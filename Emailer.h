#pragma once

#include <string>

struct Profile {
    std::string access_token;
    std::string profile_endpoint;
};

namespace RequestProfile {
std::string toJson(Profile request);
}
