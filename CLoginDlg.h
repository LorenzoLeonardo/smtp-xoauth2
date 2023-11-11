#pragma once
#include "afxdialogex.h"

#include <string>
// CLoginDlg dialog

class CLoginDlg : public CDialogEx {
    DECLARE_DYNAMIC(CLoginDlg)

  public:
    CLoginDlg(CWnd *pParent = nullptr); // standard constructor
    virtual ~CLoginDlg();

    void SetUrl(std::string url);
    void SetUserCode(std::string usercode);
    void SetExpiryTime(int expires);
    void SetErrorNotice(std::string error);

// Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_LOGIN };
#endif

  protected:
    virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support

    DECLARE_MESSAGE_MAP()
  public:
    CMFCLinkCtrl _ctrlStaticLink;
    CEdit _ctrlEditUserCode;
    virtual BOOL OnInitDialog();
    virtual void OnCancel();
    CStatic _ctrlStaticNote;
};
