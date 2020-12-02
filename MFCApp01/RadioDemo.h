#pragma once

// CRadioDemo dialog

class CRadioDemo : public CDialogEx
{
DECLARE_DYNAMIC(CRadioDemo)

	explicit CRadioDemo(CWnd* pParent = nullptr); // standard constructor
	virtual ~CRadioDemo();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_RADIO01 };
#endif
private:
	CListBox m_listShape;
	CComboBox m_cmbPenColor;
	CComboBox m_cmbBrushColor;
	CStatic m_pictureDraw;
	int m_nPenType;
	int m_nBrushType;

protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support

DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioLineTypeChange();
	afx_msg void OnBnClickedRadioFillTypeChange();
	afx_msg void OnBnClickedBtnDraw();
	afx_msg void OnBnClickedBtnClear();
};
