
// smtp-xoauth2Dlg.cpp : implementation file
//
#include "pch.h"

#include "Emailer.h"
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
    : CDialogEx(IDD_SMTPXOAUTH2_DIALOG, pParent) {
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    _pThread = NULL;
}

void Csmtpxoauth2Dlg::DoDataExchange(CDataExchange *pDX) {
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_OUTPUT, _editResponseArea);
    DDX_Control(pDX, IDC_EDIT_SENDER_NAME, _ctrlEditSenderName);
    DDX_Control(pDX, IDC_EDIT_SENDER_EMAIL, _ctrlEditSenderEmail);
}

BEGIN_MESSAGE_MAP(Csmtpxoauth2Dlg, CDialogEx)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
// ON_BN_CLICKED(IDOK, &Csmtpxoauth2Dlg::OnBnClickedOk)
// ON_BN_CLICKED(IDCANCEL, &Csmtpxoauth2Dlg::OnBnClickedCancel)
ON_BN_CLICKED(IDC_BUTTON_REQUEST_TOKEN,
              &Csmtpxoauth2Dlg::OnBnClickedButtonRequestToken)
ON_BN_CLICKED(IDC_BUTTON_LOGOUT, &Csmtpxoauth2Dlg::OnBnClickedButtonLogout)
// ON_BN_CLICKED(IDC_BUTTON_SUBSCRIBE_EVENT,
//               &Csmtpxoauth2Dlg::OnBnClickedButtonSubscribeEvent)
ON_STN_CLICKED(IDC_STATIC_FROM, &Csmtpxoauth2Dlg::OnStnClickedStaticFrom)
ON_BN_CLICKED(IDC_BUTTON_SEND, &Csmtpxoauth2Dlg::OnBnClickedButtonSend)
// ON_BN_CLICKED(IDC_BUTTON_PROFILE, &Csmtpxoauth2Dlg::OnBnClickedButtonProfile)
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

DeviceCodeFlow Csmtpxoauth2Dlg::generateDeviceCodeFlow(std::string method) {
    DeviceCodeFlow flow;

    flow.object = "oauth2.device.code.flow";
    flow.method = method;
    flow.param.authorization_endpoint =
        "https://login.microsoftonline.com/common/oauth2/v2.0/authorize";
    flow.param.device_auth_endpoint =
        "https://login.microsoftonline.com/common/oauth2/v2.0/devicecode";
    flow.param.token_endpoint =
        "https://login.microsoftonline.com/common/oauth2/v2.0/token";
    flow.param.client_id = "64c5d510-4b7e-4a18-8869-89778461c266";
    flow.param.process = "smtp-xoauth2";
    flow.param.provider = "Microsoft";
    flow.param.scopes.push_back("offline_access");
    flow.param.scopes.push_back("https://outlook.office.com/SMTP.Send");
    flow.param.scopes.push_back("https://outlook.office.com/User.Read");

    return flow;
}

void Csmtpxoauth2Dlg::login() {
    this->EnableWindow(FALSE);
    DeviceCodeFlow flow = generateDeviceCodeFlow("login");

    json listen_event = {{"event_name", "oauth2"}};
    std::string jsonString = listen_event.dump();
    size_t byte_sent =
        _client.Send(jsonString.c_str(), (int)jsonString.length());
    std::cerr << "Bytes sent: " << byte_sent << std::endl;

    json flowJson = {
        {"object", flow.object},
        {"method", flow.method},
        {"param",
         {{"process", flow.param.process},
          {"provider", flow.param.provider},
          {"scopes", flow.param.scopes},
          {"authorization_endpoint", flow.param.authorization_endpoint},
          {"token_endpoint", flow.param.token_endpoint},
          {"device_auth_endpoint", flow.param.device_auth_endpoint},
          {"client_id", flow.param.client_id}}}};

    jsonString = flowJson.dump();

    _pLoginDialog->ShowWindow(SW_SHOW);
    _pLoginDialog->UpdateWindow();
    byte_sent = _client.Send(jsonString.c_str(), (int)jsonString.length());
    std::cerr << "Bytes sent: " << byte_sent << std::endl;
}

