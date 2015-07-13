
// Arbi6View.cpp : CArbi6View 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Arbi6.h"
#endif

#include "Arbi6Doc.h"
#include "Arbi6View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CArbi6View

IMPLEMENT_DYNCREATE(CArbi6View, CView)

BEGIN_MESSAGE_MAP(CArbi6View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CArbi6View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CArbi6View 构造/析构

CArbi6View::CArbi6View()
{
	// TODO: 在此处添加构造代码

}

CArbi6View::~CArbi6View()
{
}

BOOL CArbi6View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CArbi6View 绘制

void CArbi6View::OnDraw(CDC* /*pDC*/)
{
	CArbi6Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CArbi6View 打印


void CArbi6View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CArbi6View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CArbi6View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CArbi6View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CArbi6View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CArbi6View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CArbi6View 诊断

#ifdef _DEBUG
void CArbi6View::AssertValid() const
{
	CView::AssertValid();
}

void CArbi6View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CArbi6Doc* CArbi6View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CArbi6Doc)));
	return (CArbi6Doc*)m_pDocument;
}
#endif //_DEBUG


// CArbi6View 消息处理程序
