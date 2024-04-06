
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
    ProfileResponse,
    LogoutResponse,
    EmailResponse,
    DeviceCodeFlowResponse,
    Event,
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

struct ProfileResponse {
    std::string sender_name;
    std::string sender_email;
};

struct LogoutResponse {
    bool response;
};

struct EmailResponse {
    std::string response;
};

struct Event {};
// Csmtpxoauth2Dlg dialog
class Csmtpxoauth2Dlg : public CDialogEx {
    // Construction
  public:
    Csmtpxoauth2Dlg(CWnd *pParent = nullptr); // standard constructor
    ~Csmtpxoauth2Dlg();
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
    std::string access_token;
    std::string sender_name;
    std::string sender_email;
    std::unique_ptr<CLoginDlg> _pLoginDialog;
    // Generated message map functions
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
  public:
    CEdit _ctrlEditSenderName;
    CEdit _ctrlEditSenderEmail;
    CEdit _ctrlEditRecipients;
    CEdit _ctrlEditSubject;
    CEdit _ctrlEditBody;

    TcpClient _client;
    CWinThread *_pThread;
    bool _flagCancelled;
    void login();
    void requestToken();
    void handleJsonMessages(std::string jsonStr);

    JsonType determineJsonType(const nlohmann::json &json_data);
    TokenResponse handleTokenResponse(json jsonLogin);
    TokenResponseError handleTokenResponseError(json jsonLogin);
    ProfileResponse handleProfileResponse(json jsonProfile);

    afx_msg void OnStnClickedStaticFrom();
    afx_msg void OnBnClickedButtonSend();
    afx_msg void OnBnClickedButtonLogout();
    afx_msg void OnBnClickedCancel();
};

UINT MyThreadFunction(LPVOID pParam);
