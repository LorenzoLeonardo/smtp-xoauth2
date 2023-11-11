#pragma once

#include <string>

namespace Helpers {
	CStringA Utf8ToAnsi(const std::string &utf8Str);
	CString Utf8ToCString(const std::string &utf8Str);
};
