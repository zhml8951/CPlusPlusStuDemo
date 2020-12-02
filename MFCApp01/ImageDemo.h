#pragma once

// CImageDemo dialog

class CImageDemo : public CDialogEx
{
	DECLARE_DYNAMIC(CImageDemo)

public:
	CImageDemo(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CImageDemo();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_IMAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnLoadImage();
private:
	CStatic m_imgControl;
public:
	afx_msg void OnBnClickedBtnClearImage();
};
