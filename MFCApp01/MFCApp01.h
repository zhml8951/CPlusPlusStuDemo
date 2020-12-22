// MFCApp01.h : main header file for the MFCApp01 application
//
#pragma once

#ifndef __AFXWIN_H__
#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

// CApp01App:
// See MFCApp01.cpp for the implementation of this class
//

class CApp01App : public CWinAppEx
{
public:
	CApp01App() noexcept;

	// Overrides
public:
	virtual BOOL InitInstance();

	// 手动添加,这里主要是测试，大多数情况下不需要 Overrides CWinAppEx::ExitInstance;
	virtual int ExitInstance();

	// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()	// DECLARE_MESSAGE_MAP 一般放于class最后，如果在后面再添加member，则必须指定访问权限如：public, private, protected.

protected:
	CString sTestString = _T("text");
};

extern CApp01App theApp;
