// CLoginDlg.cpp : implementation file
//
#include "pch.h"

#include "CLoginDlg.h"
#include "afxdialogex.h"
#include "smtp-xoauth2.h"

#include <afx.h>
// CLoginDlg dialog

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd *pParent /*=nullptr*/)
    : CDialogEx(IDD_DIALOG_LOGIN, pParent) {}

CLoginDlg::~CLoginDlg() {}

void CLoginDlg::DoDataExchange(CDataExchange *pDX) {
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MFCLINK_LOGIN, _ctrlStaticLink);
    DDX_Control(pDX, IDC_EDIT_USER_CODE, _ctrlEditUserCode);
}

BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
END_MESSAGE_MAP()

CStringA Utf8ToAnsi(const std::string &utf8Str) {
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

CString Utf8ToCString(const std::string &utf8Str) {
    // Convert UTF-8 to CString
    CStringW cstrW;
    cstrW.SetString(CString(Utf8ToAnsi(utf8Str)));
    return cstrW;
}

void CLoginDlg::SetUrl(std::string url) {

    _ctrlStaticLink.SetWindowText(Utf8ToCString(url).GetBuffer());
}
void CLoginDlg::SetUserCode(std::string usercode) {

    _ctrlEditUserCode.SetWindowText(Utf8ToCString(usercode).GetBuffer());
}
void CLoginDlg::SetExpiryTime(int expires) {}
