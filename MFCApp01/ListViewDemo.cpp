// ListViewDemo.cpp : implementation file

#include "pch.h"
#include "MFCApp01.h"
#include "ListViewDemo.h"
#include "afxdialogex.h"

#include <string>
#include <regex>

// CListViewDemo dialog
std::vector<std::unique_ptr<Student>> vecStudents;

IMPLEMENT_DYNAMIC(CListViewDemo, CDialogEx)

CListViewDemo::CListViewDemo(CWnd* pParent /*=nullptr*/) : CDialogEx(IDD_DLG_LISTVIEW01, pParent),
                                                           m_strId(_T("")), m_strName(_T("")),
                                                           m_strVerbal(_T("")), m_strMath(_T("")),
                                                           m_strEnglish(_T("")), m_nSex(0), m_nCount(0)
{
	//this->m_ptrStudent = nullptr;
	this->m_ptrPerson = nullptr;
}

CListViewDemo::~CListViewDemo() {}

BOOL CListViewDemo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 初始化listViewControl列信息
	m_lstStuInfo.InsertColumn(0, _T("学号"), LVCFMT_LEFT, 100);
	m_lstStuInfo.InsertColumn(1, _T("姓名"), LVCFMT_LEFT, 100);
	m_lstStuInfo.InsertColumn(2, _T("性别"), LVCFMT_LEFT, 50);
	m_lstStuInfo.InsertColumn(3, _T("语文成绩"), LVCFMT_LEFT, 90);
	m_lstStuInfo.InsertColumn(5, _T("数学成绩"), LVCFMT_LEFT, 90);
	m_lstStuInfo.InsertColumn(6, _T("英语成绩"), LVCFMT_LEFT, 90);

	if (vecStudents.size() > 0) {
		this->m_nCount = vecStudents.size();
		size_t size = vecStudents.size();
		CString strMsg;
		strMsg.Format(_T("vector.size: %I64d"), size);
		//MessageBox(strMsg);
		int i = 0;
		for(auto iter = vecStudents.cbegin(); iter != vecStudents.cend(); ++iter) {
			m_lstStuInfo.InsertItem(i, (*iter)->strID);
			m_lstStuInfo.SetItemText(i, 1, (*iter)->strName);
			m_lstStuInfo.SetItemText(i, 2, (*iter)->strSex);
			m_lstStuInfo.SetItemText(i, 3, (*iter)->strScoreVerbal);
			m_lstStuInfo.SetItemText(i, 4, (*iter)->strScoreMath);
			m_lstStuInfo.SetItemText(i, 5, (*iter)->strScoreEnglish );
			i++;
		}
	}

	return TRUE;
}

void CListViewDemo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ID, m_strId);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_VERBAL, m_strVerbal);
	DDX_Text(pDX, IDC_EDIT_MATH, m_strMath);
	DDX_Text(pDX, IDC_EDIT_ENGLISH, m_strEnglish);
	DDX_Radio(pDX, IDC_RADIO_BOY, m_nSex);
	DDX_Control(pDX, IDC_LIST_INFO, m_lstStuInfo);
	DDX_Control(pDX, IDC_EDIT_VERBAL, m_editVerbal);
	DDX_Control(pDX, IDC_EDIT_MATH, m_editMath);
}

BEGIN_MESSAGE_MAP(CListViewDemo, CDialogEx)
		ON_BN_CLICKED(IDC_BTN_ADD_INFO, &CListViewDemo::OnBnClickedAddInfo)
		ON_BN_CLICKED(IDC_BTN_DELETE_INFO, &CListViewDemo::OnBnClickedBtnDeleteInfo)
		//ON_EN_KILLFOCUS(IDC_EDIT_ID, &CListViewDemo::OnEnKillFocusEditId)
		//ON_EN_KILLFOCUS(IDC_EDIT_NAME, &CListViewDemo::OnEnKillFocusEditName)
		//ON_EN_KILLFOCUS(IDC_EDIT_VERBAL, &CListViewDemo::OnEnKillFocusEditVerbal)
		//ON_EN_KILLFOCUS(IDC_EDIT_MATH, &CListViewDemo::OnEnKillFocusEditMath)
		//ON_EN_KILLFOCUS(IDC_EDIT_ENGLISH, &CListViewDemo::OnEnKillFocusEditEnglish)
		//ON_EN_CHANGE(IDC_EDIT_ID, &CListViewDemo::OnEnChangeEditId)
END_MESSAGE_MAP()

// CListViewDemo message handlers

