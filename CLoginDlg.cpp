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
}

BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
END_MESSAGE_MAP()

void CLoginDlg::SetUrl(std::string url) {

    _ctrlStaticLink.SetWindowText(Helpers::Utf8ToCString(url).GetBuffer());
}
void CLoginDlg::SetUserCode(std::string usercode) {

    _ctrlEditUserCode.SetWindowText(
        Helpers::Utf8ToCString(usercode).GetBuffer());
}
void CLoginDlg::SetExpiryTime(int expires) {}
