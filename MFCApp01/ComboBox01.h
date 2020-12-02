#pragma once

class CComboBox01 : public CDialogEx
{
	DECLARE_DYNAMIC(CComboBox01)

public:
	explicit CComboBox01(CWnd* pParent = nullptr); // standard constructor
	virtual ~CComboBox01();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMBOBOX01 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

private:
	CString m_strBaseInfo;
	CString m_strAdvInfo;
	CString m_strOtherInfo;
	CEdit m_editBoxBase;
	CEdit m_editBoxAdv;
	CEdit m_editBoxOther;
	CComboBox m_comboBaseInfo;
	CComboBox m_comboAdvInfo;
	CComboBox m_comboOtherInfo;

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelChangeComboBase();
	afx_msg void OnCbnSelChangeComboAdv();
	afx_msg void OnCbnEditUpdateComboAdv();
};
