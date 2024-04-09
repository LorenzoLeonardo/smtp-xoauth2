#include "pch.h"

#include "Error.h"
#include "Helpers.h"

std::string Helpers::Utf8ToAnsi(const std::string &utf8Str) {
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

    return ansiStr.c_str();
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

CStringW Helpers::Utf8ToCString(const std::string &utf8Str) {

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

    // Convert UTF-8 to CString
    CStringW cstrW;
    cstrW.SetString(CStringW(CStringA(ansiStr.c_str())));
    return cstrW;
}

std::string Helpers::CStringToUtf8(const CStringW &cstr) {
    // Use CA2W to convert CStringA to wide character string
    const wchar_t *wcharStr = cstr.GetString();

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

std::vector<unsigned char> Helpers::string_to_vec(std::string str) {
    std::vector<unsigned char> bytes;

    // Convert string to vector of bytes
    for (char c : str) {
        bytes.push_back(static_cast<unsigned char>(c));
    }
    return bytes;
}

std::vector<std::string> Helpers::separate(const std::string &data) {
    std::string json_str = data;

    if (json_str.find("}{") == std::string::npos) {
        return {data};
    }

    std::vector<std::string> parts;
    size_t pos = 0;
    while ((pos = json_str.find("}{", pos)) != std::string::npos) {
        parts.push_back(json_str.substr(0, pos + 1));
        json_str.erase(0, pos + 2);
    }
    parts.push_back("{" + json_str);

    return parts;
}

std::vector<std::string>
Helpers::removeDuplicates(const std::vector<std::string> &vec) {
    std::vector<std::string> result;

    // Sort the vector to bring identical items together
    std::vector<std::string> sortedVec = vec;
    std::sort(sortedVec.begin(), sortedVec.end());

    // Unique algorithm removes consecutive duplicates
    auto it = std::unique(sortedVec.begin(), sortedVec.end());

    // Copy the unique elements to the result vector
    result.assign(sortedVec.begin(), it);

    return result;
}

CString Helpers::GetProductVersion() {
    CString versionString;
    TCHAR szFilePath[MAX_PATH] = {};
    if (GetModuleFileName(NULL, szFilePath, MAX_PATH) != 0) {
        DWORD dwDummy = 0;
        DWORD dwSize = GetFileVersionInfoSize(szFilePath, &dwDummy);
        if (dwSize != 0) {
            BYTE *pVersionInfo = new BYTE[dwSize];
            if (GetFileVersionInfo(szFilePath, 0, dwSize, pVersionInfo) != 0) {
                VS_FIXEDFILEINFO *pFileInfo = nullptr;
                UINT uLen;
                if (VerQueryValue(pVersionInfo, _T("\\"), (LPVOID *)&pFileInfo,
                                  &uLen) != 0) {
                    WORD major = HIWORD(pFileInfo->dwProductVersionMS);
                    WORD minor = LOWORD(pFileInfo->dwProductVersionMS);
                    WORD build = HIWORD(pFileInfo->dwProductVersionLS);
                    WORD revision = LOWORD(pFileInfo->dwProductVersionLS);
                    versionString.Format(_T("%d.%d.%d.%d"), major, minor, build,
                                         revision);
                }
            }
            delete[] pVersionInfo;
        }
    }
    return versionString;
}

CString Helpers::GetLegalCopyright() {
    TCHAR strFilePath[MAX_PATH] = {};
    if (GetModuleFileName(NULL, strFilePath, MAX_PATH) == 0) {
        return _T("");
    }

    // Get the size of the version information
    DWORD dwHandle = 0, dwSize = 0;
    dwSize = GetFileVersionInfoSize(strFilePath, &dwHandle);
    if (dwSize == 0) {
        std::cerr << "Failed to get version information size." << std::endl;
        return CString();
    }

    // Allocate memory for the version information
    std::vector<BYTE> versionInfo(dwSize);
    if (!GetFileVersionInfo(strFilePath, 0, dwSize, versionInfo.data())) {
        std::cerr << "Failed to get version information." << std::endl;
        return CString();
    }

    // Get the language and code page
    DWORD *pTranslation = nullptr;
    UINT cbTranslation = 0;
    if (!VerQueryValue(versionInfo.data(), _T("\\VarFileInfo\\Translation"),
                       reinterpret_cast<LPVOID *>(&pTranslation),
                       &cbTranslation)) {
        std::cerr << "Failed to get translation information." << std::endl;
        return CString();
    }

    // Form the query string for LegalCopyright
    CString strQuery;
    strQuery.Format(_T("\\StringFileInfo\\%04x%04x\\LegalCopyright"),
                    LOWORD(*pTranslation), HIWORD(*pTranslation));

    // Retrieve the LegalCopyright
    LPTSTR lpLegalCopyright = nullptr;
    UINT cbData = 0;
    if (!VerQueryValue(versionInfo.data(), strQuery,
                       reinterpret_cast<LPVOID *>(&lpLegalCopyright),
                       &cbData)) {
        std::cerr << "Failed to get LegalCopyright." << std::endl;
        return CString();
    }

    return CString(lpLegalCopyright);
}

json Helpers::loadJsonFromFile(const char *path) {
    // Parse the JSON content
    json jsonData = {};
    std::ifstream jsonFile(path);
    if (!jsonFile.is_open()) {
        std::cerr << "Failed to open JSON file" << std::endl;
        throw SmtpError("Failed to open JSON file");
    }

    try {
        jsonFile >> jsonData;
    } catch (json::parse_error &e) {
        std::string err = std::format("Parse error: {}", e.what());
        std::cerr << err << std::endl;
        throw SmtpError(err);
    }

    // Close the file
    jsonFile.close();
    return jsonData;
}
