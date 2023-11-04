
// smtp-xoauth2Dlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "smtp-xoauth2.h"
#include "smtp-xoauth2Dlg.h"
#include "afxdialogex.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Csmtpxoauth2Dlg dialog



Csmtpxoauth2Dlg::Csmtpxoauth2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SMTPXOAUTH2_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    _pThread = NULL;
}

void Csmtpxoauth2Dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_INPUT_AREA, _editInputArea);
    DDX_Control(pDX, IDC_EDIT_OUTPUT, _editResponseArea);
}

BEGIN_MESSAGE_MAP(Csmtpxoauth2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
        ON_BN_CLICKED(IDOK, &Csmtpxoauth2Dlg::OnBnClickedOk)
        END_MESSAGE_MAP()


// Csmtpxoauth2Dlg message handlers

BOOL Csmtpxoauth2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
    _pThread = AfxBeginThread(
            MyThreadFunction, this, THREAD_PRIORITY_NORMAL, 0, 0, nullptr);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void Csmtpxoauth2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Csmtpxoauth2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR Csmtpxoauth2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Csmtpxoauth2Dlg::OnBnClickedOk() {
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
	
}

UINT MyThreadFunction(LPVOID pParam) {
    // Your thread logic here
    Csmtpxoauth2Dlg *dlg = static_cast<Csmtpxoauth2Dlg *>(pParam);
    while (true) {
        std::unique_ptr<char[]> buffer(new char[UINT16_MAX]);
        int bytes_read = dlg->_client.Receive(buffer.get(), UINT16_MAX);
        if (bytes_read != SOCKET_ERROR) {
            // Calculate the required buffer size for the wide
            // character string
			int bufferSize = MultiByteToWideChar(
				CP_UTF8, 0, buffer.get(),
										-1, nullptr, 0);

            // Create a unique_ptr for the wide character string
            std::unique_ptr<wchar_t[]> wideBuffer(
                new wchar_t[bytes_read + 1]);

            // Perform the conversion from multi-byte to wide
            // character
            MultiByteToWideChar(CP_UTF8, 0, buffer.get(), -1,
                                wideBuffer.get(), bytes_read);
            wideBuffer[bytes_read] = L'\0';

			dlg->_editResponseArea.SendMessage(EM_SETSEL, -1, -1);
            dlg->_editResponseArea.SendMessage(EM_REPLACESEL, TRUE, (LPARAM)wideBuffer.get());
            dlg->_editResponseArea.SendMessage( EM_SETSEL, -1, -1);
            dlg->_editResponseArea.SendMessage(EM_REPLACESEL, TRUE, (LPARAM)L"\r\n\r\n");
        } else {
            break;
		}
    }
    return 0; // Return the thread exit code
}