
// smtp-xoauth2Dlg.cpp : implementation file
//
#include "pch.h"

#include "Emailer.h"
#include "Error.h"
#include "Helpers.h"
#include "afxdialogex.h"
#include "framework.h"
#include "smtp-xoauth2.h"
#include "smtp-xoauth2Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx {
  public:
    CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ABOUTBOX };
#endif

  protected:
    virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support

    // Implementation
  protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX) {}

void CAboutDlg::DoDataExchange(CDataExchange *pDX) {
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// Csmtpxoauth2Dlg dialog

Csmtpxoauth2Dlg::Csmtpxoauth2Dlg(CWnd *pParent /*=nullptr*/)
    : CDialogEx(IDD_SMTPXOAUTH2_DIALOG, pParent),
      m_hIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME)), _pThread(nullptr),
      _flagCancelled(false) {}

Csmtpxoauth2Dlg::~Csmtpxoauth2Dlg() { _pLoginDialog->DestroyWindow(); }

void Csmtpxoauth2Dlg::DoDataExchange(CDataExchange *pDX) {
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_SENDER_NAME, _ctrlEditSenderName);
    DDX_Control(pDX, IDC_EDIT_SENDER_EMAIL, _ctrlEditSenderEmail);
    DDX_Control(pDX, IDC_EDIT_RECIPIENTS, _ctrlEditRecipients);
    DDX_Control(pDX, IDC_EDIT_SUBJECT, _ctrlEditSubject);
    DDX_Control(pDX, IDC_EDIT_BODY, _ctrlEditBody);
}

BEGIN_MESSAGE_MAP(Csmtpxoauth2Dlg, CDialogEx)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDCANCEL, &Csmtpxoauth2Dlg::OnBnClickedCancel)
ON_BN_CLICKED(IDC_BUTTON_LOGOUT, &Csmtpxoauth2Dlg::OnBnClickedButtonLogout)
ON_STN_CLICKED(IDC_STATIC_FROM, &Csmtpxoauth2Dlg::OnStnClickedStaticFrom)
ON_BN_CLICKED(IDC_BUTTON_SEND, &Csmtpxoauth2Dlg::OnBnClickedButtonSend)
END_MESSAGE_MAP()

// Csmtpxoauth2Dlg message handlers

BOOL Csmtpxoauth2Dlg::OnInitDialog() {
    CDialogEx::OnInitDialog();

    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu *pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != nullptr) {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty()) {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }
    _flagCancelled = false;
    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);  // Set big icon
    SetIcon(m_hIcon, FALSE); // Set small icon
    _pLoginDialog = std::make_unique<CLoginDlg>(this);
    _pLoginDialog->Create(IDD_DIALOG_LOGIN, this);
    // TODO: Add extra initialization here
    _pThread = AfxBeginThread(MyThreadFunction, this, THREAD_PRIORITY_NORMAL, 0,
                              0, nullptr);
    requestToken();
    return TRUE; // return TRUE  unless you set the focus to a control
}

