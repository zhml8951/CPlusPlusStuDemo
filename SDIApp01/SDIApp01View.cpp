
// SDIApp01View.cpp: CSDIApp01View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "SDIApp01.h"
#endif

#include "SDIApp01Doc.h"
#include "SDIApp01View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSDIApp01View

IMPLEMENT_DYNCREATE(CSDIApp01View, CView)

BEGIN_MESSAGE_MAP(CSDIApp01View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSDIApp01View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CSDIApp01View 构造/析构

CSDIApp01View::CSDIApp01View() noexcept
{
	// TODO: 在此处添加构造代码

}

CSDIApp01View::~CSDIApp01View()
{
}

BOOL CSDIApp01View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CSDIApp01View 绘图

void CSDIApp01View::OnDraw(CDC* /*pDC*/)
{
	CSDIApp01Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CSDIApp01View 打印


void CSDIApp01View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CSDIApp01View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CSDIApp01View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CSDIApp01View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CSDIApp01View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSDIApp01View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CSDIApp01View 诊断

#ifdef _DEBUG
void CSDIApp01View::AssertValid() const
{
	CView::AssertValid();
}

void CSDIApp01View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSDIApp01Doc* CSDIApp01View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSDIApp01Doc)));
	return (CSDIApp01Doc*)m_pDocument;
}
#endif //_DEBUG


// CSDIApp01View 消息处理程序
