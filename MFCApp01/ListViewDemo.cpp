// ListViewDemo.cpp : implementation file

#include "pch.h"
#include "MFCApp01.h"
#include "ListViewDemo.h"
#include "afxdialogex.h"
#include "CDlgShowInfo.h"

#include <string>
#include <regex>
#include <list>
#include <algorithm>

// CListViewDemo dialog
std::vector<std::unique_ptr<Student>> vecStudents;

BOOL CListViewDemo::m_bCompareWithStr = FALSE;
UINT CListViewDemo::m_nCompareColumn = 0;
BOOL CListViewDemo::m_bReverseOrder = FALSE;

IMPLEMENT_DYNAMIC(CListViewDemo, CDialogEx)

CListViewDemo::CListViewDemo(CWnd* pParent /*=nullptr*/) : CDialogEx(IDD_DLG_LISTVIEW01, pParent),
                                                           m_strId(_T("")),
                                                           m_strName(_T("")),
                                                           m_strVerbal(_T("")),
                                                           m_strMath(_T("")),
                                                           m_strEnglish(_T("")),
                                                           m_nSex(0),
                                                           m_nCount(0),
                                                           m_strDBFile(_T("d:\\temp\\StudentInfo.txt")),
                                                           m_bFileLoaded(FALSE)
{
	//this->m_ptrStudent = nullptr;
	this->m_ptrPerson = nullptr;
}

CListViewDemo::~CListViewDemo() {}

