// CAddressBookDlg.cpp : implementation file
//
#include "pch.h"

#include "CAddressBookDlg.h"
#include "afxdialogex.h"
#include "smtp-xoauth2.h"

// CAddressBookDlg dialog

CAddressBookDlg::CAddressBookDlg(CWnd *pParent /*=nullptr*/)
    : CDialogEx(IDD_DIALOG_ADDRESS_BOOK, pParent) {}

CAddressBookDlg::~CAddressBookDlg() {}

void CAddressBookDlg::DoDataExchange(CDataExchange *pDX) {
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_CONTACTS, _ctrlListContacts);
}

BEGIN_MESSAGE_MAP(CAddressBookDlg, CDialogEx)
END_MESSAGE_MAP()

BOOL CAddressBookDlg::OnInitDialog() {
    CDialog::OnInitDialog();

    DWORD dwStyle = WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_EX_GRIDLINES;
    _ctrlListContacts.SetExtendedStyle(dwStyle);

    _ctrlListContacts.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 200);
    _ctrlListContacts.InsertColumn(1, _T("Contact Numer"), LVCFMT_LEFT, 100);
    _ctrlListContacts.InsertColumn(2, _T("Email Address"), LVCFMT_LEFT, 200);
    return TRUE;
}
