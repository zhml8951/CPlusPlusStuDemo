﻿
// MFCDialog01.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CDialog01App:
// See MFCDialog01.cpp for the implementation of this class
//

class CDialog01App : public CWinApp
{
public:
	CDialog01App();
	CShellManager *pShellManager = nullptr;

// Overrides
public:
	virtual BOOL InitInstance();
	int ExitInstance() override;

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CDialog01App theApp;