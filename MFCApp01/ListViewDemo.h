#pragma once

#include <vector>
#include <memory>

struct Student
{
	CString strID;
	CString strName;
	CString strSex;
	CString strScoreVerbal;
	CString strScoreMath;
	CString strScoreEnglish;
};

struct Person
{
	LONG lID;
	CString strName;
	BOOL bSex;
	DOUBLE dScoreVerbal;
	DOUBLE dScoreMath;
	DOUBLE dScoreEnglish;
};

// 使用vector存储所有信息
//std::shared_ptr<std::vector<Student>> StudentsDB;	 // vector使用unique_ptr，但Student存在栈中,
extern std::vector<std::unique_ptr<Student>> vecStudents;
extern std::vector<std::shared_ptr<Person>> vecPersons;

// CListViewDemo dialog
class CListViewDemo : public CDialogEx
{
DECLARE_DYNAMIC(CListViewDemo)

	explicit CListViewDemo(CWnd* pParent = nullptr); // standard constructor
	virtual ~CListViewDemo();

	virtual BOOL OnInitDialog();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_LISTVIEW01 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

DECLARE_MESSAGE_MAP()

private:
	std::unique_ptr<Student> m_ptrStudent;
	std::shared_ptr<Person> m_ptrPerson;
	CString m_strId;
	CString m_strName;
	CString m_strVerbal;
	CString m_strMath;
	CString m_strEnglish;
	int m_nSex;
	CListCtrl m_lstStuInfo;
	CEdit m_editVerbal;
	CEdit m_editMath;
	size_t m_nCount;
	CString m_strDBFile;
	BOOL m_bFileLoaded;
	static BOOL m_bCompareWithStr;
	static UINT m_nCompareColumn;
	static BOOL m_bReverseOrder;

public:
	afx_msg void OnBnClickedAddInfo();
	afx_msg void OnEnKillFocusEditId();
	afx_msg void OnEnKillFocusEditName();
	afx_msg void OnEnKillFocusEditVerbal();
	afx_msg void OnEnKillFocusEditMath();
	afx_msg void OnEnKillFocusEditEnglish();
	//afx_msg void OnEnChangeEditId();
	afx_msg void OnBnClickedBtnDeleteInfo();
	afx_msg void OnNMClickListInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedBtnClearDb();
	afx_msg void OnOpImport();
	afx_msg void OnOpExport();
	afx_msg void OnBnClickedBtnSelectId();
	afx_msg void OnSelectedSort(UINT id);
	afx_msg void OnBnClickedBtnShowChart();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedBtnUpdateId();

private:
	BOOL CheckScore(CString& strScore);
	void CheckScore(CString& strScore, CEdit* editScore);
	BOOL OpenFile(CStdioFile& dbFile, UINT nOpenFlag);
	static int CALLBACK SortCompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	auto SelectUseID();

public:
	void menu_operator_demo();

protected:
	afx_msg LRESULT OnSearchStuId(WPARAM wParam, LPARAM lParam);
};
