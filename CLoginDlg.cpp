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
