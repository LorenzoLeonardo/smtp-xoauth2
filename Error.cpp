#include "pch.h"

#include "Error.h"

EmailerError::Error EmailerError::From(const json &input) {
    if (input.find("MailSend") != input.end()) {
        return EmailerError::MailSend;
    } else if (input.find("Curl") != input.end()) {
        return EmailerError::Curl;
    } else if (input.find("Http") != input.end()) {
        return EmailerError::Http;
    } else if (input.find("Serde") != input.end()) {
        return EmailerError::Serde;
    } else if (input.find("RemoteCall") != input.end()) {
        return EmailerError::RemoteCall;
    } else {
        return EmailerError::Unknown;
    }
}
