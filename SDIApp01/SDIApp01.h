
// SDIApp01.h: SDIApp01 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // 主符号


// CSDIApp01App:
// 有关此类的实现，请参阅 SDIApp01.cpp
//

class CSDIApp01App : public CWinAppEx
{
public:
	CSDIApp01App() noexcept;


// 重写
public:
	virtual BOOL InitInstance();

// 实现
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSDIApp01App theApp;
