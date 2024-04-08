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
    : CDialogEx(IDD_DIALOG_ADDRESS_BOOK, pParent) {
    _nextLink = "https://outlook.office.com/api/v2.0/me/contacts";
}

CAddressBookDlg::~CAddressBookDlg() {}

void CAddressBookDlg::DoDataExchange(CDataExchange *pDX) {
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_CONTACTS, _ctrlListContacts);
    DDX_Control(pDX, IDC_BUTTON_PREV, _ctrlBtnPrev);
    DDX_Control(pDX, IDC_BUTTON_NEXT, _ctrlBtnNext);
}

BEGIN_MESSAGE_MAP(CAddressBookDlg, CDialogEx)
ON_BN_CLICKED(IDC_BUTTON_PREV, &CAddressBookDlg::OnBnClickedButtonPrev)
ON_BN_CLICKED(IDC_BUTTON_NEXT, &CAddressBookDlg::OnBnClickedButtonNext)
ON_NOTIFY(NM_DBLCLK, IDC_LIST_CONTACTS,
          &CAddressBookDlg::OnNMDblclkListContacts)
ON_WM_SYSCOMMAND()
ON_BN_CLICKED(IDOK, &CAddressBookDlg::OnBnClickedOk)
ON_BN_CLICKED(IDCANCEL, &CAddressBookDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL CAddressBookDlg::OnInitDialog() {
    CDialog::OnInitDialog();

    DWORD dwStyle = WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_EX_GRIDLINES |
                    LVS_EX_FULLROWSELECT;
    _ctrlListContacts.SetExtendedStyle(dwStyle);

    _ctrlListContacts.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 200);
    _ctrlListContacts.InsertColumn(1, _T("Contact Numer"), LVCFMT_LEFT, 100);
    _ctrlListContacts.InsertColumn(2, _T("Email Address"), LVCFMT_LEFT, 200);

    _isBusy.store(false);
    try {
        _remote.connect();
        _pThread = AfxBeginThread(RetrieveContactThread, this,
                                  THREAD_PRIORITY_NORMAL, 0, 0, nullptr);
    } catch (const SmtpError error) {
        AfxMessageBox(
            static_cast<LPCTSTR>(Helpers::Utf8ToCString(error.what())),
            MB_ICONERROR | MB_OK);
    }

    return TRUE;
}

UINT RetrieveContactThread(LPVOID pParam) {
    CAddressBookDlg *dlg = static_cast<CAddressBookDlg *>(pParam);
    dlg->_isBusy.store(true);
    dlg->PopulateList();
    dlg->_isBusy.store(false);
    return 0;
}

void CAddressBookDlg::PopulateList() {
    _ctrlListContacts.DeleteAllItems();

    Profile profile;
    profile.access_token = _access_token;
    profile.profile_endpoint = _nextLink;

    std::string request = RequestContacts::toJson(profile);
    std::string object = "applications.email";
    std::string method = "getContacts";
    json param = {{"access_token", profile.access_token},
                  {"profile_endpoint", profile.profile_endpoint}};

    BOOL bPrev = _ctrlBtnPrev.IsWindowEnabled();
    _ctrlBtnNext.EnableWindow(FALSE);
    _ctrlBtnPrev.EnableWindow(FALSE);
    json response = _remote.remoteCall(object, method, param);
    int nRow = 0;

    if (response.at("@odata.nextLink").empty()) {
        _ctrlBtnNext.EnableWindow(FALSE);
        _pageStack.push_back(_nextLink);
    } else {
        _ctrlBtnNext.EnableWindow(TRUE);
        _pageStack.push_back(_nextLink);
        _nextLink = response.at("@odata.nextLink").get<std::string>();
    }
    _ctrlBtnPrev.EnableWindow(bPrev);
    for (const auto &item : response.at("value")) {
        std::string name = item["DisplayName"];
        _ctrlListContacts.InsertItem(
            LVIF_TEXT | LVIF_STATE, nRow,
            static_cast<LPCTSTR>(Helpers::Utf8ToCString(name)), 0, 0, 0, 0);

        std::string mobile = item["MobilePhone1"];
        _ctrlListContacts.SetItemText(
            nRow, 1, static_cast<LPCTSTR>(Helpers::Utf8ToCString(mobile)));

        std::vector<json> email = item["EmailAddresses"];
        if (!email.empty()) {
            _ctrlListContacts.SetItemText(
                nRow, 2,
                static_cast<LPCTSTR>(Helpers::Utf8ToCString(
                    email[0].at("Address").get<std::string>())));
        }
        nRow++;
    }
}
void CAddressBookDlg::OnBnClickedButtonPrev() {
    // TODO: Add your control notification handler code here

    if (!_pageStack.empty()) {
        _pageStack.pop_back();
        _nextLink = _pageStack.back();
        _pageStack.pop_back();
        if (_pageStack.empty()) {
            _ctrlBtnPrev.EnableWindow(FALSE);
        }
    } else {
        _ctrlBtnPrev.EnableWindow(FALSE);
    }
    _pThread = AfxBeginThread(RetrieveContactThread, this,
                              THREAD_PRIORITY_NORMAL, 0, 0, nullptr);
}

