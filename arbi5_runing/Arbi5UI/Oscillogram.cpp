// Oscillogram.cpp: implementation of the COscillogram class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Oscillogram.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define CS_MARGIN 140
#define CS_LMARGIN 51

COscillogram::COscillogram()
{
	m_PreTime = 0;

	m_showTime = FALSE;
	m_beginTime = 0;
	m_endTime = 0;

	m_showTitle = TRUE;
	m_bPt = -1;

	m_GridRect = 0;
	m_CaptionRect = 0;

	m_xSpan = 0;	
	m_ySpan = 0;

	m_GridColor = RGB(255,255,255);
	m_xyTextColor = RGB(255,255,255);
	m_CaptionColor = RGB(0,255,64);
	m_bkColor = RGB(0,0,0);
	

	m_yShowCount = 2;
	m_xShowCount = 2;

	m_xCount = 2;	
	m_yCount = 2;
	
	m_yMaxVal = 1.00;
	m_yMinVal = 0.00;

	m_xMaxVal = 1.00;
	m_xMinVal = 0.00;
	
	SetYVal(m_yText,m_yMinVal,m_yMaxVal,m_yCount,m_yShowCount);
	SetXVal(m_xText,m_xMinVal,m_xMaxVal,m_xCount,m_xShowCount);	
}

BEGIN_MESSAGE_MAP(COscillogram, CWnd)
	//{{AFX_MSG_MAP(COscillogram)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COscillogram message handlers

COscillogram::~COscillogram()
{

}

BOOL COscillogram::Create(CWnd* lParent,const RECT& rect ,DWORD dwStyle )
{

	CString wndcls=AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,0,
		(HBRUSH)GetStockObject(WHITE_BRUSH),LoadIcon(NULL,IDI_APPLICATION));
	
	m_TitleTip.Create(CRect(0,0,0,0));
	SetWindowAlpha(m_TitleTip,180);
	BOOL re = CWnd::Create(wndcls,"波形图",dwStyle,rect,lParent,600);
	ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	return re;
}

void COscillogram::SetCaption(CString Caption)
{
	m_Caption = Caption;
}

void COscillogram::OnPaint() 
{
	CPaintDC dc(this);
	dc.SetBkMode(TRANSPARENT);

	DrawOscillogram(&dc);
}
	static BOOL paint = TRUE;

void COscillogram::DrawOscillogram(CDC *dc)
{
	int oldmode	= dc->SetMapMode(MM_LOMETRIC);
	SetOscillogramRect(dc);

	DrawCaption(dc);
	DrawGrid(dc);
	for(int i=0;i<GetCurveCount();i++)
		DrawCurve(dc,GetCurve(i));

	dc->SetMapMode(oldmode);
}

void COscillogram::SetOscillogramRect(CDC *dc)
{
	//获得客户区大小
	CRect rect;
	GetClientRect(rect);
	dc->DPtoLP(&rect);
	//获得网格矩形
	m_GridRect.top		 = 0 - CS_MARGIN;
	m_GridRect.left		 = CS_MARGIN;
	m_GridRect.bottom	 = rect.bottom + CS_MARGIN;
	m_GridRect.right	 = rect.right - CS_MARGIN;
	//计算标题矩形
	m_CaptionRect		 = m_GridRect;
	m_CaptionRect.top	 = 0;
	m_CaptionRect.left	 = 0;
	m_CaptionRect.bottom = m_GridRect.top ;
	m_CaptionRect.right	 = rect.right;
	//计算步长宽度
	m_xSpan = (float)m_GridRect.Width() / (m_xCount-1);
	m_ySpan = (float)m_GridRect.Height() / (m_yCount-1);

}