void Csmtpxoauth2Dlg::requestToken() {
    DeviceCodeFlow flow = generateDeviceCodeFlow("requestToken");

    json flowJson = {
        {"object", flow.object},
        {"method", flow.method},
        {"param",
         {{"process", flow.param.process},
          {"provider", flow.param.provider},
          {"scopes", flow.param.scopes},
          {"authorization_endpoint", flow.param.authorization_endpoint},
          {"token_endpoint", flow.param.token_endpoint},
          {"device_auth_endpoint", flow.param.device_auth_endpoint},
          {"client_id", flow.param.client_id}}}};

    std::string jsonString = flowJson.dump();

    size_t byte_sent =
        _client.Send(jsonString.c_str(), (int)jsonString.length());
}

// void Csmtpxoauth2Dlg::OnBnClickedCancel() {
//     DeviceCodeFlow flow = generateDeviceCodeFlow("cancel");
//
//     json flowJson = {
//         {"object", flow.object},
//         {"method", flow.method},
//         {"param",
//          {{"process", flow.param.process},
//           {"provider", flow.param.provider},
//           {"scopes", flow.param.scopes},
//           {"authorization_endpoint", flow.param.authorization_endpoint},
//           {"token_endpoint", flow.param.token_endpoint},
//           {"device_auth_endpoint", flow.param.device_auth_endpoint},
//           {"client_id", flow.param.client_id}}}};
//
//     std::string jsonString = flowJson.dump();
//
//     size_t byte_sent =
//         _client.Send(jsonString.c_str(), (int)jsonString.length());
//     std::cerr << "Bytes sent: " << byte_sent << std::endl;
//     CDialogEx::OnCancel();
// }

void Csmtpxoauth2Dlg::OnBnClickedButtonRequestToken() {
    DeviceCodeFlow flow = generateDeviceCodeFlow("requestToken");

    json flowJson = {
        {"object", flow.object},
        {"method", flow.method},
        {"param",
         {{"process", flow.param.process},
          {"provider", flow.param.provider},
          {"scopes", flow.param.scopes},
          {"authorization_endpoint", flow.param.authorization_endpoint},
          {"token_endpoint", flow.param.token_endpoint},
          {"device_auth_endpoint", flow.param.device_auth_endpoint},
          {"client_id", flow.param.client_id}}}};

    std::string jsonString = flowJson.dump();

    size_t byte_sent =
        _client.Send(jsonString.c_str(), (int)jsonString.length());
    std::cerr << "Bytes sent: " << byte_sent << std::endl;
}

void Csmtpxoauth2Dlg::OnBnClickedButtonLogout() {
    DeviceCodeFlow flow = generateDeviceCodeFlow("logout");

    json flowJson = {
        {"object", flow.object},
        {"method", flow.method},
        {"param",
         {{"process", flow.param.process},
          {"provider", flow.param.provider},
          {"scopes", flow.param.scopes},
          {"authorization_endpoint", flow.param.authorization_endpoint},
          {"token_endpoint", flow.param.token_endpoint},
          {"device_auth_endpoint", flow.param.device_auth_endpoint},
          {"client_id", flow.param.client_id}}}};

    std::string jsonString = flowJson.dump();

    size_t byte_sent =
        _client.Send(jsonString.c_str(), (int)jsonString.length());
    std::cerr << "Bytes sent: " << byte_sent << std::endl;
}

// void Csmtpxoauth2Dlg::OnBnClickedButtonSubscribeEvent() {
//  TODO: Add your control notification handler code here

//    json flowJson = {{"event_name", "oauth2"}};
//
//    std::string jsonString = flowJson.dump();
//
//    size_t byte_sent =
//        _client.Send(jsonString.c_str(), (int)jsonString.length());
//    std::cerr << "Bytes sent: " << byte_sent << std::endl;
//}

UINT MyThreadFunction(LPVOID pParam) {
    // Your thread logic here
    Csmtpxoauth2Dlg *dlg = static_cast<Csmtpxoauth2Dlg *>(pParam);
    while (true) {
        char buffer[UINT16_MAX] = {};

        int bytes_read = dlg->_client.Receive(buffer, UINT16_MAX);
        if (bytes_read != SOCKET_ERROR) {

            std::string jsonStr(buffer, bytes_read);
            dlg->handleJsonMessages(jsonStr);

            dlg->_editResponseArea.SendMessage(EM_SETSEL, -1, -1);
            dlg->_editResponseArea.SendMessage(
                EM_REPLACESEL, TRUE,
                (LPARAM) static_cast<LPCTSTR>(Helpers::Utf8ToCString(jsonStr)));
            dlg->_editResponseArea.SendMessage(EM_SETSEL, -1, -1);
            dlg->_editResponseArea.SendMessage(EM_REPLACESEL, TRUE,
                                               (LPARAM)L"\r\n\r\n");
        } else {
            break;
        }
    }
    return 0; // Return the thread exit code
}

