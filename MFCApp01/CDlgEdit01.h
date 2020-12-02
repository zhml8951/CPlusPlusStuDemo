#pragma once

// CDlgEdit01 dialog

class CDlgEdit01 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgEdit01)

public:
	explicit CDlgEdit01(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgEdit01();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_EDIT01 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_edit_input;
	CEdit m_edit_output;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnEnSetFocusEditInput();
	afx_msg void OnEnSetFocusEditOutput();
	afx_msg void OnBnClickedBtnOutData();
//	afx_msg void OnChangeEditInput();
	afx_msg void OnUpdateEditInput();
};
