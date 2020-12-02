// ImageDemo.cpp : implementation file
//

#include "pch.h"
#include "MFCApp01.h"
#include "ImageDemo.h"
#include "afxdialogex.h"

// CImageDemo dialog

IMPLEMENT_DYNAMIC(CImageDemo, CDialogEx)

CImageDemo::CImageDemo(CWnd* pParent /*=nullptr*/) : CDialogEx(IDD_DLG_IMAGE, pParent) {}

CImageDemo::~CImageDemo() {}

void CImageDemo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMAGE, m_imgControl);
}

BEGIN_MESSAGE_MAP(CImageDemo, CDialogEx)
		ON_BN_CLICKED(IDC_BTN_LOAD_IMAGE, &CImageDemo::OnBnClickedBtnLoadImage)
	ON_BN_CLICKED(IDC_BTN_CLEAR_IMAGE, &CImageDemo::OnBnClickedBtnClearImage)
END_MESSAGE_MAP()

// CImageDemo message handlers

void CImageDemo::OnBnClickedBtnLoadImage()
{
	CBitmap bitmap;
	bitmap.LoadBitmapW(IDB_IMAGE_ACUMI);
	HBITMAP hBmp = static_cast<HBITMAP>(bitmap.GetSafeHandle());
	
	m_imgControl.SetBitmap(hBmp);
}


void CImageDemo::OnBnClickedBtnClearImage()
{
	auto hBmp = m_imgControl.GetBitmap();
	if(hBmp) {
		m_imgControl.SetBitmap(nullptr);
		UpdateData(FALSE);
	}
}