void COscillogram::DrawCaption(CDC *dc)
{
	CFont	m_cFont;
	CFont	*m_oldFont;

	m_cFont.CreateFont(55, 0,0,0,FW_HEAVY, 0,0,0,
		DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");

	dc->SetTextColor(m_CaptionColor);
	m_oldFont = dc->SelectObject(&m_cFont);
	dc->DrawText(m_Caption,m_CaptionRect,DT_CENTER | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);
	dc->SelectObject(m_oldFont);
}

VOID CALLBACK COscillogram::LineDDAProc(int X, int Y, LPARAM lpData )
{
	CDC * dc = (CDC *)lpData;
	dc->SetPixel(X,Y,RGB(255,255,0));
	return ;
}

void COscillogram::DotCurveLine(CDC *dc,int x, int y, int x1, int y1, COLORREF pixCol, int Span)
{
	//回调函数效率低 暂时不考虑
	::LineDDA(x,y,x1,y1,LineDDAProc,(LPARAM)dc);
}

void COscillogram::DotLine(CDC *dc,int x, int y, int x1, int y1, COLORREF pixCol, int Span)
{//画直虚线
	if(y == y1)
		for(int i=x;i<=x1;i+=Span)
			dc->SetPixel(i,y,pixCol);
	if(x == x1)
		for(int j=y;j>=y1;j-=Span)
			dc->SetPixel(x,j,pixCol);
}

void COscillogram::_DrawGrid(CDC *dc)
{
	CPen pen(PS_SOLID,0,RGB(55,55,55));
	dc->SelectObject(&pen);

	for(int i=0;i<m_xCount;i++)
	{
		dc->MoveTo(m_GridRect.left + (int)(m_xSpan*i) , m_GridRect.top);
		dc->LineTo(m_GridRect.left + (int)(m_xSpan*i) , m_GridRect.bottom);
	}
	
	for(int j=0;j<m_yCount;j++)
	{
		dc->MoveTo(m_GridRect.left , m_GridRect.top + (int)(m_ySpan*j)),
		dc->LineTo(m_GridRect.right,0-CS_MARGIN + (int)(m_ySpan*j));
	}
}

void COscillogram::DrawGrid(CDC *dc)
{
	//_DrawGrid(dc);

	CString tempStr;
	CSize	tempSzie;
	int		tempPos;
	CFont	m_cFont, *oldFont;
	CPen	pen(PS_SOLID,6,m_GridColor);

	m_cFont.CreatePointFont(260,"宋体");//"Arial");
	oldFont = dc->SelectObject(&m_cFont);
	dc->SelectObject(&pen);
	dc->SetTextColor(m_xyTextColor);

	float m_xTemp = (float)m_GridRect.Width() / (m_xShowCount-1);
	float m_yTemp = (float)m_GridRect.Height() / (m_yShowCount-1);
	CTimeSpan m_xTimeSpan = 0;

	if(m_showTime)
	{
		CTimeSpan sc    = m_endTime - m_beginTime;

		double secCount = (sc.GetDays()*86400) + (sc.GetHours()*3600) + 
						  (sc.GetMinutes()*60) + sc.GetSeconds();
		secCount = secCount / (m_xShowCount-1);

		int day    = (int)secCount/86400;	//天
		secCount  -= day*86400;
		int hour   = (int)secCount/3600;	//小时
		secCount  -= hour*3600;
		int minute = (int)secCount/60;		//分钟
		secCount  -= minute*60;
		int second = (int)secCount;			//秒

		m_xTimeSpan = CTimeSpan(day,hour,minute,second);
	}
	
	for(int i=0;i<m_xShowCount;i++)
	{
		tempPos =   m_GridRect.left + (int)(m_xTemp*i);
		
		DotLine(dc, tempPos, 
			        m_GridRect.top ,
				    tempPos , 
				    m_GridRect.bottom ,
				    m_GridColor );
		
		if(!m_showTime)
			tempStr.Format("%.2f", 
					(m_xMaxVal-m_xMinVal)/(m_xShowCount-1) 
					* i	+ m_xMinVal );
		else
		{
			CTime cnTime = m_beginTime;;

			for(int j=0;j<i;j++)
				cnTime += m_xTimeSpan;

			tempStr = cnTime.Format("%H:%M:%S");//%Y/%m/%d 
		}

		tempSzie =  dc->GetTextExtent(tempStr);
		dc->TextOut(tempPos - tempSzie.cx/2 , 
					m_GridRect.bottom - 15 ,
					tempStr );
	}

	for(int j=0;j<m_yShowCount;j++)
	{
		tempPos = m_GridRect.top + (int)(m_yTemp*j);

		DotLine(dc, m_GridRect.left , 
			        tempPos ,
				    m_GridRect.right ,
				    tempPos ,
				    m_GridColor );
	
		tempStr.Format("%.2f",
					(m_yMaxVal-m_yMinVal)/(m_yShowCount-1)
					* (m_yShowCount-1-j) + m_yMinVal );

		tempSzie =  dc->GetTextExtent(tempStr);

		dc->TextOut(m_GridRect.left - tempSzie.cx-15 , 
					tempPos + tempSzie.cy/2 ,
					tempStr );
	}
	
	//X轴文本
	tempSzie = dc->GetTextExtent(m_xText);
	dc->TextOut(m_GridRect.right + 15,m_GridRect.bottom + tempSzie.cy/2,m_xText);
	//Y轴文本
	tempSzie = dc->GetTextExtent(m_yText);
	dc->TextOut(m_GridRect.left - tempSzie.cx/2,m_GridRect.top + tempSzie.cy + 15,m_yText);

	//XY轴线
	dc->MoveTo(m_GridRect.left,m_GridRect.top);
	dc->LineTo(m_GridRect.left,m_GridRect.bottom);
	dc->MoveTo(m_GridRect.left,m_GridRect.bottom);
	dc->LineTo(m_GridRect.right,m_GridRect.bottom);
	//上面小三角
	int i;
	for(i=1;i<12;i++)
	{
		dc->MoveTo(m_GridRect.left-i-3,m_GridRect.top-i);
		dc->LineTo(m_GridRect.left+i-3,m_GridRect.top-i);
	}
	//上面小三角
	for(i=1;i<12;i++)
	{
		dc->MoveTo(m_GridRect.right-i,m_GridRect.bottom-i);
		dc->LineTo(m_GridRect.right-i,m_GridRect.bottom+i);
	}
	
	dc->SelectObject(oldFont);
}

COLORREF COscillogram::SetGridColor(COLORREF nColor)
{
	COLORREF rCol;
	rCol = m_GridColor;
	m_GridColor = nColor;
	this->Invalidate();
	return rCol;
}

COLORREF COscillogram::SetXYTextColor(COLORREF nColor)
{
	COLORREF rCol;
	rCol = m_xyTextColor;
	m_xyTextColor = nColor;
	this->Invalidate();
	return rCol;
}

COLORREF COscillogram::SetCaptionColor(COLORREF nColor)
{
	COLORREF rCol;
	rCol = m_CaptionColor;
	m_CaptionColor = nColor;
	this->Invalidate();
	return rCol;
}

COLORREF COscillogram::SetBkColor(COLORREF nColor)
{
	COLORREF rCol;
	rCol = m_bkColor;
	m_bkColor = nColor;
	this->Invalidate();
	return rCol;
}

BOOL COscillogram::SetYVal(LPCTSTR yText ,float minVal, float maxVal, int Count, int showCount)
{
	if(Count<2 || showCount<2)
		return FALSE;

	m_yCount = Count;
	m_yShowCount = showCount;
	m_yMinVal = minVal;
	m_yMaxVal = maxVal;
	m_yText = yText;

	if(m_hWnd)
		Invalidate();
	
	return TRUE;
}

BOOL COscillogram::SetXVal(LPCTSTR xText ,float minVal, float maxVal, int Count, int showCount)
{
	if(Count<2 || showCount<2)
		return FALSE;

	m_xCount = Count;
	m_xShowCount = showCount;
	m_xMinVal = minVal;
	m_xMaxVal = maxVal;
	m_xText = xText;


	for(int i=0;i<GetCurveCount();i++)
		GetCurve(i)->ptVal.CreateLink(m_xCount);
	
	if(m_hWnd)
		Invalidate();

	return TRUE;
}

BOOL COscillogram::SetXTime(CTime &beginTime, CTime &endTime, BOOL showTime)
{	

	m_beginTime = beginTime;
	m_endTime = endTime;
	m_showTime = showTime;

	CRect rect;
	GetClientRect(rect);
	InvalidateRect(CRect(0,rect.bottom-CS_LMARGIN+4,rect.right,rect.bottom));

	return FALSE;
}

void COscillogram::DrawPrint(CDC *dc, CRect *dRect)
{
	int oldmode			 = dc->SetMapMode(MM_LOMETRIC);
	//获得网格矩形
	m_GridRect.top		 = dRect->top    - CS_MARGIN;
	m_GridRect.left		 = dRect->left   + CS_MARGIN;
	m_GridRect.bottom	 = dRect->bottom + CS_MARGIN;
	m_GridRect.right	 = dRect->right  - CS_MARGIN;
	//计算标题矩形
	m_CaptionRect.top	 = dRect->top;
	m_CaptionRect.left	 = dRect->left;
	m_CaptionRect.bottom = m_GridRect.top ;
	m_CaptionRect.right	 = dRect->right;
	//计算步长宽度

	m_xSpan = (float)m_GridRect.Width() / (m_xCount-1);
	m_ySpan = (float)m_GridRect.Height() / (m_yCount-1);
	//绘画工作
	DrawCaption(dc);
	DrawGrid(dc);
	for(int i=0;i<GetCurveCount();i++)
		DrawCurve(dc,GetCurve(i));

	dc->SetMapMode(oldmode);
}

void COscillogram::DrawCurve(CDC *dc, Curve * curve)
{
	float y;
	float sval;
	float numspan;
	CPoint pt=-1;
	CPoint Dpt=-1;
	BOOL previousPt = FALSE;
	CPointValue * pv = &curve->ptVal;
	CPen pen(PS_SOLID,curve->LineWidth,ConverseColor(curve->lColor));

	dc->SelectObject(&pen);
	dc->SetROP2(R2_NOTXORPEN);
	//dc->SelectObject(GetStockObject(NULL_PEN)) ;  
	//dc->SetROP2(R2_NOT);   

	numspan = (float)(m_yMaxVal-m_yMinVal)/(m_yCount-1);
	
	for(int i=0;i<m_xCount;i++)
	{
		sval  = (pv->GetValue() - m_yMinVal)/numspan;
		y	  = m_GridRect.bottom -  sval * m_ySpan;
		Dpt.x = (int)(m_xSpan*i) + m_GridRect.left;
		Dpt.y = (int)y;

		if(pt.x==-1)
		{
			previousPt = pv->GetStart();
			pt = Dpt;
		}
		else
		{
			//回调函数效率低 暂时不考虑
			//this->DotCurveLine(dc,pt.x,pt.y,Dpt.x,Dpt.y,m_lineColor);
			if(pv->GetStart() && previousPt)
			{
				dc->MoveTo(pt);
				dc->LineTo(Dpt);
				//dc->SetPixel(pt,curve->lColor);
			}
			previousPt = pv->GetStart();
		
			pt = Dpt;
		}
		pv->MoveNext();
	}
}

void COscillogram::OnMouseMove(UINT nFlags, CPoint point) 
{
	CStringArray valArray;
	CDWordArray colArray;
	CString     strVal;
	CRect		mRect;
	CClientDC	dc(this);
	float		length;		//鼠标位置绝对象素数
	float		gValue;
	int			oldMode;
	int			curCell;	//所在单元格
	CPen pen(PS_SOLID,0,RGB(0,0,0));
	BOOL		PtState = FALSE;

	//(整个函数过程的功能)计算所有线所在单元格的数值
	oldMode = dc.SetMapMode(MM_LOMETRIC);
	SetOscillogramRect(&dc);
	dc.SelectObject(&pen);
	dc.SetROP2(R2_NOTXORPEN);
	dc.DPtoLP(&point);

	//如果 鼠标不在波形图内 或者 没有曲线 不做处理返回
	if(!(point.x >= m_GridRect.left  && point.x <= m_GridRect.right+3
		&& point.y <= m_GridRect.top && point.y >= m_GridRect.bottom)
		|| GetCurveCount() < 1 || m_showTitle == FALSE)
	{
		if(m_bPt.x != -1)
		{
			DrawMouseLine(&dc,m_bPt);
			m_bPt =-1;
		}
		m_TitleTip.ShowWindow(SW_HIDE);
		return;
	}
	
	//绘画跟随鼠标的十字线
	if(m_bPt.x == -1)
	{
		m_bPt = point;
		DrawMouseLine(&dc,point);
	}
	else
	{	
		DrawMouseLine(&dc,m_bPt);
		m_bPt = point;
		DrawMouseLine(&dc,point);
	}

	//计算个单元格数值
	length  = (float)( point.x - m_GridRect.left );
	curCell = (int)( length / m_xSpan );
	
	if(!m_showTime)
	{
		float n1 = (m_xMaxVal - m_xMinVal)/(m_xCount-1);
		float n2 = m_xMinVal + curCell*n1;
		strVal.Format("%s: %.2f",m_xText,n2);
	}
	else
	{
		CTimeSpan m_xTimeSpan = 0;
		CTimeSpan sc    = m_endTime - m_beginTime;
		CTime	cnTime  = m_beginTime;
		
		double secCount = (sc.GetDays()*86400) + (sc.GetHours()*3600) + 
			(sc.GetMinutes()*60) + sc.GetSeconds();
		secCount = secCount / (m_xCount-1);
		
		int day    = (int)secCount/86400;	//天
		secCount  -= day*86400;
		int hour   = (int)secCount/3600;	//小时
		secCount  -= hour*3600;
		int minute = (int)secCount/60;		//分钟
		secCount  -= minute*60;
		int second = (int)secCount;			//秒
		m_xTimeSpan = CTimeSpan(day,hour,minute,second);

		for(int j=0;j<curCell;j++)
			cnTime += m_xTimeSpan;
		strVal.Format("%s: %s",m_xText,cnTime.Format("%Y/%m/%d  %H:%M:%S"));
	}
	colArray.Add(RGB(0,0,0));
	valArray.Add(strVal);

	for(int i=0;i<GetCurveCount();i++)
	{
		gValue = GetCurve(i)->ptVal.GetPointValue(curCell,PtState);
		
		if(PtState)
			strVal.Format("%s: %.2f",GetCurveName(i),gValue);
		else
			strVal.Format("%s: ",GetCurveName(i));
		colArray.Add(GetCurve(i)->lColor);
		valArray.Add(strVal);
	}

	//显示浮动窗体
	dc.LPtoDP(&point);
	dc.SetMapMode(oldMode);

	//窗口跟随鼠标位置移动
	GetClientRect(mRect);
	mRect.left   += CS_LMARGIN;
	mRect.top    += CS_LMARGIN;
	mRect.right  -= CS_LMARGIN;
	mRect.bottom -= CS_LMARGIN;
	ClientToScreen(&point);
	ClientToScreen(&mRect);

	m_TitleTip.SetParentRect(mRect);
	m_TitleTip.SetStrArray(valArray,colArray);
	m_TitleTip.SetPos(&point);
	ScreenToClient(&point);
	
	CWnd::OnMouseMove(nFlags, point);
}

void COscillogram::DrawMouseLine(CDC *dc, CPoint &point)
{
	dc->MoveTo(point.x,m_GridRect.top);
	dc->LineTo(point.x,m_GridRect.bottom);
	
	dc->MoveTo(m_GridRect.left,point.y);
	dc->LineTo(m_GridRect.right,point.y);
}

void COscillogram::OnDestroy() 
{
	m_CurevLink.Release();
	CWnd::OnDestroy();
}

void COscillogram::AddCurve(COLORREF nColor,LPCTSTR nName,int lineWidth)
{
	m_CurevLink.Add(m_xCount, nName, nColor, lineWidth);
}

int COscillogram::GetCurveCount()
{
	return m_CurevLink.GetCount();
}

Curve * COscillogram::GetCurve(int nIndex)
{
	return m_CurevLink.Find(nIndex);
}

void COscillogram::AddCurveValue(int nIndex ,float val,BOOL state)
{
	CPointValue * temp;

	if(GetCurve(nIndex))
		temp = &GetCurve(nIndex)->ptVal;
	else
		return ;
	
	CClientDC dc(this);

	dc.SetBkMode(TRANSPARENT);
	int oldMode	= dc.SetMapMode(MM_LOMETRIC);
	SetOscillogramRect(&dc);

	DrawCurve(&dc,GetCurve(nIndex));
	temp->SetValue(val,state);
	temp->MoveNext();
	DrawCurve(&dc,GetCurve(nIndex));

	dc.SetMapMode(oldMode);
}

void COscillogram::DeleteCurve(int nIndex)
{
	m_CurevLink.Delete(nIndex);
	
	if(m_hWnd && IsWindowVisible())
		Invalidate();
}

CString COscillogram::GetCurveName(int nIndex)
{
	if(!GetCurve(nIndex))
		return "";

	return GetCurve(nIndex)->name;
}

BOOL COscillogram::OnEraseBkgnd(CDC* pDC) 
{
	CRect rc;   
	CBrush brush;

	brush.CreateSolidBrush(m_bkColor);
	GetClientRect(&rc);
	
	pDC->FillRect(&rc, &brush);

	return TRUE;
	//return CWnd::OnEraseBkgnd(pDC);
}

BOOL COscillogram::SetWindowAlpha(CWnd &wnd, int Alpha)
{
	::SetWindowLong(wnd.m_hWnd,GWL_EXSTYLE,
		GetWindowLong(wnd.m_hWnd,GWL_EXSTYLE)^0x80000);
		
	HINSTANCE hInst = LoadLibrary("User32.DLL"); 
	
	if(hInst) 
	{ 
		typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD); 
		MYFUNC fun = NULL;
		//取得SetLayeredWindowAttributes函数指针 
		fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
		if(fun)
			int zero = fun(wnd.m_hWnd,0,Alpha,2); 
		DWORD word = GetLastError();
	}
	FreeLibrary(hInst); 

	return TRUE;
}

void COscillogram::SetTitleTip(BOOL show)
{
	m_showTitle = show;
}

COLORREF COscillogram::SetCurveColor(int nIndex, COLORREF nColor)
{
	COLORREF col = -1;

	if(GetCurve(nIndex))
	{
		col = GetCurve(nIndex)->lColor;
		GetCurve(nIndex)->lColor = nColor;
		this->Invalidate();
	}
	
	return col;
}

COLORREF COscillogram::ConverseColor(COLORREF nColor)
{
	int R = GetRValue(nColor);
	int G = GetGValue(nColor);
	int B = GetBValue(nColor);
	R = 255-R;
	G = 255-G;
	B = 255-B;
	return RGB(R,G,B);	
}

COLORREF COscillogram::GetCurveColor(int nIndex)
{
	COLORREF col = -1;

	if(GetCurve(nIndex))
		col = GetCurve(nIndex)->lColor;
	
	return col;	
}
