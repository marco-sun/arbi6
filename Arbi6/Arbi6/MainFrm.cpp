
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include <process.h>
#include "Arbi6.h"

#include "MainFrm.h"

//#include "ExchangeRateInfoDlg.h"
#include "Loglist.h"
//#include "OperManageDlg.h"
#include "UserLoadDlg.h"
//#include "DialogSugar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMainFrame * gpMainFrame;


IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

	BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
		//{{AFX_MSG_MAP(CMainFrame)
		ON_WM_CREATE()
		ON_COMMAND(ID_SHOWBOTTOMBAR, OnShowBottomBar)
		ON_UPDATE_COMMAND_UI(ID_SHOWBOTTOMBAR, OnUpdateShowBottomBar)
		ON_COMMAND(ID_SHOWLEFTBAR, OnShowLeftBar)
		ON_UPDATE_COMMAND_UI(ID_SHOWLEFTBAR, OnUpdateShowLeftBar)
		ON_COMMAND(ID_BUTTON_FIND, OnButtonFind)
		ON_COMMAND(ID_BUTTON_FILTER, OnButtonFilter)
		ON_WM_PAINT()
		ON_COMMAND(ID_SHOWRIGHTBAR, OnShowrightbar)
		ON_UPDATE_COMMAND_UI(ID_SHOWRIGHTBAR, OnUpdateShowrightbar)
		ON_COMMAND(ID_EXCHANGE_RATE_CONFIG, OnExchangeRateConfig)
		ON_COMMAND(ID_ID_SYS_CONFIG, OnIdSysConfig)
		ON_COMMAND(ID_ID_USER_CONFIG, OnIdUserConfig)
		ON_COMMAND(ID_MENUITEM_LOCK, OnMenuitemLock)
		ON_COMMAND(ID_SUGAR, OnSugar)
		ON_COMMAND(ID_BOHAI, OnStopBoHai)
		ON_COMMAND(ID_BOHAI2, OnStartBoHai)
		ON_COMMAND(ID_BOHAI3, OnPauseBoHai)
		//}}AFX_MSG_MAP
	END_MESSAGE_MAP()

	static UINT indicators[] =
	{
		ID_SEPARATOR,           // status line indicator
		ID_INDICATOR_CAPS,
		ID_INDICATOR_NUM,
		ID_INDICATOR_SCRL,
	};

	/////////////////////////////////////////////////////////////////////////////
	// CMainFrame construction/destruction

	CMainFrame::CMainFrame()
	{
		// TODO: add member initialization code here
		//	m_ptransSerial = new CTransSerial;
		gpMainFrame = this;

	}

	CMainFrame::~CMainFrame()
	{

	}
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}


	//-----------����toolbar-------------
	int index = 0;


	int   nFullWidth=GetSystemMetrics(SM_CXSCREEN); 

	if(!(m_LeftCtrBar.Create(_T("������1"), this, CSize(nFullWidth,0),FALSE, 123,WS_VISIBLE)))
	{
		TRACE0("Failed to create m_LeftCtrBar\n");
		return -1;
	}

	if(!(m_BottomHexBar.Create(_T("������3"), this, CSize(0, 250), TRUE, 80,WS_VISIBLE)))
	{
		TRACE0("Failed to cretae m_BottomHexBar\n");
		return -1;
	}

	//---------------7.Create TabCtrl---------
	m_TabCtrl.Create(TCS_DOWN | WS_CHILD | WS_VISIBLE,
		CRect(0,0,100,100),
		&m_LeftCtrBar,
		125);
	m_TabDtail.Create(TCS_UP | WS_CHILD | WS_VISIBLE,
		CRect(0,0,100,100),
		&m_BottomHexBar,
		125);


	m_TabCtrl.AddPage(RUNTIME_CLASS(CIntentionPairDlg) ,  IDD_DIALOG_INTENTIONPAIR," Intention Pair ");//);
	m_TabDtail.AddPage(RUNTIME_CLASS(CStrategyInfoDlg) ,  IDD_DIALOG_STRATEGY_INFO," Strategy Info ");//);
 	m_TabDtail.AddPage(RUNTIME_CLASS(CArbiLogDlg) ,  IDD_DIALOG_ARBI_LOG," Operator Log ");//);
 	m_TabDtail.AddPage(RUNTIME_CLASS(CArbiReportDlg) ,  IDD_DIALOG_ARBI_REPORT," Report ");//);


	m_TabDtail.UpdateWindow();
	m_TabCtrl.UpdateWindow();

	//-------------------2.ͣ������-----------------------------------
	m_LeftCtrBar.SetBarStyle(m_LeftCtrBar.GetBarStyle() 
		| CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	m_BottomHexBar.SetBarStyle(m_BottomHexBar.GetBarStyle() 
		| CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	m_LeftCtrBar.EnableDocking(CBRS_ALIGN_ANY);

	m_BottomHexBar.EnableDocking(CBRS_ALIGN_ANY);

	EnableDocking(CBRS_ALIGN_ANY);

	DockControlBar(&m_LeftCtrBar, AFX_IDW_DOCKBAR_LEFT);      //ͣ�������

	CRect rect;
	RecalcLayout();
	m_LeftCtrBar.GetWindowRect(rect);
	DockControlBar(&m_BottomHexBar, AFX_IDW_DOCKBAR_BOTTOM, &rect);//ͣ���ڵײ�
	RecalcLayout();

	//---------9.ʹ������ͣ����ʾ��ͬ��ͷ��-----------------
	//-----ע��Ҫ��stafx.h�м��ϣ�#define	_SCB_REPLACE_MINIFRAME
#ifdef _SCB_REPLACE_MINIFRAME
	m_pFloatingFrameClass = RUNTIME_CLASS(CSCBMiniDockFrameWnd);
#endif //_SCB_REPLACE_MINIFRAME


	CLoglist Loglist;
	CString strlog;
	strlog.Format("����Ա��¼ϵͳ");

	Loglist.set_log(strlog.GetBuffer(strlog.GetLength()));

	return 0;
}



BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	//	 cs.style   &=   ~WS_MAXIMIZEBOX;//ȥ����󻯰�ť 

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


//-----------��ʾ������Ŀ�����-----------------
void CMainFrame::OnShowBottomBar() 
{
	//COMMAND������˵�������ʱ������������Ӧ����������Ӧ����
	ShowControlBar(&m_BottomCtrBar, !m_BottomCtrBar.IsVisible(), FALSE);	
}

void CMainFrame::OnUpdateShowBottomBar(CCmdUI* pCmdUI) 
{
	//UPDATE_COMMAND_UI��Ϣ�����ϲ�˵�����ʱ������
	pCmdUI->Enable(TRUE);   //Enable():�������Ʋ˵��͹�������ʹ�ܺͽ�ֹ״̬

	//SetCheck():�������ƹ������İ��º͵���״̬��
	pCmdUI->SetCheck(m_BottomCtrBar.IsVisible());	

}  //-----------��ʾ������Ŀ�����-----------------



//------��ʾ��߿�����---------------
void CMainFrame::OnShowLeftBar() 
{
	ShowControlBar(&m_LeftCtrBar, !m_LeftCtrBar.IsVisible(), FALSE);
}

void CMainFrame::OnUpdateShowLeftBar(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck(m_LeftCtrBar.IsVisible());

}   //------��ʾ��߿�����---------------

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::DockControlBarLeftOf(CToolBar* Bar, CToolBar* LeftOf)
{
	CRect rect;
	DWORD dw;
	UINT n;

	// get MFC to adjust the dimensions of all docked ToolBars
	// so that GetWindowRect will be accurate
	RecalcLayout(TRUE);

	LeftOf->GetWindowRect(&rect);
	rect.OffsetRect(1,0);
	dw=LeftOf->GetBarStyle();
	n = 0;
	n = (dw&CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
	n = (dw&CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = (dw&CBRS_ALIGN_LEFT && n==0) ? AFX_IDW_DOCKBAR_LEFT : n;
	n = (dw&CBRS_ALIGN_RIGHT && n==0) ? AFX_IDW_DOCKBAR_RIGHT : n;

	// When we take the default parameters on rect, DockControlBar will dock
	// each Toolbar on a seperate line. By calculating a rectangle, we
	// are simulating a Toolbar being dragged to that location and docked.
	DockControlBar(Bar,n,&rect);
}

void CMainFrame::OnButtonFind() 
{
	// TODO: Add your command handler code here
	int i = 0;

	//	CDatabaseAC datbase;
}

void CMainFrame::OnButtonFilter() 
{
	// TODO: Add your command handler code here

}

char * getNameFor(char * szOverseaInvestorID,char * szDomesticInvestorID) 
{
	if(strcmp(szOverseaInvestorID, "2656") == 0) return "������";
	if(strcmp(szOverseaInvestorID, "2653") == 0) return "������";
	if(strcmp(szOverseaInvestorID, "2662") == 0) return "��ΰȨ";
	if(strcmp(szOverseaInvestorID, "2668") == 0) return "��";
	if(strcmp(szOverseaInvestorID, "1090210001") == 0) return "��̩����";
	if(strcmp(szOverseaInvestorID, "2639") == 0) return "�齭����";
	if(strcmp(szOverseaInvestorID, "2675") == 0) return "�쾩��";
	if(strcmp(szOverseaInvestorID, "2676") == 0) return "�ν�������";
	if(strcmp(szOverseaInvestorID, "2671") == 0) return "����";
	if(strcmp(szDomesticInvestorID, "661555") == 0) return "������";
	if(strcmp(szDomesticInvestorID, "80200285") == 0) return "������˾";
	if(strcmp(szDomesticInvestorID, "600478") == 0) return "ʢ��";

	return "δ֪";
}

void CMainFrame::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	//��ȡAIB.ini�ļ��е��������ʺţ��Ա���ʾ�ڱ�����
	char szDomesticInvestorID[16];
	memset(szDomesticInvestorID, 0, sizeof(szDomesticInvestorID));
	char szOverseaInvestorID[16];
	memset(szOverseaInvestorID, 0, sizeof(szOverseaInvestorID));
	char InvestorID[16];
	memset(InvestorID, 0, sizeof(InvestorID));

	DWORD ret1 = GetPrivateProfileString("TRADE", "INVESTOR_ID", "N/A", szDomesticInvestorID, 16, ".\\AIB.ini");
	DWORD ret2 = GetPrivateProfileString("SP_TRADE", "SPInvestorId", "N/A", szOverseaInvestorID, 16, ".\\AIB.ini");
	DWORD ret3 = GetPrivateProfileString("SP_TRADE", "APPID", "N/A", InvestorID, 16, ".\\AIB.ini");

	char szTitle[128];
	memset(szTitle,0,sizeof(szTitle));
	char * name = getNameFor(szOverseaInvestorID,szDomesticInvestorID);

	sprintf_s(szTitle, 128,"%s,����:%s,����:%s", InvestorID, szOverseaInvestorID,szDomesticInvestorID);
	SetWindowText(szTitle);


	// Do not call CFrameWnd::OnPaint() for painting messages
}

void CMainFrame::OnShowrightbar() 
{
	// TODO: Add your command handler code here
	ShowControlBar(&m_BottomHexBar, !m_BottomHexBar.IsVisible(), FALSE);
}

void CMainFrame::OnUpdateShowrightbar(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);   //Enable():�������Ʋ˵��͹�������ʹ�ܺͽ�ֹ״̬

	//SetCheck():�������ƹ������İ��º͵���״̬��
	pCmdUI->SetCheck(m_BottomHexBar.IsVisible());
}

void CMainFrame::GetCurTime(char* stime)
{
	SYSTEMTIME t;
	GetLocalTime( &t );
	sprintf_s(stime, 10, "%02d%02d%02d%ld", t.wHour, t.wMinute, t.wSecond,t.wMilliseconds);
}


void CMainFrame::OnExchangeRateConfig() 
{
	// TODO: Add your command handler code here
// 	CExchangeRateInfoDlg *pDlg=new CExchangeRateInfoDlg();
// 	pDlg->Create(IDD_DIALOG_EXCHANGE_RATE);
// 	pDlg->ShowWindow(SW_SHOW);
// 	pDlg->CenterWindow();//���ھ�����ʾ

}

void CMainFrame::OnIdSysConfig() 
{
	// TODO: Add your command handler code here

}

void CMainFrame::OnIdUserConfig() 
{
	// TODO: Add your command handler code here
// 	COperManageDlg operManageDlg;
// 	operManageDlg.DoModal();
}

void CMainFrame::OnMenuitemLock() 
{
	// TODO: Add your command handler code here
	CUserLoadDlg userLoadDlg;
	userLoadDlg.DoModal();
}

void CMainFrame::OnSugar() 
{

}

void CMainFrame::OnStopBoHai() 
{

}

void CMainFrame::OnStartBoHai()
{

}

void CMainFrame::OnPauseBoHai()
{

}
