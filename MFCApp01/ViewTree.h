#pragma once

// CViewTree window

class CViewTree : public CTreeCtrl
{
	// Construction
public:
	CViewTree() noexcept;

	// Overrides
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	/*
	 * ON_COMMAND 菜单和工具栏项 处理消息的宏；
	 * ON_MESSAGE 处理自定义消息宏
	 * ON_NOTIFY  是控件向父窗口发送消息
	 */
	// Implementation
public:
	virtual ~CViewTree();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMDbLClk(NMHDR *pNMHDR, LRESULT *pResult);
};