int CListViewDemo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	CMenu* pMenuSort = GetMenu()->GetSubMenu(0)->GetSubMenu(2);
	CMenu* pMenuSortByID = pMenuSort->GetSubMenu(0);

	pMenuSortByID->CheckMenuItem(0, MF_BYPOSITION | MF_CHECKED);
	pMenuSortByID->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);

	pMenuSort->CheckMenuItem(0, MF_BYPOSITION | MF_CHECKED);

	return 0;
}

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
		for (auto iter = vecStudents.cbegin(); iter != vecStudents.cend(); ++iter) {
			m_lstStuInfo.InsertItem(i, (*iter)->strID);
			m_lstStuInfo.SetItemText(i, 1, (*iter)->strName);
			m_lstStuInfo.SetItemText(i, 2, (*iter)->strSex);
			m_lstStuInfo.SetItemText(i, 3, (*iter)->strScoreVerbal);
			m_lstStuInfo.SetItemText(i, 4, (*iter)->strScoreMath);
			m_lstStuInfo.SetItemText(i, 5, (*iter)->strScoreEnglish);
			i++;
		}
		m_bCompareWithStr = TRUE;
		m_nCompareColumn = 0;
		m_bReverseOrder = FALSE;
		m_lstStuInfo.SortItemsEx(SortCompareFunc, reinterpret_cast<LPARAM>(&m_lstStuInfo));

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
		ON_COMMAND(ID_OP_IMPORT, &CListViewDemo::OnOpImport)
		ON_BN_CLICKED(IDC_BTN_ADD_INFO, &CListViewDemo::OnBnClickedAddInfo)
		ON_BN_CLICKED(IDC_BTN_DELETE_INFO, &CListViewDemo::OnBnClickedBtnDeleteInfo)
		//ON_EN_KILLFOCUS(IDC_EDIT_ID, &CListViewDemo::OnEnKillFocusEditId)
		//ON_EN_KILLFOCUS(IDC_EDIT_NAME, &CListViewDemo::OnEnKillFocusEditName)
		//ON_EN_KILLFOCUS(IDC_EDIT_VERBAL, &CListViewDemo::OnEnKillFocusEditVerbal)
		//ON_EN_KILLFOCUS(IDC_EDIT_MATH, &CListViewDemo::OnEnKillFocusEditMath)
		//ON_EN_KILLFOCUS(IDC_EDIT_ENGLISH, &CListViewDemo::OnEnKillFocusEditEnglish)
		//ON_EN_CHANGE(IDC_EDIT_ID, &CListViewDemo::OnEnChangeEditId)
		ON_NOTIFY(NM_CLICK, IDC_LIST_INFO, &CListViewDemo::OnNMClickListInfo)
		ON_BN_CLICKED(IDC_BTN_CLEAR_DB, &CListViewDemo::OnBnClickedBtnClearDb)
		ON_BN_CLICKED(IDC_BTN_SELECT_ID, &CListViewDemo::OnBnClickedBtnSelectId)
		ON_COMMAND(ID_OP_EXPORT, &CListViewDemo::OnOpExport)
		ON_COMMAND_RANGE(ID_SORT_ID_ASC, ID_SORT_RANDOM, &CListViewDemo::OnSelectedSort)
		ON_BN_CLICKED(IDC_BTN_SHOW_CHART, &CListViewDemo::OnBnClickedBtnShowChart)
		ON_WM_CREATE()
		ON_BN_CLICKED(IDC_BTN_UPDATE_ID, &CListViewDemo::OnBnClickedBtnUpdateId)
		ON_MESSAGE(SEARCH_STU_ID, &CListViewDemo::OnSearchStuId)
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

	m_lstStuInfo.InsertItem(static_cast<int>(m_nCount), m_strId);
	m_lstStuInfo.SetItemText(static_cast<int>(m_nCount), 1, m_strName);
	m_lstStuInfo.SetItemText(static_cast<int>(m_nCount), 2, strSex);
	m_lstStuInfo.SetItemText(static_cast<int>(m_nCount), 3, m_strVerbal);
	m_lstStuInfo.SetItemText(static_cast<int>(m_nCount), 4, m_strMath);
	m_lstStuInfo.SetItemText(static_cast<int>(m_nCount), 5, m_strEnglish);
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
	UpdateData(TRUE);
	this->m_strId.Trim();
	CString strMsg;
	if (m_strId.IsEmpty()) {
		strMsg = _T("学号为空,删除取消!");
		MessageBox(strMsg);
		return;
	}

	if (vecStudents.size() == 0) {
		if (m_lstStuInfo.GetItemCount() > 0) {
			m_lstStuInfo.DeleteAllItems();
		}
		strMsg = _T("数据库空,删除取消!");
		MessageBox(strMsg);
		return;
	}

	/*
	 *	vector.erase(item)会导致vector内的元素位置前移，同样CListCtrl的Item使用DeleteItem也会使用元素位置前移。
	 *	如果两个需要删除的元素在一起，删除元素后再++(item++/n++), 会出现后一元素直接跳过的bug;
	 *	这里采用方法是else{时自增。 删除元素时位置下标不动;
	 */
	for (auto item = vecStudents.begin(); item != vecStudents.end();) {
		if ((*item)->strID == m_strId) {
			item = vecStudents.erase(item);
			m_nCount--;
		}
		else {
			++item;
		}
	}

	for (int n = 0; n < m_lstStuInfo.GetItemCount();) {
		CString strFount = m_lstStuInfo.GetItemText(n, 0);
		if (strFount == m_strId) {
			m_lstStuInfo.DeleteItem(n);
		}
		else {
			n++;
		}
	}

	std::vector<Student>::size_type size = vecStudents.size();
	size_t capacity = vecStudents.capacity();

	CString strShow;
	strShow.Format(_T("capacity: %I64d, size: %I64d"), capacity, size);
}

void CListViewDemo::OnNMClickListInfo(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	*pResult = 0;
	NMLISTVIEW* pNMListView = reinterpret_cast<NMLISTVIEW*>(pNMHDR);
	if (-1 != pNMListView->iItem) {
		CString strID = m_lstStuInfo.GetItemText(pNMListView->iItem, 0);
		m_strId = strID;
		m_strName = m_lstStuInfo.GetItemText(pNMListView->iItem, 1);
		UpdateData(FALSE);
	}
}

void CListViewDemo::OnBnClickedBtnClearDb()
{
	CString strMsg;
	if (vecStudents.size() > 0) {
		strMsg.Format(_T("Students.size: %I64d, capcity: %I64d"), vecStudents.size(), vecStudents.capacity());
		MessageBox(strMsg);
		std::vector<std::unique_ptr<Student>>().swap(vecStudents);
		m_lstStuInfo.DeleteAllItems();
		m_nCount = 0;
	}
	else {
		strMsg.Format(_T("Students.size: %I64d, capcity: %I64d"), vecStudents.size(), vecStudents.capacity());
		MessageBox(strMsg);
	}
}

