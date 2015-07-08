// Arbi5UIView.cpp : implementation of the CMyArbi5UIView class
//

#include "stdafx.h"
#include "Arbi5UI.h"

#include "Arbi5UIDoc.h"
#include "Arbi5UIView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyArbi5UIView

IMPLEMENT_DYNCREATE(CMyArbi5UIView, CView)

BEGIN_MESSAGE_MAP(CMyArbi5UIView, CView)
	//{{AFX_MSG_MAP(CMyArbi5UIView)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	// Standard printing commands
//	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyArbi5UIView construction/destruction

CMyArbi5UIView::CMyArbi5UIView()// : m_sheet("")
{
	// TODO: add construction code here
	m_bLButtonDown = FALSE;
	m_hCross = AfxGetApp()->LoadStandardCursor(IDC_CROSS);
}

CMyArbi5UIView::~CMyArbi5UIView()
{
}

BOOL CMyArbi5UIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMyArbi5UIView drawing

void CMyArbi5UIView::OnDraw(CDC* pDC)
{
	CMyArbi5UIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}


/////////////////////////////////////////////////////////////////////////////
// CMyArbi5UIView diagnostics

#ifdef _DEBUG
void CMyArbi5UIView::AssertValid() const
{
	CView::AssertValid();
}

void CMyArbi5UIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMyArbi5UIDoc* CMyArbi5UIView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyArbi5UIDoc)));
	return (CMyArbi5UIDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyArbi5UIView message handlers
/*
///--左键按下响应-------
void CMyArbi5UIView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bLButtonDown = TRUE;
	m_ptStart = point;
	m_ptOld   = point;
	SetCapture();     //把以后的鼠标输入消息都送到本窗口
	
	CRect rect;
	GetClientRect(&rect);  //得到当前客户区的大小和位置，并保存在rect结构体中
	ClientToScreen(&rect); //把其参数点或矩形坐标从当前窗口转换到屏幕坐标
	ClipCursor(&rect);     //限定鼠标范围,鼠标不能到这个限定区域之外

	CView::OnLButtonDown(nFlags, point);
}


//----鼠标移到消息------------------------------
void CMyArbi5UIView::OnMouseMove(UINT nFlags, CPoint point) 
{
	SetCursor(m_hCross);
	if(m_bLButtonDown)
	{
		CClientDC dc(this);  //用于在客户区画图和显示文本
		dc.SetROP2(R2_NOT);  //用来设置当前绘图方式

		dc.MoveTo(m_ptStart);
		dc.LineTo(m_ptOld);

		dc.MoveTo(m_ptStart);
		dc.LineTo(point);

		m_ptOld = point;
	}

	
	CView::OnMouseMove(nFlags, point);
}

//-----左键抬起响应---------------
void CMyArbi5UIView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(m_bLButtonDown)
	{
		m_bLButtonDown = FALSE;
		ReleaseCapture();
		ClipCursor(NULL);

		CClientDC dc(this);
		dc.SetROP2(R2_NOT);
		dc.MoveTo(m_ptStart);
		dc.LineTo(m_ptOld);

		dc.SetROP2(R2_COPYPEN);
		dc.MoveTo(m_ptStart);
		dc.LineTo(point);
	}

	
	CView::OnLButtonUp(nFlags, point);
}
*/

void CMyArbi5UIView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
/*		
	m_sheet.Create(this, WS_CHILD | WS_VISIBLE, 0);
	//使选项卡的按钮在下面
	::SetWindowLong(m_sheet.GetTabControl()->m_hWnd, GWL_STYLE,
		::GetWindowLong(m_sheet.GetTabControl()->m_hWnd, GWL_STYLE) | TCS_BOTTOM);
	if(m_sheet.m_hWnd)
		m_sheet.ShowWindow(SW_MAXIMIZE);//显示选项卡

	((CMainFrame*)::AfxGetMainWnd())->m_psheet = &m_sheet;
*/
}

void CMyArbi5UIView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
/*	this->ShowWindow(SW_MAXIMIZE);
	CWnd *psheet = this->GetWindow(GW_CHILD);//m_messageSheet是resultview的唯一顶层子窗口
	if(psheet != NULL){
		CRect rect;
		this->GetWindowRect(&rect);
		psheet->ScreenToClient(&rect);
		psheet->SetWindowPos(NULL, 0, 0, rect.Width() , rect.Height() ,
							SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
		CWnd *ptab = this->m_sheet.GetTabControl();
		if(ptab != NULL){
			ptab->ScreenToClient(&rect);
			ptab->SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(),
								SWP_NOZORDER | SWP_NOACTIVATE);

			//让PropertySheet自动调整page的大小
			this->m_sheet.SetActivePage(this->m_sheet.GetActiveIndex());
		}
	}
	*/
	// TODO: Add your message handler code here
	
}
