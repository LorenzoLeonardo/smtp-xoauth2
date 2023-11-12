#pragma once

#include <string>

namespace Helpers {
CStringA Utf8ToAnsi(const std::string &utf8Str);
CString Utf8ToCString(const std::string &utf8Str);
std::string CStringToUtf8(const CString &cstrA);
std::string AnsiToUtf8(const std::string &ansiStr);
std::string Utf8ToAnsiString(const std::string &utf8Str);
}; // namespace Helpers