BOOL CListViewDemo::OpenFile(CStdioFile& dbFile, const UINT nOpenFlag)
{
	CString strFilter = _T("Document Text (*.txt)|*.txt|All File (*.*)|*.*||");
	CFileDialog dlgFile(TRUE, _T("txt"), m_strDBFile, OFN_HIDEREADONLY, strFilter);
	CString strSelectedFile = _T("");
	if (dlgFile.DoModal() == IDOK) {
		strSelectedFile = dlgFile.GetPathName();
	}

	if (strSelectedFile == _T("")) {
		return FALSE;
	}

	if (dbFile.Open(strSelectedFile, nOpenFlag | CFile::typeBinary)) {
		this->m_strDBFile = strSelectedFile;
		return TRUE;
	}
	return FALSE;
}

int CListViewDemo::SortCompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CListCtrl* pListStudentInfo = reinterpret_cast<CListCtrl*>(lParamSort);
	CString strItem1 = pListStudentInfo->GetItemText(static_cast<int>(lParam1), m_nCompareColumn);
	CString strItem2 = pListStudentInfo->GetItemText(static_cast<int>(lParam2), m_nCompareColumn);
	int rst;
	if (m_bCompareWithStr) {
		rst = strItem1.CompareNoCase(strItem2);
	}
	else {
		double x1 = _tstof(strItem1.GetBuffer());
		double x2 = _tstof(strItem2.GetBuffer());
		x1 - x2 > 0 ? rst = 1 : rst = -1;
	}

	if (m_bReverseOrder) {
		rst = 0 - rst;
	}

	return rst;
}


void CListViewDemo::OnOpImport()
{
	CStdioFile fileStuDB;

	if (!OpenFile(fileStuDB, CFile::modeRead)) {
		return;
	}

	CString strLine;
	std::regex rItem(
		"^\\W?(\\w+)[\\s|\\t]+([a-zA-Z_0-9]+)[\\s|\\t]+(\\d+)[\\s|\\t]+(\\d+)[\\s|\\t]+(\\d+)[\\s|\\t]+(\\d+)[\\s|\\t]*$");
	std::regex rBlankLine("^[\\s|\\t]*$");
	std::regex rQuestionMark("^\\?+$");

	size_t nListItemNum = m_nCount;
	while (fileStuDB.ReadString(strLine)) {
		strLine.Trim();
		if (strLine == _T("")) {
			continue;
		}
		USES_CONVERSION;
		std::string cstrItem = W2A(strLine);
		std::smatch sm;
		if (std::regex_match(cstrItem, rQuestionMark)) {
			continue;
		}
		std::regex_search(cstrItem, sm, rItem);
		if (sm.size() < 2) {
			continue;
		}

		CString id = CString(sm[1].str().c_str());
		CString name = CString(sm[2].str().c_str());
		CString str_sex = CString(sm[3].str().c_str());
		CString verbal = CString(sm[4].str().c_str());
		CString math = CString(sm[5].str().c_str());
		CString english = CString(sm[6].str().c_str());

		CString sex = _T("男");
		if (str_sex != _T("0")) {
			sex = _T("女");
		}

		int num = static_cast<int>(nListItemNum);

		m_lstStuInfo.InsertItem(num, id);
		m_lstStuInfo.SetItemText(num, 1, name);
		m_lstStuInfo.SetItemText(num, 2, sex);
		m_lstStuInfo.SetItemText(num, 3, verbal);
		m_lstStuInfo.SetItemText(num, 4, math);
		m_lstStuInfo.SetItemText(num, 5, english);

		std::unique_ptr<Student> student(new Student{id, name, sex, verbal, math, english});
		vecStudents.push_back(std::move(student));

		nListItemNum++;
	}

	fileStuDB.Close();

	this->m_bFileLoaded = TRUE;
	this->m_nCount = nListItemNum;
}

