#include "pch.h"

#include "Helpers.h"
#include <atlstr.h>
#include <string>

CStringA Helpers::Utf8ToAnsi(const std::string &utf8Str) {
    // Convert UTF-8 to ANSI
    int size = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, nullptr, 0);
    std::wstring wideStr(size, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, &wideStr[0], size);

    // Convert wide character string to ANSI
    int ansiSize = WideCharToMultiByte(CP_ACP, 0, wideStr.c_str(), -1, nullptr,
                                       0, nullptr, nullptr);
    std::string ansiStr(ansiSize, '\0');
    WideCharToMultiByte(CP_ACP, 0, wideStr.c_str(), -1, &ansiStr[0], ansiSize,
                        nullptr, nullptr);

    return CStringA(ansiStr.c_str());
}

CString Helpers::Utf8ToCString(const std::string &utf8Str) {
    // Convert UTF-8 to CString
    CStringW cstrW;
    cstrW.SetString(CString(Utf8ToAnsi(utf8Str)));
    return cstrW;
}

std::string Helpers::AnsiToUtf8(const std::string &ansiStr) {
    // Convert ANSI to wide character string
    int size = MultiByteToWideChar(CP_ACP, 0, ansiStr.c_str(), -1, nullptr, 0);
    std::wstring wideStr(size, L'\0');
    MultiByteToWideChar(CP_ACP, 0, ansiStr.c_str(), -1, &wideStr[0], size);

    // Convert wide character string to UTF-8
    int utf8Size = WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, nullptr,
                                       0, nullptr, nullptr);
    std::string utf8Str(utf8Size, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, &utf8Str[0], utf8Size,
                        nullptr, nullptr);

    return utf8Str;
}

std::string Helpers::CStringToUtf8(const CString &cstrW) {
    // Use CA2W to convert CStringA to wide character string
    const wchar_t *wcharStr = cstrW.GetString();

    // Convert wide character string to UTF-8
    int utf8Size = WideCharToMultiByte(CP_UTF8, 0, wcharStr, -1, nullptr, 0,
                                       nullptr, nullptr);
    std::string utf8Str(utf8Size, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wcharStr, -1, &utf8Str[0], utf8Size,
                        nullptr, nullptr);

    utf8Str.erase(std::remove(utf8Str.begin(), utf8Str.end(), '\0'),
                  utf8Str.end());
    return utf8Str;
}

std::string Helpers::Utf8ToAnsiString(const std::string &utf8Str) {
    // Convert UTF-8 to wide character string
    int size = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, nullptr, 0);
    std::wstring wideStr(size, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, &wideStr[0], size);

    // Convert wide character string to ANSI
    int ansiSize = WideCharToMultiByte(CP_ACP, 0, wideStr.c_str(), -1, nullptr,
                                       0, nullptr, nullptr);
    std::string ansiStr(ansiSize, '\0');
    WideCharToMultiByte(CP_ACP, 0, wideStr.c_str(), -1, &ansiStr[0], ansiSize,
                        nullptr, nullptr);

    return ansiStr;
}
