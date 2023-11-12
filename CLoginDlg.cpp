// CLoginDlg.cpp : implementation file
//
#include "pch.h"

#include "CLoginDlg.h"
#include "Helpers.h"
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
    DDX_Control(pDX, IDC_STATIC_LOGIN_NOTICE, _ctrlStaticNote);
}

BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
ON_EN_SETFOCUS(IDC_EDIT_USER_CODE, &CLoginDlg::OnEnSetfocusEditUserCode)
END_MESSAGE_MAP()

void CLoginDlg::SetUrl(std::string url) {

    _ctrlStaticLink.SetWindowText(
        static_cast<LPCTSTR>(Helpers::Utf8ToCString(url)));
}
void CLoginDlg::SetUserCode(std::string usercode) {

    _ctrlEditUserCode.SetWindowText(
        static_cast<LPCTSTR>(Helpers::Utf8ToCString(usercode)));
}
void CLoginDlg::SetExpiryTime(int expires) {}

void CLoginDlg::SetErrorNotice(std::string error) {
    _ctrlStaticNote.SetWindowText(
        static_cast<LPCTSTR>(Helpers::Utf8ToCString(error)));
}
BOOL CLoginDlg::OnInitDialog() {
    CDialogEx::OnInitDialog();

    if (GetParent() != NULL) {
        GetParent()->EnableWindow(FALSE);
    }

    return TRUE;
}

void CLoginDlg::OnCancel() {
    // TODO: Add your specialized code here and/or call the base class
    if (GetParent() != NULL) {
        GetParent()->EnableWindow(TRUE);

        GetParent()->PostMessage(WM_CLOSE);
    }
    CDialogEx::OnCancel();
}

void CLoginDlg::OnEnSetfocusEditUserCode() {
    CString selectedText;

    // Get the text from the edit control
    _ctrlEditUserCode.GetWindowText(selectedText);

    if (!selectedText.IsEmpty()) {
        // Set the selection to the entire text
        _ctrlEditUserCode.SetSel(0, -1);
        // Copy the selected text to the clipboard
        if (::OpenClipboard(m_hWnd)) {
            ::EmptyClipboard();
            HGLOBAL hClipboardData = ::GlobalAlloc(
                GMEM_DDESHARE, (selectedText.GetLength() + 1) * sizeof(TCHAR));
            if (hClipboardData != nullptr) {
                LPTSTR pchData =
                    static_cast<LPTSTR>(::GlobalLock(hClipboardData));
                if (pchData != nullptr) {
                    _tcscpy_s(pchData, selectedText.GetLength() + 1,
                              selectedText);
                }
                ::GlobalUnlock(hClipboardData);
                ::SetClipboardData(CF_UNICODETEXT, hClipboardData);
            }
            ::CloseClipboard();
        }
    }
}
