// CDlgEdit01.cpp : implementation file
//

#include "pch.h"
#include "MFCApp01.h"
#include "CDlgEdit01.h"
#include "afxdialogex.h"

// CDlgEdit01 dialog

static CString strEditInput = _T("Input Text Here.");
static CString strEditOutput = _T("Display Text Here");

IMPLEMENT_DYNAMIC(CDlgEdit01, CDialogEx)

CDlgEdit01::CDlgEdit01(CWnd* pParent /*=nullptr*/) : CDialogEx(IDD_DLG_EDIT01, pParent) {}

CDlgEdit01::~CDlgEdit01() {}

void CDlgEdit01::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_edit_input);
	DDV_MaxChars(pDX, m_edit_input, 20);
	DDX_Control(pDX, IDC_EDIT_OUTPUT, m_edit_output);
}

BEGIN_MESSAGE_MAP(CDlgEdit01, CDialogEx)
		ON_WM_CREATE()
		ON_WM_PAINT()
		ON_EN_SETFOCUS(IDC_EDIT_INPUT, &CDlgEdit01::OnEnSetFocusEditInput)
		ON_EN_SETFOCUS(IDC_EDIT_OUTPUT, &CDlgEdit01::OnEnSetFocusEditOutput)
		//		ON_EN_CHANGE(IDC_EDIT_INPUT, &CDlgEdit01::OnChangeEditInput)
		ON_EN_UPDATE(IDC_EDIT_INPUT, &CDlgEdit01::OnUpdateEditInput)
		ON_CONTROL(BN_CLICKED, IDC_BTN_OUT_DATA, &CDlgEdit01::OnBnClickedBtnOutData)
END_MESSAGE_MAP()

// CDlgEdit01 message handlers

int CDlgEdit01::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	//GetDlgItem(IDC_EDIT_OUTPUT)->SetWindowTextW(_T("Text Display Here"));
	AfxMessageBox(_T("Dialog_edit OnCreate."));
	return 0;
}

BOOL CDlgEdit01::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//this->m_edit_input.SetString(strEditInput);
	SetDlgItemTextW(IDC_EDIT_INPUT, strEditInput);
	this->m_edit_output.SetWindowTextW(strEditOutput);

	return TRUE; // return TRUE unless you set the focus to a control
}

void CDlgEdit01::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	GetDlgItem(IDOK)->SetFocus();
}

void CDlgEdit01::OnSetFocus(CWnd* pOldWnd)
{
	CDialogEx::OnSetFocus(pOldWnd);
	//AfxMessageBox(_T("ON_WM_SETFOCUS."));
}

void CDlgEdit01::OnBnClickedBtnOutData()
{
	CString strInputTextCtl;
	GetDlgItemTextW(IDC_READ, strInputTextCtl);

	CString strIdcRead, strIdcOutput;
	strIdcRead.LoadStringW(IDC_READ);
	strIdcOutput.LoadStringW(IDC_OUTPUT);
	if (strIdcRead != strInputTextCtl) {
		CString strIn;
		m_edit_output.GetWindowTextW(strIn);
		SetDlgItemTextW(IDC_EDIT_INPUT, strIn);
		// TODO 修改CEditControl 内容及可读写状态，TextBox内容；
		SetDlgItemTextW(IDC_READ, strIdcRead);
		SetDlgItemTextW(IDC_OUTPUT, strIdcOutput);
		CEdit* editInput = static_cast<CEdit*>(GetDlgItem(IDC_EDIT_INPUT));
		editInput->SetReadOnly(FALSE);
	}
	auto capture = GetDlgItem(IDC_EDIT_INPUT)->GetCapture();
	UpdateData(TRUE);
	int length = this->m_edit_input.GetLength();
	CString strOut;
	strOut.Format(_T("Input length: %d ."), length);
	AfxMessageBox(strOut, MB_OK);
	this->m_edit_output.SetWindowTextW(this->m_edit_input);
	UpdateData(FALSE);
}

void CDlgEdit01::OnEnSetFocusEditInput()
{
	CString strInputted;
	GetDlgItem(IDC_EDIT_INPUT)->GetWindowTextW(strInputted);
	if (strInputted == strEditInput || strInputted == _T("")) {
		this->m_edit_input.SetString(_T(""));
		this->m_edit_output.SetReadOnly(FALSE);
		UpdateData(FALSE);
	}
	else {
		this->m_edit_output.SetReadOnly(TRUE);
	}
}

void CDlgEdit01::OnEnSetFocusEditOutput()
{
	CEdit* editInput = static_cast<CEdit*>(GetDlgItem(IDC_EDIT_INPUT));
	if (this->m_edit_input.GetString() == strEditInput || this->m_edit_input.GetLength() == 0) {
		CString strIdcOutput, strIdcInput;
		strIdcInput.LoadStringW(IDC_READ);
		strIdcOutput.LoadStringW(IDC_OUTPUT);
		GetDlgItem(IDC_READ)->SetWindowTextW(strIdcOutput);
		SetDlgItemTextW(IDC_OUTPUT, strIdcInput);
		editInput->SetReadOnly(TRUE);
		this->m_edit_output.SetReadOnly(FALSE);
		UpdateData(FALSE);
	}
}

//void CDlgEdit01::OnChangeEditInput()
//{
//	this->m_edit_output.SetWindowTextW(m_edit_input);
//	UpdateData(TRUE);
//}

void CDlgEdit01::OnUpdateEditInput()
{
	//UpdateData(FALSE);	// 变量同步到控件
	UpdateData(TRUE); // 控件同步到变量
	this->m_edit_output.SetWindowTextW(m_edit_input);
	UpdateData(TRUE);
}