JsonType Csmtpxoauth2Dlg::determineJsonType(const nlohmann::json &json_data) {
    if (json_data.find("response") != json_data.end()) {
        json response = json_data.at("response");
        if (response.find("device_code") != response.end()) {
            return JsonType::LoginReply;
        } else if (response.find("access_token") != response.end()) {
            return JsonType::TokenResponse;
        } else if (response.find("sender_name") != response.end()) {
            return JsonType::ProfileResponse;
        } else if (json_data.at("response").get<bool>() == true) {
            return JsonType::LogoutResponse;
        } else {
            return JsonType::Unknown;
        }
    } else if (json_data.find("error") != json_data.end()) {
        return JsonType::Error;
    } else if (json_data.find("event") != json_data.end()) {
        if (json_data.find("result") != json_data.end()) {
            json response = json_data.at("result");
            if (response.find("error_code") != response.end()) {
                return JsonType::TokenResponseError;
            } else if (response.find("access_token") != response.end()) {
                return JsonType::TokenResponse;
            } else {
                return JsonType::Unknown;
            }
        } else if (json_data.find("error") != json_data.end()) {
            return JsonType::Error;
        } else {
            return JsonType::Unknown;
        }
    } else {
        return JsonType::Unknown;
    }
}

void Csmtpxoauth2Dlg::handleJsonMessages(std::string jsonStr) {
    json jsonLogin = nlohmann::json::parse(jsonStr);

    JsonType jType = determineJsonType(jsonLogin);

    try {
        switch (jType) {
        case JsonType::LoginReply: {
            LoginReply login;

            login.interval = jsonLogin.at("response").at("interval").get<int>();
            login.device_code =
                jsonLogin.at("response").at("device_code").get<std::string>();
            login.user_code =
                jsonLogin.at("response").at("user_code").get<std::string>();
            login.verification_uri = jsonLogin.at("response")
                                         .at("verification_uri")
                                         .get<std::string>();
            login.expires_in =
                jsonLogin.at("response").at("expires_in").get<int>();
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
            this->_client.Send(request.c_str(), (int)request.length());
            break;
        }
        case JsonType::TokenResponseError: {
            TokenResponseError token = handleTokenResponseError(jsonLogin);

            if (token.error_code == "no_token" ||
                token.error_code == "expired_token" ||
                token.error_code == "invalid_grant") {
                login();
            } else {

                _pLoginDialog->ShowWindow(SW_HIDE);
                _pLoginDialog->UpdateWindow();
                AfxMessageBox(static_cast<LPCTSTR>(Helpers::Utf8ToCString(
                                  token.error_code_desc)),
                              MB_ICONERROR | MB_OK);
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
            break;
        }
        case JsonType::LogoutResponse: {
            bool response = false;

            response = jsonLogin.at("response").get<bool>();
            if (response) {
                login();
            }
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
    json response;

    if (jsonLogin.find("response") != jsonLogin.end()) {
        response = jsonLogin.at("response");
    } else if (jsonLogin.find("event") != jsonLogin.end()) {
        if (jsonLogin.find("result") != jsonLogin.end()) {
            response = jsonLogin.at("result");
        }
    }

    TokenResponse token;

    token.access_token = response.at("access_token").get<std::string>();
    token.refresh_token = response.at("refresh_token").get<std::string>();
    token.scopes = response.at("scopes").get<std::vector<std::string>>();
    token.expires_in.secs = response.at("expires_in").at("secs").get<int>();
    token.expires_in.nanos = response.at("expires_in").at("nanos").get<int>();
    token.token_receive_time.secs =
        response.at("token_receive_time").at("secs").get<int>();
    token.token_receive_time.nanos =
        response.at("token_receive_time").at("nanos").get<int>();

    return token;
}

TokenResponseError Csmtpxoauth2Dlg::handleTokenResponseError(json jsonLogin) {
    json response;

    if (jsonLogin.find("response") != jsonLogin.end()) {
        response = jsonLogin.at("response");
    } else if (jsonLogin.find("event") != jsonLogin.end()) {
        if (jsonLogin.find("result") != jsonLogin.end()) {
            response = jsonLogin.at("result");
        }
    }

    TokenResponseError error;

    error.error_code = response.at("error_code").get<std::string>();
    error.error_code_desc = response.at("error_code_desc").get<std::string>();

    return error;
}

ProfileResponse Csmtpxoauth2Dlg::handleProfileResponse(json jsonProfile) {
    json response;

    if (jsonProfile.find("response") != jsonProfile.end()) {
        response = jsonProfile.at("response");
    }

    ProfileResponse profile;
    profile.sender_name = response.at("sender_name").get<std::string>();
    profile.sender_email = response.at("sender_email").get<std::string>();
    return profile;
}
void Csmtpxoauth2Dlg::OnStnClickedStaticFrom() {
    // TODO: Add your control notification handler code here
}

void Csmtpxoauth2Dlg::OnBnClickedButtonSend() {
    // TODO: Add your control notification handler code here
}

// void Csmtpxoauth2Dlg::OnBnClickedButtonProfile() {
//  TODO: Add your control notification handler code here

//    Profile profile;
//    profile.access_token =
//        "EwBQA+l3BAAUpSDGiWSEqG8SEbhMwx+LVy/"
//        "3Wu8AAUqS2MIindJ+"
//        "w4j52NO2DCfVeF42MODRx9jeX7pAAUlnwRKH2gXLFm8uXXGDZ3mmDj4p4DUayAEtlGNrx9"
//        "Kyb8k3pg5nb+"
//        "2F0MTge58tiIqiTmsISEmKfKu3R98LEusHzRY6ojGGxOSAE96O3Z2rxlCNwMenQuyYg7N5"
//        "ohkAL7yoKw/"
//        "MGG6KBoEPKhlmxfAIk4z+H+"
//        "Kg7LsZkJkoIPLTyj5I6JC03jVYLT349ENQ7jDdCnd29r6FmynwEkzu2O7npoYTRz945fAN"
//        "b5jBx/5rEM4RhFZOmzy/mCp8g0/oGRzyGLzAoaa0IiF3/"
//        "TKM2Tp6Ee8YMFSCnFVclniE5fyhuzoDZgAACBeeECDnMCTxIALWHgh3PMrC630Io19gEs4"
//        "CP/c8NfRcBPYCYUuRRnKIAHTBL48/u8VvGO/"
//        "aaoSHqXwTpzoMJyuXPr6vg0KPo+ya4uB26TAm+3Qj4Fy1pn77+"
//        "MYUfHQ7V8dnNSaMEg5Xa7HzIIZ2ohEEkAfu0PN7GuQ5rhVNcUX+6dOmhbP1rzJtnbNX+"
//        "uWfbhaQgazUT/"
//        "thFD7OaX+"
//        "z2FIFEsp26lpJCUKEchJcUFd3ewZsAFSp0InPLLKnNWu1khJafcVlEd25WMbnkW6p9Kzc3"
//        "vgeNvr/FIhlArDJmu/"
//        "iQTs1KBCZJYnXwoJr2hoNT4biS62Y2C7lfPq+rwcg4XXcQ4Cf+"
//        "crpdKrCdMgpSC9cCYiCPj7rZFv8pfygg+AZaV2oxuhtnga/5y/Nz/"
//        "4bFZhJWyVLGSiJdF648mMjx8PEOza8fwDrhZYXb/"
//        "CNayWsWhWyqGUxlC0pNdGCyxoZ2mez82dGBCNTmtp4Ja6Az5dDcrldzToVzgxGr1SYU4ft"
//        "Y961MEJjpBreJ2VKRatmcmLZ/wScMm41r72OJ4/"
//        "rSSw3AfSOEWlmJ9GPgTernCsEakALhqrvDIIwutLP3t/jyJ3hnL48Qi2/"
//        "kr787S3DcMWiAS4KrYsiOU/U30WsMqWrrRT7aVRF013iROSjGtd/pM8kO77O/"
//        "ONCdYMw0H/j7LEYxh5Bnb0qZM4DM8T1VcKKpv+mZOFJPnjPRcEsl/"
//        "44Vqh+4MmS4PWdC2o7aAI=";
//    profile.profile_endpoint = "https://outlook.office.com/api/v2.0/me";
//
//    std::string request = RequestProfile::toJson(profile);
//    this->_client.Send(request.c_str(), (int)request.length());
//}
