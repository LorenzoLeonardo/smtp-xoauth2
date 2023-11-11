
// smtp-xoauth2.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h" // main symbols
#include <vector>
// Csmtpxoauth2App:
// See smtp-xoauth2.cpp for the implementation of this class
//

class Csmtpxoauth2App : public CWinApp {
  public:
    Csmtpxoauth2App();

    // Overrides
  public:
    virtual BOOL InitInstance();
    // Implementation

  protected:
    std::vector<PROCESS_INFORMATION> _processInfo;

    DECLARE_MESSAGE_MAP()
};

extern Csmtpxoauth2App theApp;
