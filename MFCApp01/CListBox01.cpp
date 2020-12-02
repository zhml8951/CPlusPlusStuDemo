// CListBox01.cpp : implementation file
//

#include "pch.h"
#include "MFCApp01.h"
#include "CListBox01.h"
#include "afxdialogex.h"

// CListBox01 dialog

IMPLEMENT_DYNAMIC(CListBox01, CDialogEx)

CListBox01::CListBox01(CWnd* pParent /*=nullptr*/) : CDialogEx(IDD_DLG_LIST01, pParent)
                                                     , m_showSelected(_T("")) {}

CListBox01::~CListBox01() {}

void CListBox01::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SELECT, m_selectList);
	DDX_Text(pDX, IDC_EDIT_SHOW_SELECTED, m_showSelected);
}

BEGIN_MESSAGE_MAP(CListBox01, CDialogEx)
		ON_LBN_SELCHANGE(IDC_LIST_SELECT, &CListBox01::OnLbnSelChangeList1)
END_MESSAGE_MAP()

// CListBox01 message handlers

BOOL CListBox01::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CListBox* listBoxSel = static_cast<CListBox*>(GetDlgItem(IDC_LIST_SELECT));

	m_selectList.AddString(_T("MFC_COM"));
	m_selectList.AddString(_T("MFC_OLE"));
	m_selectList.AddString(_T("MFC_ActiveX"));
	m_selectList.AddString(_T("MFC_OCX"));
	m_selectList.AddString(_T("MFC_DLL"));
	m_selectList.AddString(_T("WM_prefix Message"));
	m_selectList.AddString(_T("Control Notifications"));
	m_selectList.AddString(_T("WM_COMMAND"));
	m_selectList.AddString(_T("OnNotify virtual function"));

	m_selectList.SetCurSel(0);

	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CListBox01::OnLbnSelChangeList1()
{
	CString strSelText;
	int nCurSelect = m_selectList.GetCurSel();
	CString strCount;
	strCount.Format(_T("ListBox Count: %d"), nCurSelect);
	int nListCount = m_selectList.GetCount();
	m_selectList.GetText(nCurSelect, strSelText);
	this->m_showSelected.SetString(strSelText);
	//AfxMessageBox(strCount);
	UpdateData(FALSE);
}
