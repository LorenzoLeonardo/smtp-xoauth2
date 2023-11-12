#pragma once

#include <string>
#include <vector>

struct Profile {
    std::string access_token;
    std::string profile_endpoint;
};

struct EmailInfo {
    struct Sender {
        std::string name;
        std::string email;
    };
    struct Recipient {
        std::string name;
        std::string email;
    };
    std::string smtp_server;
    int smtp_port;
    Sender sender;
    std::vector<Recipient> recipients;
    std::string subject;
    std::string text_body;
    std::string access_token;
};

namespace RequestProfile {
std::string toJson(Profile request);
}

namespace Emailer {
std::string toJson(EmailInfo info);
}