void Csmtpxoauth2Dlg::OnSysCommand(UINT nID, LPARAM lParam) {
    if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    } else {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Csmtpxoauth2Dlg::OnPaint() {
    if (IsIconic()) {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND,
                    reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    } else {
        CDialogEx::OnPaint();
    }
}

// The system calls this function to obtain the cursor to display while the user
// drags
//  the minimized window.
HCURSOR Csmtpxoauth2Dlg::OnQueryDragIcon() {
    return static_cast<HCURSOR>(m_hIcon);
}

void Csmtpxoauth2Dlg::login() {
    this->EnableWindow(FALSE);

    std::string jsonString = OAuth2DeviceCodeFlow::login();

    size_t byte_sent =
        _client.Send(jsonString.c_str(), (int)jsonString.length());

    _pLoginDialog->ShowWindow(SW_SHOW);
    _pLoginDialog->UpdateWindow();
}

void Csmtpxoauth2Dlg::requestToken() {
    std::string jsonString = OAuth2DeviceCodeFlow::requestToken();

    size_t byte_sent =
        _client.Send(jsonString.c_str(), (int)jsonString.length());
}

void Csmtpxoauth2Dlg::OnBnClickedButtonLogout() {
    std::string jsonString = OAuth2DeviceCodeFlow::logout();

    size_t byte_sent =
        _client.Send(jsonString.c_str(), (int)jsonString.length());
}

UINT MyThreadFunction(LPVOID pParam) {
    // Your thread logic here
    try {
        Csmtpxoauth2Dlg *dlg = static_cast<Csmtpxoauth2Dlg *>(pParam);
        while (true) {
            std::string jsonStr;

            size_t bytes_read = dlg->_client.Receive(jsonStr);
            if (bytes_read != SOCKET_ERROR && !dlg->_flagCancelled) {
                dlg->handleJsonMessages(jsonStr);
            } else {
                break;
            }
        }
    } catch (const SmtpError e) {
    }
    return 0; // Return the thread exit code
}

JsonType Csmtpxoauth2Dlg::determineJsonType(const nlohmann::json &json_data) {

    if (json_data.find("device_code") != json_data.end()) {
        return JsonType::LoginReply;
    } else if (json_data.find("access_token") != json_data.end()) {
        return JsonType::TokenResponse;
    } else if (json_data.find("sender_name") != json_data.end()) {
        return JsonType::ProfileResponse;
    } else if (json_data.find("error") != json_data.end()) {
        return JsonType::Error;
    } else if (json_data.find("event") != json_data.end()) {
        if (json_data.find("param") != json_data.end()) {
            json response = json_data.at("param");
            if (response.find("error_code") != response.end()) {
                return JsonType::TokenResponseError;
            } else if (response.find("access_token") != response.end()) {
                return JsonType::TokenResponse;
            } else {
                return JsonType::DeviceCodeFlowResponse;
            }
        } else if (json_data.find("error") != json_data.end()) {
            return JsonType::Error;
        } else {
            return JsonType::Unknown;
        }
    } else if (json_data.find("error_code") != json_data.end()) {
        return JsonType::TokenResponseError;
    } else if (json_data.is_string()) {

        if (json_data.get<std::string>() == "success") {
            return JsonType::EmailResponse;
        } else {
            return JsonType::Error;
        }
    } else if (json_data.is_boolean()) {
        if (json_data.get<bool>() == true) {
            return JsonType::LogoutResponse;
        } else {
            return JsonType::Error;
        }
    } else {
        return JsonType::Unknown;
    }
}

void Csmtpxoauth2Dlg::handleJsonMessages(std::string jsonStr) {
    json raw = nlohmann::json::parse(jsonStr);

    std::vector<unsigned char> msg =
        raw.at("msg").get<std::vector<unsigned char>>();
    std::string str(msg.begin(), msg.end());

    json jsonLogin = nlohmann::json::parse(str);
    JsonType jType = determineJsonType(jsonLogin);

    try {
        switch (jType) {
        case JsonType::LoginReply: {
            LoginReply login;

            login.interval = jsonLogin.at("interval").get<int>();
            login.device_code = jsonLogin.at("device_code").get<std::string>();
            login.user_code = jsonLogin.at("user_code").get<std::string>();
            login.verification_uri =
                jsonLogin.at("verification_uri").get<std::string>();
            login.expires_in = jsonLogin.at("expires_in").get<int>();
            _pLoginDialog->SetUrl(login.verification_uri);
            _pLoginDialog->SetUserCode(login.user_code);
            break;
        }
        case JsonType::Error: {
            Error error;

            error.error = jsonLogin.at("error").get<std::string>();

            if (error.error.find("io_error") != std::string::npos) {
                login();
            } else {
                _pLoginDialog->ShowWindow(SW_HIDE);
                _pLoginDialog->UpdateWindow();
                this->EnableWindow(TRUE);
                AfxMessageBox(
                    static_cast<LPCTSTR>(Helpers::Utf8ToCString(error.error)),
                    MB_ICONERROR | MB_OK);
                this->PostMessage(WM_CLOSE);
            }
            break;
        }
        case JsonType::TokenResponse: {
            TokenResponse token = handleTokenResponse(jsonLogin);

            _pLoginDialog->ShowWindow(SW_HIDE);
            _pLoginDialog->UpdateWindow();
            this->EnableWindow(TRUE);

            Profile profile;
            profile.access_token = token.access_token;
            profile.profile_endpoint = "https://outlook.office.com/api/v2.0/me";

            std::string request = RequestProfile::toJson(profile);
            this->access_token = profile.access_token;
            this->_client.Send(request.c_str(), (int)request.length());

            profile.access_token = token.access_token;
            profile.profile_endpoint =
                "https://outlook.office.com/api/v2.0/me/contacts";

            request = RequestContacts::toJson(profile);
            this->access_token = profile.access_token;
            this->_client.Send(request.c_str(), (int)request.length());
            break;
        }
        case JsonType::TokenResponseError: {
            TokenResponseError token = handleTokenResponseError(jsonLogin);

            CString desc = static_cast<LPCTSTR>(
                Helpers::Utf8ToCString(token.error_code_desc));
            CString code =
                static_cast<LPCTSTR>(Helpers::Utf8ToCString(token.error_code));
            CString error = _T("Error Code: ") + code + _T("\r\n\r\n") + desc;

            if (token.error_code == "no_token" ||
                token.error_code == "expired_token" ||
                token.error_code == "invalid_grant") {
                _pLoginDialog->ShowWindow(SW_HIDE);
                _pLoginDialog->UpdateWindow();

                if (CWnd *pWnd = AfxGetMainWnd()) {
                    pWnd->MessageBox(error, code, MB_ICONERROR | MB_OK);
                }
                login();
            } else if (token.error_code == "authorization_pending" ||
                       token.error_code == "slow_down") {
                _pLoginDialog->SetErrorNotice(error);
            } else if (token.error_code == "io_error") {
                login();
            }

            else {
                _pLoginDialog->ShowWindow(SW_HIDE);
                _pLoginDialog->UpdateWindow();

                if (CWnd *pWnd = AfxGetMainWnd()) {
                    pWnd->MessageBox(error, code, MB_ICONERROR | MB_OK);
                }
                this->PostMessage(WM_CLOSE);
            }
            break;
        }
        case JsonType::ProfileResponse: {
            ProfileResponse response;
            response = handleProfileResponse(jsonLogin);
            _ctrlEditSenderName.SetWindowText(static_cast<LPCTSTR>(
                Helpers::Utf8ToCString(response.sender_name)));
            _ctrlEditSenderEmail.SetWindowText(static_cast<LPCTSTR>(
                Helpers::Utf8ToCString(response.sender_email)));

            this->sender_email = response.sender_email;
            this->sender_name = response.sender_name;

            break;
        }
        case JsonType::LogoutResponse: {
            bool response = false;

            response = jsonLogin.get<bool>();
            if (response) {
                login();
            }
            break;
        }
        case JsonType::EmailResponse: {
            if (jsonLogin.get<std::string>() == "success") {
                AfxMessageBox(_T("The E-mail was successfully sent!"),
                              MB_ICONINFORMATION | MB_OK);
            }
            break;
        }
        case JsonType::DeviceCodeFlowResponse: {
            DeviceCodeFlowResponse response =
                OAuth2DeviceCodeFlow::toDeviceCodeFlowResponse(jsonLogin);
            break;
        }
        default: {
            break;
        }
        }
    } catch (const nlohmann::json::exception &e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
    }
}

TokenResponse Csmtpxoauth2Dlg::handleTokenResponse(json jsonLogin) {
    TokenResponse token;
    std::string dom = jsonLogin.dump();

    if (jsonLogin.find("param") == jsonLogin.end()) {
        token.access_token = jsonLogin.at("access_token").get<std::string>();
        token.refresh_token = jsonLogin.at("refresh_token").get<std::string>();
        token.scopes = jsonLogin.at("scopes").get<std::vector<std::string>>();
        token.expires_in.secs =
            jsonLogin.at("expires_in").at("secs").get<int>();
        token.expires_in.nanos =
            jsonLogin.at("expires_in").at("nanos").get<int>();
        token.token_receive_time.secs =
            jsonLogin.at("token_receive_time").at("secs").get<int>();
        token.token_receive_time.nanos =
            jsonLogin.at("token_receive_time").at("nanos").get<int>();
    } else {
        token.access_token =
            jsonLogin.at("param").at("access_token").get<std::string>();
        token.refresh_token =
            jsonLogin.at("param").at("refresh_token").get<std::string>();
        token.scopes =
            jsonLogin.at("param").at("scopes").get<std::vector<std::string>>();
        token.expires_in.secs =
            jsonLogin.at("param").at("expires_in").at("secs").get<int>();
        token.expires_in.nanos =
            jsonLogin.at("param").at("expires_in").at("nanos").get<int>();
        token.token_receive_time.secs = jsonLogin.at("param")
                                            .at("token_receive_time")
                                            .at("secs")
                                            .get<int>();
        token.token_receive_time.nanos = jsonLogin.at("param")
                                             .at("token_receive_time")
                                             .at("nanos")
                                             .get<int>();
    }
    return token;
}

TokenResponseError Csmtpxoauth2Dlg::handleTokenResponseError(json jsonLogin) {

    TokenResponseError error = {};

    std::string dom = jsonLogin.dump();

    if (jsonLogin.find("param") == jsonLogin.end()) {
        error.error_code = jsonLogin.at("error_code").get<std::string>();
        error.error_code_desc =
            jsonLogin.at("error_code_desc").get<std::string>();
    } else {
        error.error_code =
            jsonLogin.at("param").at("error_code").get<std::string>();
        error.error_code_desc =
            jsonLogin.at("param").at("error_code_desc").get<std::string>();
    }
    return error;
}

ProfileResponse Csmtpxoauth2Dlg::handleProfileResponse(json jsonLogin) {
    ProfileResponse profile = {};

    std::string dom = jsonLogin.dump();

    profile.sender_name = jsonLogin.at("sender_name").get<std::string>();
    profile.sender_email = jsonLogin.at("sender_email").get<std::string>();
    return profile;
}
void Csmtpxoauth2Dlg::OnStnClickedStaticFrom() {
    // TODO: Add your control notification handler code here
}

void Csmtpxoauth2Dlg::OnBnClickedButtonSend() {

    EmailInfo info = {};

    info.access_token = this->access_token;
    info.smtp_port = 587;
    info.smtp_server = "smtp.office365.com";
    info.sender = {this->sender_name, this->sender_email};

    CString recipient, subject, body;
    _ctrlEditRecipients.GetWindowText(recipient);
    _ctrlEditSubject.GetWindowText(subject);
    _ctrlEditBody.GetWindowText(body);

    info.subject = Helpers::CStringToUtf8(subject);
    info.text_body = Helpers::CStringToUtf8(body);

    info.recipients = {
        {Helpers::CStringToUtf8(recipient), Helpers::CStringToUtf8(recipient)}};

    std::string data = Emailer::toJson(info);
    _client.Send(data.c_str(), (int)data.length());
}

void Csmtpxoauth2Dlg::OnBnClickedCancel() {
    _pThread->SuspendThread();
    std::string jsonString = OAuth2DeviceCodeFlow::cancel();

    size_t byte_sent =
        _client.Send(jsonString.c_str(), (int)jsonString.length());

    _flagCancelled = true;

    CDialogEx::OnCancel();
}