void CListViewDemo::OnOpExport()
{
	CStdioFile fileExport;
	CString strDefaultFilName = _T("d:\\temp\\StudentOut.txt");
	CString strFilter = _T("Text (*.txt)|*.txt|All File (*.*)|*.*||");
	CFileDialog dlgOpenFile(TRUE, _T("txt"), strDefaultFilName, OFN_EXPLORER, strFilter);

	auto bRet = dlgOpenFile.DoModal();

	if (bRet != IDOK || vecStudents.size() == 0) {
		return;
	}

	CString strFileName = dlgOpenFile.GetPathName();

	CString strMsg;
	strMsg.Format(_T("size: %I64d"), vecStudents.size());
	MessageBox(strMsg);

	if (!fileExport.Open(strFileName,
	                     CFile::typeBinary | CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate)) {
		return;
	}

	for (auto& item : vecStudents) {
		CString strLine = item->strID + _T("\t\t") + item->strName + _T("\t\t") + item->strSex + _T("\t\t") +
			item->strScoreVerbal + _T("\t\t") + item->strScoreMath + _T("\t\t") + item->strScoreEnglish + _T("\n");
		fileExport.WriteString(strLine);
	}
	fileExport.Close();
}

void CListViewDemo::menu_operator_demo()
{
	CMenu* pMenuOperator = this->GetMenu(); // 获取当前窗口菜单, 即:  菜单栏
	CString strSortMenu;
	int nRet = pMenuOperator->GetMenuString(0, strSortMenu, MF_BYPOSITION);
	for (int n = 0; n < pMenuOperator->GetMenuItemCount(); n++) {
		// 顶层('操作')
		CMenu* pSubMenu = pMenuOperator->GetSubMenu(n);
		CString strMenu, strMsg;
		for (int m = 0; m < pSubMenu->GetMenuItemCount(); m++) {
			pSubMenu->GetMenuStringW(m, strMenu, MF_BYPOSITION);
			strMsg = strMsg + _T("  __  ") + strMenu;
		}
		MessageBox(strSortMenu + _T(" --> ") + strMsg);
	}

	CMenu* pSortMenu = this->GetMenu()->GetSubMenu(0)->GetSubMenu(2);
	pSortMenu->GetMenuStringW(0, strSortMenu, MF_BYPOSITION);
	MessageBox(strSortMenu);
}


auto CListViewDemo::SelectUseID()
{
	CString strID;
	GetDlgItemText(IDC_EDIT_ID, strID);
	strID.Trim();
	if (strID.IsEmpty()) {
		SetDlgItemText(IDC_EDIT_ID, _T(""));
		GetDlgItem(IDC_EDIT_ID)->SetFocus();
		return vecStudents.cend();
	}

	auto ret_iter = std::find_if(
		vecStudents.cbegin(), vecStudents.cend(), [&](const std::unique_ptr<Student>& stu) -> bool {
			if (stu->strID == strID) {
				return true;
			}
			return false;
		});

	//使用for循环手动进行查找，

	//std::vector<std::unique_ptr<Student>>::const_iterator ret_iter = vecStudents.cbegin();
	//for (; ret_iter != vecStudents.cend(); ++ret_iter) {
	//	if (ret_iter->get()->strID == strID) break;
	//}

	if (ret_iter == vecStudents.cend()) {
		MessageBox(_T("没有此ID的学生."));
		GetDlgItem(IDC_EDIT_ID)->SetFocus();
		static_cast<CEdit*>(GetDlgItem(IDC_EDIT_ID))->SetSel(0, -1);
		return vecStudents.cend();
	}
	return ret_iter;
}


void CListViewDemo::OnBnClickedBtnSelectId()
{
	auto item = SelectUseID();
	if (item == vecStudents.cend()) { return; }
	CDlgShowInfo dlgInfo;
	dlgInfo.m_bShowState = TRUE;
	dlgInfo.m_pStudent = item->get();
	dlgInfo.DoModal();
}

void CListViewDemo::OnBnClickedBtnUpdateId()
{
	auto item = SelectUseID();
	if (item == vecStudents.cend()) { return; }
	CDlgShowInfo dlgInfo;
	dlgInfo.m_bShowState = FALSE;
	dlgInfo.m_pStudent = item->get();
	dlgInfo.DoModal();
}

