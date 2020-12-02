#pragma once

// CListBox01 dialog

class CListBox01 : public CDialogEx
{
	DECLARE_DYNAMIC(CListBox01)

public:
	explicit CListBox01(CWnd* pParent = nullptr); // standard constructor
	virtual ~CListBox01();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_LIST01 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	CListBox m_selectList;
	CString m_showSelected;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelChangeList1();
};
