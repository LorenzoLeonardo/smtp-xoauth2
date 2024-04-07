// CAddressBookDlg.cpp : implementation file
//
#include "pch.h"

#include "CAddressBookDlg.h"
#include "Emailer.h"
#include "Error.h"
#include "Helpers.h"
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

    DWORD dwStyle = WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_EX_GRIDLINES |
                    LVS_EX_FULLROWSELECT;
    _ctrlListContacts.SetExtendedStyle(dwStyle);

    _ctrlListContacts.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 200);
    _ctrlListContacts.InsertColumn(1, _T("Contact Numer"), LVCFMT_LEFT, 100);
    _ctrlListContacts.InsertColumn(2, _T("Email Address"), LVCFMT_LEFT, 200);

    try {
        _remote.connect();

        Profile profile;
        profile.access_token = _access_token;
        profile.profile_endpoint =
            "https://outlook.office.com/api/v2.0/me/contacts";

        std::string request = RequestContacts::toJson(profile);
        std::string object = "applications.email";
        std::string method = "getContacts";
        json param = {{"access_token", profile.access_token},
                      {"profile_endpoint", profile.profile_endpoint}};

        json response = _remote.remoteCall(object, method, param);
        int nRow = 0;
        for (const auto &item : response.at("value")) {
            std::string name = item["DisplayName"];
            _ctrlListContacts.InsertItem(
                LVIF_TEXT | LVIF_STATE, nRow,
                static_cast<LPCTSTR>(Helpers::Utf8ToCString(name)), 0, 0, 0, 0);

            std::string mobile = item["MobilePhone1"];
            _ctrlListContacts.SetItemText(
                nRow, 1, static_cast<LPCTSTR>(Helpers::Utf8ToCString(mobile)));

            std::vector<std::string> email = item["EmailAddressess"];
            if (!email.empty()) {
                _ctrlListContacts.SetItemText(
                    nRow, 2,
                    static_cast<LPCTSTR>(Helpers::Utf8ToCString(email[0])));
            }
            nRow++;
        }
    } catch (const SmtpError error) {
        AfxMessageBox(
            static_cast<LPCTSTR>(Helpers::Utf8ToCString(error.what())),
            MB_ICONERROR | MB_OK);
    }

    return TRUE;
}