void check_item(const CMenu* pMenu, const UINT menu_item)
{
	CMenu* pMenuSort = pMenu->GetSubMenu(2);
	UINT items[] = {ID_SORT_ID_ASC, ID_SORT_ID_DESC, ID_SORT_VERBAL, ID_SORT_MATH,ID_SORT_ENGLISH, ID_SORT_RANDOM};

	if (menu_item == ID_SORT_ID_ASC || menu_item == ID_SORT_ID_DESC) {
		pMenuSort->CheckMenuItem(0, MF_BYPOSITION | MF_CHECKED);
	}
	else {
		pMenuSort->CheckMenuItem(0, MF_BYPOSITION | MF_UNCHECKED);
	}

	for (int i = 0; i < sizeof(items) / sizeof(items[0]); i++) {
		if (menu_item == items[i]) {
			pMenuSort->CheckMenuItem(items[i], MF_BYCOMMAND | MF_CHECKED);
			pMenuSort->EnableMenuItem(items[i], MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		}
		else {
			pMenuSort->CheckMenuItem(items[i], MF_BYCOMMAND | MF_UNCHECKED);
			pMenuSort->EnableMenuItem(items[i], MF_BYCOMMAND | MF_ENABLED);
		}
	}
}

void CListViewDemo::OnSelectedSort(UINT id)
{
	CMenu* pMenu = GetMenu()->GetSubMenu(0);
	CMenu* pMenuSort = GetMenu()->GetSubMenu(0)->GetSubMenu(2);
	CMenu* pMenuSortByID = pMenuSort->GetSubMenu(0);
	CString strMsg;
	UINT nState = pMenuSortByID->GetMenuState(ID_SORT_ID_DESC, MF_BYCOMMAND);
	switch (id) {
	case ID_SORT_ID_ASC:
	{
		m_bCompareWithStr = TRUE;
		m_nCompareColumn = 0;
		m_bReverseOrder = FALSE;
		m_lstStuInfo.SortItemsEx(SortCompareFunc, reinterpret_cast<LPARAM>(&m_lstStuInfo));
		::check_item(pMenu, ID_SORT_ID_ASC);
		break;
	}
	case ID_SORT_ID_DESC:
	{
		m_bCompareWithStr = TRUE;
		m_nCompareColumn = 0;
		m_bReverseOrder = TRUE;
		m_lstStuInfo.SortItemsEx(SortCompareFunc, reinterpret_cast<LPARAM>(&m_lstStuInfo));
		::check_item(pMenu, ID_SORT_ID_DESC);
		break;
	}
	case ID_SORT_VERBAL:
	{
		m_bCompareWithStr = FALSE;
		m_nCompareColumn = 3;
		m_bReverseOrder = FALSE;
		m_lstStuInfo.SortItemsEx(SortCompareFunc, reinterpret_cast<LPARAM>(&m_lstStuInfo));
		::check_item(pMenu, ID_SORT_VERBAL);
		break;
	}

	case ID_SORT_MATH:
	{
		m_bCompareWithStr = FALSE;
		m_nCompareColumn = 4;
		m_bReverseOrder = FALSE;
		m_lstStuInfo.SortItemsEx(SortCompareFunc, reinterpret_cast<LPARAM>(&m_lstStuInfo));
		::check_item(pMenu, ID_SORT_MATH);
		break;
	}

	case ID_SORT_ENGLISH:
	{
		m_bCompareWithStr = FALSE;
		m_nCompareColumn = 5;
		m_bReverseOrder = FALSE;
		m_lstStuInfo.SortItemsEx(SortCompareFunc, reinterpret_cast<LPARAM>(&m_lstStuInfo));
		::check_item(pMenu, ID_SORT_ENGLISH);
		break;
	}

	default:
		m_bCompareWithStr = FALSE;
		::check_item(pMenu, ID_SORT_RANDOM);
	}
}


void CListViewDemo::OnBnClickedBtnShowChart()
{
	CMenu* menuSort = GetMenu()->GetSubMenu(0)->GetSubMenu(2);
	CString strMsg;
	menuSort->GetMenuStringW(ID_SORT_ID_ASC, strMsg, MF_BYCOMMAND);
	menuSort->CheckMenuRadioItem(ID_SORT_ID_ASC, ID_SORT_RANDOM, ID_SORT_ID_ASC, MF_BYCOMMAND | MF_CHECKED);
	menuSort->SetDefaultItem(0, TRUE);
	MessageBox(strMsg);
}


afx_msg LRESULT CListViewDemo::OnSearchStuId(WPARAM wParam, LPARAM lParam)
{
	return 0;
}
