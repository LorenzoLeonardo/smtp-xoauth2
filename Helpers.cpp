#include "pch.h"

#include "Helpers.h"

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
