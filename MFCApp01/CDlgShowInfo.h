#pragma once
#include "ListViewDemo.h"

class CDlgShowInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgShowInfo)

public:
	explicit CDlgShowInfo(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgShowInfo();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SHOW_ID };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	Student* m_pStudent;
	BOOL m_bShowState;

private:
	CString m_strID;
	CString m_strName;
	CString m_strVerbal;
	CString m_strMath;
	CString m_strEnglish;

public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedShow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
