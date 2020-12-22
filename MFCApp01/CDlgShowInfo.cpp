// CDlgShowInfo.cpp : implementation file
//

#include "pch.h"
#include "MFCApp01.h"
#include "CDlgShowInfo.h"
#include "afxdialogex.h"

// CDlgShowInfo dialog

IMPLEMENT_DYNAMIC(CDlgShowInfo, CDialogEx)

CDlgShowInfo::CDlgShowInfo(CWnd* pParent /*=nullptr*/) : CDialogEx(IDD_DLG_SHOW_ID, pParent),
                                                         m_strID(_T("")),
                                                         m_strName(_T("")),
                                                         m_strVerbal(_T("")),
                                                         m_strMath(_T("")),
                                                         m_strEnglish(_T(""))
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif
}

CDlgShowInfo::~CDlgShowInfo() {}

void CDlgShowInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SHOW_ID, m_strID);
	DDX_Text(pDX, IDC_EDIT_SHOW_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_SHOW_VERBAL, m_strVerbal);
	DDX_Text(pDX, IDC_EDIT_SHOW_MATH, m_strMath);
	DDX_Text(pDX, IDC_EDIT_SHOW_ENGLISH, m_strEnglish);
}

BEGIN_MESSAGE_MAP(CDlgShowInfo, CDialogEx)
		ON_BN_CLICKED(IDOK_SHOW, &CDlgShowInfo::OnBnClickedShow)
END_MESSAGE_MAP()

// CDlgShowInfo message handlers


BOOL CDlgShowInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CFont fontHead;
	fontHead.CreateFont(16, 0, 0, 0, FW_BOLD, FALSE, FALSE,
	                    0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
	                    DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("微软雅黑"));

	this->m_strID = m_pStudent->strID;
	this->m_strName = m_pStudent->strName;
	this->m_strVerbal = m_pStudent->strScoreVerbal;
	this->m_strMath = m_pStudent->strScoreMath;
	this->m_strEnglish = m_pStudent->strScoreEnglish;
		GetDlgItem(IDC_TXT_HEAD_SHOW)->SetFont(&fontHead, FALSE);

	if (m_bShowState) {
		SetDlgItemTextW(IDC_TXT_HEAD_SHOW, _T("学生信息查询"));

		static_cast<CEdit*>(GetDlgItem(IDC_EDIT_SHOW_NAME))->SetReadOnly(TRUE);
		static_cast<CEdit*>(GetDlgItem(IDC_EDIT_SHOW_VERBAL))->SetReadOnly(TRUE);
		static_cast<CEdit*>(GetDlgItem(IDC_EDIT_SHOW_VERBAL))->SetReadOnly(TRUE);
		static_cast<CEdit*>(GetDlgItem(IDC_EDIT_SHOW_ENGLISH))->SetReadOnly(TRUE);
		static_cast<CEdit*>(GetDlgItem(IDC_EDIT_SHOW_MATH))->SetReadOnly(TRUE);
		GetDlgItem(ID_CANCEL_INFO)->ShowWindow(SW_HIDE);
	}
	else {
		SetDlgItemTextW(IDC_TXT_HEAD_SHOW, _T("学生信息更新"));
		static_cast<CEdit*>(GetDlgItem(IDC_EDIT_SHOW_ID))->SetReadOnly();
	}


	UpdateData(FALSE);

	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgShowInfo::OnBnClickedShow()
{
	EndDialog(0);
}



BOOL CDlgShowInfo::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_LBUTTONDBLCLK) {
		CRect rc;
		static_cast<CEdit*>(GetDlgItem(IDC_EDIT_SHOW_ID))->GetWindowRect(&rc);
		if(rc.PtInRect(pMsg->pt)) {
			MessageBox(_T("ID double clicked."));
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
