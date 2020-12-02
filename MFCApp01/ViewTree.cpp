#include "pch.h"
#include "framework.h"
#include "ViewTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CViewTree

CViewTree::CViewTree() noexcept {}

CViewTree::~CViewTree() {}

BEGIN_MESSAGE_MAP(CViewTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CViewTree::OnNMDbLClk)
END_MESSAGE_MAP()

// CViewTree message handlers

BOOL CViewTree::OnNotify(WPARAM wParam, LONG_PTR lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = reinterpret_cast<NMHDR*>(lParam);
	ASSERT(pNMHDR != nullptr);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != nullptr) {
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}

void CViewTree::OnNMDbLClk(NMHDR* pNMHDR, LRESULT* pResult)
{
	HTREEITEM hTreeItem = GetSelectedItem();
	CString strT1 = GetItemText(hTreeItem);
	AfxMessageBox(_T("You selected: ") + strT1);
	*pResult = 0;
}
