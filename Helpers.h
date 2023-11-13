#pragma once

#include <string>

namespace Helpers {
std::string Utf8ToAnsi(const std::string &utf8Str);
std::string AnsiToUtf8(const std::string &ansiStr);

CStringW Utf8ToCString(const std::string &utf8Str);
std::string CStringToUtf8(const CStringW &cstr);

}; // namespace Helpers
