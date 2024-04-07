#pragma once
#include "Remote.h"
#include "afxdialogex.h"
#include <string>
#include <vector>
// CAddressBookDlg dialog

class CAddressBookDlg : public CDialogEx {
  public:
    CAddressBookDlg(CWnd *pParent = nullptr);
    ~CAddressBookDlg();

    void setToken(std::string token) { _access_token = token; }
// Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_ADDRESS_BOOK };
#endif

  protected:
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support
    void PopulateList();

    DECLARE_MESSAGE_MAP()
    CListCtrl _ctrlListContacts;
    RemoteCaller _remote;
    std::string _access_token;
    CButton _ctrlBtnPrev;
    CButton _ctrlBtnNext;
    std::string _nextLink;
    std::vector<std::string> _pageStack;

  public:
    afx_msg void OnBnClickedButtonPrev();
    afx_msg void OnBnClickedButtonNext();
};