BOOL CListViewDemo::CheckScore(CString& strScore)
{
	BOOL rst = TRUE;
	std::regex re("^[1-9]\\d{0,2}\\.\\d*|^[1-9]\\d{0,2}|^0\\.\\d{1}|0$");
	USES_CONVERSION;
	std::string sScore = W2A(strScore.GetString());
	auto bScoreValid = std::regex_match(sScore, re);

	if (strScore.IsEmpty() || !bScoreValid) {
		return FALSE;
	}

	double dScore = _ttof(strScore);
	if (dScore < 0.0 || dScore > 150.0) {
		strScore = _T("0");
		return FALSE;
	}
	return rst;
}

void CListViewDemo::CheckScore(CString& strScore, CEdit* editScore)
{
	CEdit* editCurrent = static_cast<CEdit*>(GetFocus());
	strScore.Trim();

	std::regex re("^[1-9]\\d{0,2}\\.\\d*|^[1-9]\\d{0,2}|^0\\.\\d{1}|0$");
	USES_CONVERSION;
	std::string sScore = W2A(strScore.GetString());
	auto bScoreValid = std::regex_match(sScore, re);
	if (strScore.IsEmpty() || !bScoreValid) {
		MessageBox(_T("分数输入非法!"));
		editScore->SetFocus();
		editScore->SetSel(0, -1);
		UpdateData(FALSE);
		return;
	}

	if (!CheckScore(strScore)) {
		MessageBox(_T("false"));
	}
}

void CListViewDemo::OnEnKillFocusEditId()
{
	CString strID;
	static_cast<CEdit*>(GetDlgItem(IDC_EDIT_ID))->GetWindowTextW(strID);
	UpdateData(TRUE);
	m_strId.Trim();
	int len = m_strId.GetLength();
	CEdit* editID = static_cast<CEdit*>(GetDlgItem(IDC_EDIT_ID));
	if (m_strId.IsEmpty() || len < 6) {
		MessageBox(_T("学生ID输入有误，必须是6位长度!!!!"));
		m_strId = strID;
		editID->SetFocus();
		editID->SetSel(0, -1);
		UpdateData(FALSE);
	}
}

/* 响应OnChange时输入每个字符时都就响应事件，不符合逻辑. 只能使用OnKillFocus 事件
void CListViewDemo::OnEnChangeEditId()
{
	CString strID = this->m_strId;
	UpdateData(TRUE);
	int len = m_strId.GetLength();
	if (m_strId.IsEmpty() || len < 6) {
		MessageBox(_T("学生ID输入有误，必须是8位长度!!!!"));
		m_strId = strID;
		static_cast<CEdit*>(GetDlgItem(IDC_EDIT_ID))->SetFocus();
		static_cast<CEdit* > ( GetDlgItem ( IDC_EDIT_ID) ) -> SetWindowTextW ( strID ) ;
	 }
 }
 */

void CListViewDemo::OnEnKillFocusEditName()
{
	CEdit* editName = static_cast<CEdit*>(GetDlgItem(IDC_EDIT_NAME));
	CString strOrgName;
	editName->GetWindowTextW(strOrgName);
	UpdateData(TRUE);
	this->m_strName.Trim();
	CString strMsg;

	auto msgFunc = [&]() {
		MessageBox(strMsg);
		editName->SetWindowTextW(strOrgName);
		editName->SetFocus();
		editName->SetSel(0, -1);
	};

	if (m_strName.GetLength() < 2) {
		strMsg.Format(_T("姓名输入长度问题, 长度: %ld"), m_strName.GetLength());
		msgFunc();
		return;
	}

	for (int i = 0; i < m_strName.GetLength(); ++i) {
		int nCharUnicode = static_cast<int>(m_strName.GetAt(i));
		if (nCharUnicode <= '9' && nCharUnicode >= '0') {
			strMsg.Format(_T("姓名输入格式问题. 包含数字: %c"), static_cast<char>(m_strName.GetAt(i)));
			msgFunc();
			return;
		}
	}
}

void CListViewDemo::OnEnKillFocusEditVerbal()
{
	UpdateData(TRUE);
	CEdit* editVerbal = static_cast<CEdit*>(GetDlgItem(IDC_EDIT_VERBAL));
	CheckScore(this->m_strVerbal, editVerbal);
}

void CListViewDemo::OnEnKillFocusEditMath()
{
	UpdateData(TRUE);
	auto editMath = static_cast<CEdit*>(GetDlgItem(IDC_EDIT_MATH));
	CheckScore(m_strMath, editMath);
}

void CListViewDemo::OnEnKillFocusEditEnglish()
{
	UpdateData(TRUE);
	auto editEnglish = static_cast<CEdit*>(GetDlgItem(IDC_EDIT_ENGLISH));
	CheckScore(m_strEnglish, editEnglish);
}

