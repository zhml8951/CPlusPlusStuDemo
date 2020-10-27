
// MFCApp01View.cpp : implementation of the CApp01View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MFCApp01.h"
#endif

#include "MFCApp01Doc.h"
#include "MFCApp01View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CApp01View

IMPLEMENT_DYNCREATE(CApp01View, CView)

BEGIN_MESSAGE_MAP(CApp01View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CApp01View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CApp01View construction/destruction

CApp01View::CApp01View() noexcept
{
	// TODO: add construction code here

}

CApp01View::~CApp01View()
{
}

BOOL CApp01View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CApp01View drawing

void CApp01View::OnDraw(CDC* /*pDC*/)
{
	CApp01Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CApp01View printing


void CApp01View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CApp01View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CApp01View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CApp01View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CApp01View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CApp01View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CApp01View diagnostics

#ifdef _DEBUG
void CApp01View::AssertValid() const
{
	CView::AssertValid();
}

void CApp01View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CApp01Doc* CApp01View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CApp01Doc)));
	return (CApp01Doc*)m_pDocument;
}
#endif //_DEBUG


// CApp01View message handlers
