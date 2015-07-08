// TitleTip.cpp: implementation of the CTitleTips class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TitleTips.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTitleTips::CTitleTips()
{
	font.CreateFont(15, 0,0,0,FW_HEAVY, 0,0,0,
		DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");
}

CTitleTips::~CTitleTips()
{

}

BEGIN_MESSAGE_MAP(CTitleTips, CWnd)
	//{{AFX_MSG_MAP(CTitleTips)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTitleTips message handlers

BOOL CTitleTips::Create(const RECT& rect)
{
	return CWnd::CreateEx(
					    WS_EX_TOPMOST | WS_EX_WINDOWEDGE | WS_EX_TOOLWINDOW,
						AfxRegisterWndClass(CS_NOCLOSE,0,0,0),
						_T(""),  WS_POPUP , rect, NULL, NULL);
}

void CTitleTips::OnMouseMove(UINT nFlags, CPoint point) 
{
	ClientToScreen(&point);
	SetPos(&point);
	CWnd::OnMouseMove(nFlags, point);
}

void CTitleTips::SetPos(CPoint *point)
{
	int x,y;
	CRect rc;   

	GetClientRect(&rc);

	if(m_pRect.right - point->x > rc.Width())
		x = point->x;
	else
		x = point->x - rc.Width();

	if(point->y - m_pRect.top > rc.Height())
		y = point->y - rc.Height() ;
	else
	{	
		if(x == point->x)
		{
			y = point->y +20;
			x = x + 10;
		}
		else
			y = point->y ;
	}
	SetWindowPos(NULL,x,y,rc.Width(),rc.Height(),SWP_NOACTIVATE  | SWP_SHOWWINDOW);
}

void CTitleTips::SetParentRect(CRect &rect)
{
	m_pRect = rect;
}

void CTitleTips::SetStrArray(CStringArray &strArray, CDWordArray &colArray)
{
	CSize sz = 0;
	CRect	rc;   
	CPen	pen;
	int   x = 0;
	int   y = 0;
	CClientDC dc(this);
	CBrush	brush(RGB(248,214,252));
	pen.CreatePen(PS_SOLID,0,RGB(255,255,255));

	m_strArray.RemoveAll();
	m_strArray.Append(strArray);
	m_colArray.RemoveAll();
	m_colArray.Append(colArray);
	
	for(int i=0;i<m_strArray.GetSize();i++)
	{
		sz = dc.GetTextExtent(m_strArray.GetAt(i));
		if(sz.cx > x) x = sz.cx;
		y += sz.cy;
	}
	SetWindowPos(NULL,0,0,x+50,y+10,SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

	GetClientRect(&rc);
	dc.SetBkMode(TRANSPARENT);
	dc.SelectObject(&pen);
	dc.SelectObject(&brush);
	dc.SelectObject(&font);
	dc.Rectangle(&rc);	
	int i;
	for(i=0;i<m_strArray.GetSize();i++)
	{
		dc.SetTextColor(m_colArray.GetAt(i));
		sz = dc.GetTextExtent(m_strArray.GetAt(i));
		dc.TextOut(5,5+i*sz.cy,m_strArray.GetAt(i));
	}
	
}

int CTitleTips::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	return MA_NOACTIVATE;
}
