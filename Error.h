#pragma once
#include <exception>
#include <iostream>
#include <string>

class SmtpError : public std::exception {
  private:
    std::string message;

  public:
    SmtpError(const std::string msg) : message(msg) {
        // std::cerr << message << std::endl;
    }

    const char *what() const noexcept override { return message.c_str(); }
};
