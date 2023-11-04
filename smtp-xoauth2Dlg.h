
// smtp-xoauth2Dlg.h : header file
//

#pragma once
#include "TcpClient.h"

// Csmtpxoauth2Dlg dialog
class Csmtpxoauth2Dlg : public CDialogEx
{
// Construction
public:
	Csmtpxoauth2Dlg(CWnd* pParent = nullptr);	// standard constructor
	~Csmtpxoauth2Dlg() { 
		_client.Close();
	}
	void SetTcpClient(TcpClient& client) { _client = client;	}
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SMTPXOAUTH2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;


    CWinThread *_pThread;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
      public:
        CEdit _editInputArea;
        afx_msg void OnBnClickedOk();
        CEdit _editResponseArea;
        TcpClient _client;
};
UINT MyThreadFunction(LPVOID pParam);