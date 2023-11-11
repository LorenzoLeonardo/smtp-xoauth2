
// smtp-xoauth2Dlg.cpp : implementation file
//
#include "pch.h"

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
}

BEGIN_MESSAGE_MAP(Csmtpxoauth2Dlg, CDialogEx)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDOK, &Csmtpxoauth2Dlg::OnBnClickedOk)
ON_BN_CLICKED(IDCANCEL, &Csmtpxoauth2Dlg::OnBnClickedCancel)
ON_BN_CLICKED(IDC_BUTTON_REQUEST_TOKEN,
              &Csmtpxoauth2Dlg::OnBnClickedButtonRequestToken)
ON_BN_CLICKED(IDC_BUTTON_LOGOUT, &Csmtpxoauth2Dlg::OnBnClickedButtonLogout)
ON_BN_CLICKED(IDC_BUTTON_SUBSCRIBE_EVENT,
              &Csmtpxoauth2Dlg::OnBnClickedButtonSubscribeEvent)
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

void Csmtpxoauth2Dlg::OnBnClickedOk() {
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

void Csmtpxoauth2Dlg::OnBnClickedCancel() {
    DeviceCodeFlow flow = generateDeviceCodeFlow("cancel");

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
    CDialogEx::OnCancel();
}

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

void Csmtpxoauth2Dlg::OnBnClickedButtonSubscribeEvent() {
    // TODO: Add your control notification handler code here

    json flowJson = {{"event_name", "oauth2"}};

    std::string jsonString = flowJson.dump();

    size_t byte_sent =
        _client.Send(jsonString.c_str(), (int)jsonString.length());
    std::cerr << "Bytes sent: " << byte_sent << std::endl;
}

UINT MyThreadFunction(LPVOID pParam) {
    // Your thread logic here
    Csmtpxoauth2Dlg *dlg = static_cast<Csmtpxoauth2Dlg *>(pParam);
    while (true) {

        std::unique_ptr<char[]> buffer(new char[UINT16_MAX]);
        int bytes_read = dlg->_client.Receive(buffer.get(), UINT16_MAX);
        if (bytes_read != SOCKET_ERROR) {

            std::string jsonStr(buffer.get(), bytes_read);
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
        } else {
            return JsonType::Unknown;
        }
    } else if (json_data.find("error") != json_data.end()) {
        return JsonType::Error;
    } else if (json_data.find("event") != json_data.end()) {
        if (json_data.find("result") != json_data.end()) {
            json response = json_data.at("result");
            return JsonType::TokenResponse;
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
        }
        case JsonType::Error: {
            Error error;

            error.error = jsonLogin.at("error").get<std::string>();
        }
        case JsonType::TokenResponse: {
            TokenResponse token = handleTokenResponse(jsonLogin);

            _pLoginDialog->ShowWindow(SW_HIDE);
            _pLoginDialog->UpdateWindow();
        }
        default: {
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

/* void Csmtpxoauth2Dlg::OnBnClickedOk() {
CString input;

_editInputArea.GetWindowTextW(input);

int wideStringLength = input.GetLength();


WCHAR *narrowStringBuffer = input.GetBuffer(wideStringLength);

int bufferSize =
WideCharToMultiByte(CP_UTF8, 0, narrowStringBuffer,
                    wideStringLength,
                    NULL, 0, NULL, NULL);

std::unique_ptr<char[]> narrowString(new char[bufferSize + 1]);
std::fill(narrowString.get(), narrowString.get() + bufferSize + 1, '\0');

WideCharToMultiByte(CP_UTF8, 0, narrowStringBuffer, wideStringLength,
                narrowString.get(), bufferSize, NULL, NULL);

int byte_sent = _client.Send(narrowString.get(), bufferSize);
std::cerr << "Bytes sent: " << byte_sent << std::endl;

input.ReleaseBuffer();

}*/
