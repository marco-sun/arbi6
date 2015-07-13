// ArbiLogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Arbi6.h"
#include "ArbiLogDlg.h"
#include "MainFrm.h"
#include "./arbi/Strategy.h"
#include "./arbi/DealHistory.h"
#include "DealWarningDlg.h"
#include <process.h>
#include "DealerHistoryLog.h"
#include "LogStore.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//StrategyListener *g_StrategyListener;

/////////////////////////////////////////////////////////////////////////////
// CArbiLogDlg dialog

IMPLEMENT_DYNCREATE(CArbiLogDlg, CDialog)
CArbiLogDlg::CArbiLogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CArbiLogDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CArbiLogDlg)
		// NOTE: the ClassWizard will add member initialization here
	
	//}}AFX_DATA_INIT
	AfxInitRichEdit();
	m_runflag = TRUE;
}
CArbiLogDlg::~CArbiLogDlg()
{
	m_runflag = FALSE;
}

void CArbiLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CArbiLogDlg)
//	DDX_Control(pDX, IDC_EDIT_LOG, m_editLog);
	m_editLog= (CEdit*)GetDlgItem(IDC_EDIT_LOG);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CArbiLogDlg, CDialog)
	//{{AFX_MSG_MAP(CArbiLogDlg)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CArbiLogDlg message handlers

void CArbiLogDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CDialog::OnSize(nType, cx, cy);
	if (GetSafeHwnd())
	{
		CEdit * peditLog = (CEdit*)GetDlgItem(IDC_EDIT_LOG);
		if (peditLog->GetSafeHwnd())
		{
			CRect rect(0,0,cx,cy);
			peditLog->MoveWindow(&rect);
		}
	}
}
void LogChange( void* pParams )
{ 
    CArbiLogDlg *pArbiLogDlg = (CArbiLogDlg *) pParams;
	if (pArbiLogDlg == NULL) {
		TRACE_LOG(" err: pArbiLogDlg is NULL");
		return;
	}
	int remberSize = 0;
	DealHistory * dealHistory = DealHistory::getInstance();
	while(pArbiLogDlg->m_runflag == TRUE)
	{
		pArbiLogDlg->m_strLog = "";
		list<string>  listDealLog = dealHistory->listAll();
		list<string> ::iterator it ;
		if (remberSize != listDealLog.size())
		{
			for(it = listDealLog.begin(); it != listDealLog.end(); ++it)// list
			{
				pArbiLogDlg->m_strLog = "\r\n" + pArbiLogDlg->m_strLog;
				pArbiLogDlg->m_strLog = (* it).c_str() + pArbiLogDlg->m_strLog; //add to head.
				CEdit * peditLog = (CEdit*)pArbiLogDlg->GetDlgItem(IDC_EDIT_LOG);
				peditLog->SetWindowText(pArbiLogDlg->m_strLog);
			}
			remberSize = listDealLog.size();
		}

        Sleep(50); 
	}
	_endthread();
}

BOOL CArbiLogDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CMainFrame * pMainFrame = NULL;
	pMainFrame = ((CMainFrame*)::AfxGetMainWnd());
	pMainFrame->m_pArbiLogDlg = this;
	// TODO: Add extra initialization here
	m_strLog = "";
//  DealerHistoryLog dealerHistoryLog;
//	m_strLog = dealerHistoryLog.GetDealerLog();
	CEdit * peditLog = (CEdit*)GetDlgItem(IDC_EDIT_LOG);
	peditLog->SetWindowText(m_strLog);
	_beginthread(LogChange, 0, (void *)this);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

