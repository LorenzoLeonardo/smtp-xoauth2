
// smtp-xoauth2Dlg.h : header file
//

#pragma once
#include "CLoginDlg.h"
#include "OAuth2DeviceCodeFlow.h"
#include "TcpClient.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum class JsonType {
    LoginReply,
    Error,
    TokenResponse,
    TokenResponseError,
    Unknown
};

struct LoginReply {
    int interval;
    std::string device_code;
    std::string user_code;
    std::string verification_uri;
    int expires_in;
};

struct Error {
    std::string error;
};

struct TokenResponse {
    struct ExpiresIn {
        int secs;
        int nanos;
    };

    std::string access_token;
    std::vector<std::string> scopes;
    struct TokenReceiveTime {
        int secs;
        int nanos;
    };
    std::string refresh_token;
    ExpiresIn expires_in;
    TokenReceiveTime token_receive_time;
};

struct TokenResponseError {
    std::string error_code_desc;
    std::string error_code;
};

// Csmtpxoauth2Dlg dialog
class Csmtpxoauth2Dlg : public CDialogEx {
    // Construction
  public:
    Csmtpxoauth2Dlg(CWnd *pParent = nullptr); // standard constructor
    ~Csmtpxoauth2Dlg() { _client.Close(); }
    void SetTcpClient(TcpClient &client) { _client = client; }
// Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_SMTPXOAUTH2_DIALOG };
#endif

  protected:
    virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support

    // Implementation
  protected:
    HICON m_hIcon;

    CWinThread *_pThread;
    std::unique_ptr<CLoginDlg> _pLoginDialog;
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

    DeviceCodeFlow generateDeviceCodeFlow(std::string);
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedButtonRequestToken();
    afx_msg void OnBnClickedButtonLogout();
    afx_msg void OnBnClickedButtonSubscribeEvent();
    void handleJsonMessages(std::string jsonStr);
    JsonType determineJsonType(const nlohmann::json &json_data);
    TokenResponse handleTokenResponse(json jsonLogin);
    TokenResponseError handleTokenResponseError(json jsonLogin);
};

UINT MyThreadFunction(LPVOID pParam);
