// DataCurve.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Arbi5UI.h"
#include "DataCurve.h"
#include "afxdialogex.h"

// CDataCurve �Ի���

IMPLEMENT_DYNAMIC(CDataCurve, CDialog)

CDataCurve::CDataCurve(CWnd* pParent /*=NULL*/)
	: CDialog(CDataCurve::IDD, pParent)
{

}

CDataCurve::~CDataCurve()
{
}

void CDataCurve::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDataCurve, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDataCurve ��Ϣ�������


BOOL CDataCurve::OnInitDialog()
{
	CDialog::OnInitDialog();

	RECT rect;
	rect.top = 100;
	rect.left = 100;
	rect.right = 100;
	rect.bottom = 100 ;
	if(!m_osc)
		m_osc.Create(this,rect,
		WS_CHILD | WS_VISIBLE);
	else
	{
		MessageBox("�����Ѿ�������");
		return FALSE;
	}

	m_osc.SetCaption("intention name");

	m_osc.SetYVal("�۲�",-200,200,11,11);
	m_osc.SetXVal("ʱ��",0,0,60,13);		
	m_osc.SetGridColor(RGB(255,0,255));
	m_osc.SetXYTextColor(RGB(200,200,200));
	m_osc.SetCaptionColor(RGB(0,0,128));
	
	CRect cRrect;
	this->GetClientRect(cRrect);
	m_osc.MoveWindow(0,0,cRrect.Width(),cRrect.Height());

	int i = 0;
	COLORREF col = RGB(128,128,128);
	COLORREF col1 = RGB(128,128,128);
	for(i = 1;i < 3; i++)
	{
		switch(i)
		{
		case 1:
			col = RGB(255,128,0);
			m_osc.AddCurve(col,"��۲�",6);
			break;
		case 2:
			col1 = RGB(0,255,0);
			m_osc.AddCurve(col1,"�׼۲�",6);
			break;
		}
	}
	int j = 0;
	for(j = 0;j < 16; j++)
	{
		float a=(float)(10+rand()%30);
		for(i=0;i<m_osc.GetCurveCount();i++)
		{
			float x=(float)(i*40 + 0.001);
			m_osc.AddCurveValue(i,a+x,TRUE);
		}
	}
	this->SetTimer(1,1000,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
}


void CDataCurve::OnTimer(UINT_PTR nIDEvent)
{
	float a=(float)(10+rand()%30);

	int i;
	for(i=0;i<m_osc.GetCurveCount();i++)
	{
		float x=(float)(i*40 + 0.001);

		m_osc.AddCurveValue(i,a+x,TRUE);
	}

	CTime b;
	b = CTime::GetCurrentTime();
	CTimeSpan c(0,0,60,0);//������������λ�Ƿ���,X��
	CTime e;
	e = b - c;
	m_osc.SetXTime(e,b,TRUE);
	CDialog::OnTimer(nIDEvent);
}
