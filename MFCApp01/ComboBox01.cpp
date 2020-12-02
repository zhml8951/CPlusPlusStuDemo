// ComboBox01.cpp : implementation file
//

#include "pch.h"
#include "MFCApp01.h"
#include "ComboBox01.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CComboBox01, CDialogEx)

CComboBox01::CComboBox01(CWnd* pParent /*=nullptr*/) : CDialogEx(IDD_COMBOBOX01, pParent)
                                                       , m_strBaseInfo(_T(""))
                                                       , m_strAdvInfo(_T(""))
                                                       , m_strOtherInfo(_T("")) {}

CComboBox01::~CComboBox01() {}

void CComboBox01::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_COMBO_BASE, m_strBaseInfo);
	DDX_CBString(pDX, IDC_COMBO_ADV, m_strAdvInfo);
	DDX_CBString(pDX, IDC_COMBO_OTHER, m_strOtherInfo);
	DDX_Control(pDX, IDC_EDIT_BASE_INFO, m_editBoxBase);
	DDX_Control(pDX, IDC_EDIT_ADV_INFO, m_editBoxAdv);
	DDX_Control(pDX, IDC_EDIT_OTHER_INFO, m_editBoxOther);
	DDX_Control(pDX, IDC_COMBO_BASE, m_comboBaseInfo);
	DDX_Control(pDX, IDC_COMBO_ADV, m_comboAdvInfo);
	DDX_Control(pDX, IDC_COMBO_OTHER, m_comboOtherInfo);
}

BEGIN_MESSAGE_MAP(CComboBox01, CDialogEx)
		ON_WM_CREATE()
	ON_BN_CLICKED(IDOK, &CComboBox01::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_BASE, &CComboBox01::OnCbnSelChangeComboBase)
	ON_CBN_SELCHANGE(IDC_COMBO_ADV, &CComboBox01::OnCbnSelChangeComboAdv)
	ON_CBN_EDITUPDATE(IDC_COMBO_ADV, &CComboBox01::OnCbnEditUpdateComboAdv)
END_MESSAGE_MAP()

// CComboBox01 message handlers

int CComboBox01::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL CComboBox01::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_comboBaseInfo.AddString(_T("MFC CString"));
	m_comboBaseInfo.AddString(_T("MFC_CObject"));
	m_comboBaseInfo.SetCurSel(0);

	m_comboAdvInfo.AddString(_T("MFC_CEditBox"));
	m_comboAdvInfo.AddString(_T("MFC_CButton"));

	m_comboOtherInfo.AddString(_T("MFC CmdTarget"));
	m_comboOtherInfo.AddString(_T("MFC CView"));

	m_editBoxBase.SetReadOnly(TRUE);
	m_editBoxAdv.SetHighlight(0,10);
	m_editBoxOther.SetModify(FALSE);

	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CComboBox01::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void CComboBox01::OnCbnSelChangeComboBase()
{
	CString strComboText;
	INT nSel = this->m_comboBaseInfo.GetCurSel();
	m_comboBaseInfo.GetLBText(nSel, strComboText);

	this->m_editBoxBase.SetWindowTextW(strComboText);

	UpdateData(FALSE);
	UpdateData(TRUE);
}


void CComboBox01::OnCbnSelChangeComboAdv()
{
	CString strComboText;
	DWORD nSel = m_comboAdvInfo.GetEditSel();
	CString strDisplayText;
	strDisplayText.Format(_T("adv combo edit sel: %u"), nSel);
	m_editBoxAdv.SetWindowTextW(strDisplayText);
	UpdateData(FALSE);
	UpdateData(TRUE);
}


void CComboBox01::OnCbnEditUpdateComboAdv()
{
	UpdateData(TRUE);
	m_editBoxAdv.SetWindowTextW(m_strAdvInfo);
	UpdateData(FALSE);
}
