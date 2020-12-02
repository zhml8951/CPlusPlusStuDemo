// RadioDemo.cpp : implementation file

#include "pch.h"
#include "MFCApp01.h"
#include "RadioDemo.h"
#include "afxdialogex.h"

// CRadioDemo dialog

IMPLEMENT_DYNAMIC(CRadioDemo, CDialogEx)

CRadioDemo::CRadioDemo(CWnd* pParent /*=nullptr*/) : CDialogEx(IDD_DLG_RADIO01, pParent)
                                                     , m_nPenType(0)
                                                     , m_nBrushType(0) {}

CRadioDemo::~CRadioDemo() {}

void CRadioDemo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SHAPE, m_listShape);
	DDX_Control(pDX, IDC_COMBO_PEN_COLOR, m_cmbPenColor);
	DDX_Control(pDX, IDC_COMBO_BRUSH_COLOR, m_cmbBrushColor);
	DDX_Radio(pDX, IDC_RADIO_LINE_TYPE1, m_nPenType);
	DDX_Radio(pDX, IDC_RADIO_FILL_TYPE1, m_nBrushType);
	DDX_Control(pDX, IDC_STATIC_DRAW, m_pictureDraw);
}

BEGIN_MESSAGE_MAP(CRadioDemo, CDialogEx)
		ON_BN_CLICKED(IDC_RADIO_LINE_TYPE1, &CRadioDemo::OnBnClickedRadioLineTypeChange)
		ON_BN_CLICKED(IDC_RADIO_LINE_TYPE2, &CRadioDemo::OnBnClickedRadioLineTypeChange)
		ON_BN_CLICKED(IDC_RADIO_LINE_TYPE3, &CRadioDemo::OnBnClickedRadioLineTypeChange)
		ON_BN_CLICKED(IDC_RADIO_LINE_TYPE4, &CRadioDemo::OnBnClickedRadioLineTypeChange)
		ON_BN_CLICKED(IDC_RADIO_FILL_TYPE1, &CRadioDemo::OnBnClickedRadioFillTypeChange)
		ON_BN_CLICKED(IDC_RADIO_FILL_TYPE2, &CRadioDemo::OnBnClickedRadioFillTypeChange)
		ON_BN_CLICKED(IDC_RADIO_FILL_TYPE3, &CRadioDemo::OnBnClickedRadioFillTypeChange)
		ON_BN_CLICKED(IDC_RADIO_FILL_TYPE4, &CRadioDemo::OnBnClickedRadioFillTypeChange)
		ON_BN_CLICKED(IDC_BTN_DRAW, &CRadioDemo::OnBnClickedBtnDraw)
		ON_BN_CLICKED(IDC_BTN_CLEAR, &CRadioDemo::OnBnClickedBtnClear)
END_MESSAGE_MAP()

// CRadioDemo message handlers


BOOL CRadioDemo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_listShape.AddString(_T("线  型"));
	m_listShape.AddString(_T("矩  型"));
	m_listShape.AddString(_T("圆  型"));
	m_listShape.AddString(_T("圆角矩形"));

	m_listShape.SetCurSel(0);

	m_cmbPenColor.AddString(_T("红  色"));
	m_cmbPenColor.AddString(_T("绿  色"));
	m_cmbPenColor.AddString(_T("蓝  色"));
	m_cmbPenColor.AddString(_T("黄  色"));
	m_cmbPenColor.AddString(_T("橙  色"));
	m_cmbPenColor.SetCurSel(0);

	m_cmbBrushColor.AddString(_T("白  色"));
	m_cmbBrushColor.AddString(_T("黑  色"));
	m_cmbBrushColor.AddString(_T("灰  色"));
	m_cmbBrushColor.AddString(_T("青  色"));
	m_cmbBrushColor.SetCurSel(0);

	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CRadioDemo::OnBnClickedRadioLineTypeChange()
{
	UpdateData(TRUE);
	switch (m_nPenType) {
	case 0:
		AfxMessageBox(_T("0"));
		break;
	case 1:
		AfxMessageBox(_T("1"));
		break;
	case 2:
		break;
	case 3:
		break;
	default:
		AfxMessageBox(_T("Where???"));
	}
}


void CRadioDemo::OnBnClickedRadioFillTypeChange()
{
	UpdateData(TRUE);
	switch (m_nBrushType) {
	case 0:
		AfxMessageBox(_T("0"));
		break;
	case 1:
		AfxMessageBox(_T("1"));
		break;
	case 2:
		break;
	case 3:
		break;
	default:
		AfxMessageBox(_T("Where???"));
	}
}


void CRadioDemo::OnBnClickedBtnDraw()
{
	CRect rectPic;
	m_pictureDraw.GetClientRect(&rectPic);
	CString strOut;
	//strOut.Format(_T("rect.left: %ld, top: %ld, right: %ld, buttom: %ld"),
	              //pRect->left, pRect->top, pRect->right, pRect->bottom);


	LONG left = rectPic.left ;
	strOut.Format(_T("rect_left: %ld, right: %ld, top: %ld"), left, rectPic.right, rectPic.top);
	AfxMessageBox(strOut);
}


void CRadioDemo::OnBnClickedBtnClear()
{
	// TODO: Add your control notification handler code here
}
