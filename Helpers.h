#pragma once

#include <afxver_.h> // For VS_FIXEDFILEINFO definition
#include <afxwin.h>
#include <algorithm>
#include <atlstr.h>
#include <fstream>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
#include <vector>

using json = nlohmann::json;

namespace Helpers {
std::string Utf8ToAnsi(const std::string &utf8Str);
std::string AnsiToUtf8(const std::string &ansiStr);

CStringW Utf8ToCString(const std::string &utf8Str);
std::string CStringToUtf8(const CStringW &cstr);
std::vector<unsigned char> string_to_vec(std::string str);
std::vector<std::string> separate(const std::string &data);
std::vector<std::string> removeDuplicates(const std::vector<std::string> &vec);
CString GetLegalCopyright();
CString GetProductVersion();
json loadJsonFromFile(const char *path);
}; // namespace Helpers