void CListViewDemo::OnBnClickedAddInfo()
{
	UpdateData(TRUE);

	auto fnCheckID = [&]() -> BOOL {
		std::regex rgxID("^\\w{2}\\d{4}$");
		m_strId.Trim();
		USES_CONVERSION;
		std::string sID = W2A(m_strId);
		BOOL matchID = std::regex_match(sID, rgxID);
		if (m_strId.IsEmpty() || !matchID) {
			MessageBox(_T("学号ID输入有误"));
			return FALSE;
		}
		return TRUE;
	};

	auto fnCheckName = [&]() -> BOOL {
		std::regex rgxName("^\\w{2,}$");
		m_strName.Trim();
		USES_CONVERSION;
		std::string sName = W2A(m_strName);
		BOOL bMatch = std::regex_match(sName, rgxName);
		if (m_strName.IsEmpty() || !bMatch) {
			MessageBox(_T("姓名输入有误"));
			return FALSE;
		}
		return TRUE;
	};

	if (!fnCheckID()) {
		static_cast<CEdit*>(GetDlgItem(IDC_EDIT_ID))->SetFocus();
		int nIdLen = m_strId.GetLength();
		static_cast<CEdit*>(GetDlgItem(IDC_EDIT_ID))->SetSel(nIdLen, nIdLen, TRUE);
		UpdateData(FALSE);
		return;
	}

	if (!fnCheckName()) {
		static_cast<CEdit*>(GetDlgItem(IDC_EDIT_NAME))->SetFocus();
		int nLen = m_strName.GetLength();
		static_cast<CEdit*>(GetDlgItem(IDC_EDIT_NAME))->SetSel(0, nLen, TRUE);
		UpdateData(FALSE);
		return;
	}

	CString strSex = _T("男");
	if (m_nSex != 0) {
		strSex = _T("女");
	}

	//check verbal score: (0-150)
	m_strVerbal.Trim();
	if (!CheckScore(m_strVerbal)) {
		MessageBox(_T("语文分数无效!"));
		this->m_editVerbal.SetFocus();
		this->m_editVerbal.SetSel(0, -1, TRUE);
		UpdateData(FALSE);
		return;
	}

	// Check Math Score: (0-150)
	m_strMath.Trim();
	if (!CheckScore(m_strMath)) {
		MessageBox(_T("数学分数无效!"));
		m_editMath.SetFocus();
		m_editMath.SetSel(-1, -1);
		UpdateData(FALSE);
		return;
	}

	// Check English Score: (0-150)
	m_strEnglish.Trim();
	if (!CheckScore(m_strEnglish)) {
		MessageBox(_T("英语分数无效!"));
		CEdit* editEnglish = static_cast<CEdit*>(GetDlgItem(IDC_EDIT_ENGLISH));
		editEnglish->SetFocus();
		editEnglish->SetSel(0, -1, TRUE);
		UpdateData(FALSE);
		return;
	}

	CString strOut;
	strOut.Format(_T("sex: %d"), this->m_nSex);
	strOut = m_strId + m_strName + m_strVerbal + m_strMath + m_strEnglish + strSex;

	m_ptrStudent = std::make_unique<Student>();
	m_ptrStudent->strID = m_strId;
	m_ptrStudent->strName = m_strName;
	m_ptrStudent->strSex = strSex;
	m_ptrStudent->strScoreVerbal = m_strVerbal;
	m_ptrStudent->strScoreMath = m_strMath;
	m_ptrStudent->strScoreEnglish = m_strEnglish;
	vecStudents.push_back(std::move(m_ptrStudent));

	m_lstStuInfo.InsertItem(m_nCount, m_strId);
	m_lstStuInfo.SetItemText(m_nCount, 1, m_strName);
	m_lstStuInfo.SetItemText(m_nCount, 2, strSex);
	m_lstStuInfo.SetItemText(m_nCount, 3, m_strVerbal);
	m_lstStuInfo.SetItemText(m_nCount, 4, m_strMath);
	m_lstStuInfo.SetItemText(m_nCount, 5, m_strEnglish);
	m_nCount++;

	m_strId = _T("");
	m_strName = _T("");
	m_strVerbal = _T("");
	m_strMath = _T("");
	m_strEnglish = _T("");
	m_nSex = 0;
	static_cast<CEdit*>(GetDlgItem(IDC_EDIT_ID))->SetFocus();
	UpdateData(FALSE);
}


void CListViewDemo::OnBnClickedBtnDeleteInfo()
{
	std::vector<Student>::size_type size = vecStudents.size();
	size_t capacity = vecStudents.capacity();

	CString strShow;
	strShow.Format(_T("capacity: %ld, size: %ld"), long(capacity), long(size));
	MessageBox(strShow);
}