void CAddressBookDlg::OnBnClickedButtonNext() {
    // TODO: Add your control notification handler code here
    _ctrlBtnPrev.EnableWindow(TRUE);
    _pThread = AfxBeginThread(RetrieveContactThread, this,
                              THREAD_PRIORITY_NORMAL, 0, 0, nullptr);
}

void CAddressBookDlg::OnNMDblclkListContacts(NMHDR *pNMHDR, LRESULT *pResult) {
    LPNMITEMACTIVATE pNMItemActivate =
        reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: Add your control notification handler code here
    *pResult = 0;
    LVITEM lvItem = {};
    if (pNMItemActivate->iItem >= 0) {
        lvItem.pszText = new TCHAR[MAX_PATH]; // Buffer to receive the item text
        lvItem.cchTextMax = MAX_PATH;

        lvItem.mask = LVIF_TEXT |
                      LVIF_PARAM; // Specify the mask to retrieve text and data
        lvItem.iItem = pNMItemActivate->iItem; // Index of the item
        lvItem.iSubItem =
            2; // Subitem index (if your list control has subitems)

        _ctrlListContacts.GetItem(&lvItem);
        _chosenEmail = lvItem.pszText;
        delete[] lvItem.pszText;
        CAddressBookDlg::OnOK();
    }
    // pNMItemActivate->iItem
}

void CAddressBookDlg::OnSysCommand(UINT nID, LPARAM lParam) {

    if ((nID & 0xFFF0) == SC_CLOSE) {
        // Optionally, display a message to inform the user
        if (_isBusy.load()) {

            AfxMessageBox(_T("Address book is still busy."),
                          MB_OK | MB_ICONINFORMATION);
            // Return without calling the base class implementation,
            // effectively preventing the dialog from closing.
            return;
        }
    }
    CDialogEx::OnSysCommand(nID, lParam);
}

void CAddressBookDlg::OnBnClickedOk() {
    // TODO: Add your control notification handler code here
    // Optionally, display a message to inform the user
    if (_isBusy.load()) {

        AfxMessageBox(_T("Address book is still busy."),
                      MB_OK | MB_ICONINFORMATION);
        // Return without calling the base class implementation,
        // effectively preventing the dialog from closing.
        return;
    }
    CDialogEx::OnOK();
}

void CAddressBookDlg::OnBnClickedCancel() {
    // TODO: Add your control notification handler code here
    // Optionally, display a message to inform the user
    if (_isBusy.load()) {

        AfxMessageBox(_T("Address book is still busy, please close later"),
                      MB_OK | MB_ICONINFORMATION);
        // Return without calling the base class implementation,
        // effectively preventing the dialog from closing.
        return;
    }
    CDialogEx::OnCancel();
}
