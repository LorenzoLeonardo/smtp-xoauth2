#pragma once
#include "afxdialogex.h"

// CAddressBookDlg dialog

class CAddressBookDlg : public CDialogEx {
  public:
    CAddressBookDlg(CWnd *pParent = nullptr); // standard constructor
    ~CAddressBookDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_ADDRESS_BOOK };
#endif

  protected:
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support

    DECLARE_MESSAGE_MAP()
    CListCtrl _ctrlListContacts;
};
