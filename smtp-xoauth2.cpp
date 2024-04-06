
// smtp-xoauth2.cpp : Defines the class behaviors for the application.
//
#include "pch.h"

#include "Error.h"
#include "Helpers.h"
#include "TcpClient.h"
#include "framework.h"
#include "smtp-xoauth2.h"
#include "smtp-xoauth2Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Csmtpxoauth2App

BEGIN_MESSAGE_MAP(Csmtpxoauth2App, CWinApp)
ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

// Csmtpxoauth2App construction

Csmtpxoauth2App::Csmtpxoauth2App() {
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}

// The one and only Csmtpxoauth2App object

Csmtpxoauth2App theApp;

// Csmtpxoauth2App initialization

BOOL Csmtpxoauth2App::InitInstance() {
    // InitCommonControlsEx() is required on Windows XP if an application
    // manifest specifies use of ComCtl32.dll version 6 or later to enable
    // visual styles.  Otherwise, any window creation will fail.
    INITCOMMONCONTROLSEX InitCtrls = {};
    InitCtrls.dwSize = sizeof(InitCtrls);
    // Set this to include all the common control classes you want to use
    // in your application.
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();

    if (!AfxSocketInit()) {
        AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
        return FALSE;
    }

    // Create the shell manager, in case the dialog contains
    // any shell tree view or shell list view controls.
    std::unique_ptr<CShellManager> pShellManager(new CShellManager);

    // Activate "Windows Native" visual manager for enabling themes in MFC
    // controls
    CMFCVisualManager::SetDefaultManager(
        RUNTIME_CLASS(CMFCVisualManagerWindows));

    // Standard initialization
    // If you are not using these features and wish to reduce the size
    // of your final executable, you should remove from the following
    // the specific initialization routines you do not need
    // Change the registry key under which our settings are stored
    // TODO: You should modify this string to be something appropriate
    // such as the name of your company or organization
    SetRegistryKey(_T("Local AppWizard-Generated Applications"));

    try {

        TcpClient client = TcpClient("127.0.0.1", 1986);

        client.Connect();

        std::string stream = OAuth2DeviceCodeFlow::subscribeToEvent();
        client.Send(stream.c_str(), (int)stream.length());

        Csmtpxoauth2Dlg dlg;
        dlg.SetTcpClient(client);

        INT_PTR nResponse = dlg.DoModal();

        dlg._pThread->ResumeThread();
        if (nResponse == IDOK) {
            // TODO: Place code here to handle when the dialog is
            //  dismissed with OK
        } else if (nResponse == IDCANCEL) {
            // TODO: Place code here to handle when the dialog is
            //  dismissed with Cancel
        } else if (nResponse == -1) {
            TRACE(traceAppMsg, 0,
                  "Warning: dialog creation failed, so application is "
                  "terminating "
                  "unexpectedly.\n");
            TRACE(traceAppMsg, 0,
                  "Warning: if you are using MFC controls on the dialog, you "
                  "cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
        }

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
        ControlBarCleanUp();
#endif
        return TRUE;
    } catch (const SmtpError &e) {

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
        ControlBarCleanUp();
#endif

        AfxMessageBox(Helpers::Utf8ToCString(e.what()));

        return FALSE;
    } catch (std::exception e) {
        AfxMessageBox(Helpers::Utf8ToCString(e.what()));
        return FALSE;
    }
}
