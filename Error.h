#pragma once
#include <exception>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

class SmtpError : public std::exception {
  private:
    std::string message;

  public:
    SmtpError(const std::string msg) : message(msg) {
        std::cerr << message << std::endl;
    }

    const char *what() const noexcept override { return message.c_str(); }
};

namespace EmailerError {
enum Error {
    Curl = 0,
    Http,
    MailSend,
    Serde,
    RemoteCall,
    Unknown,
};

Error From(const json &input);
} // namespace EmailerError
