// ArbiReportDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Arbi6.h"
#include "ArbiReportDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "./arbi/Strategy.h"
#include "./arbi/DealHistory.h"
#include "DealWarningDlg.h"
#include <process.h>
#include "DealerHistoryLog.h"
#include "LogStore.h"
// CArbiReportDlg 对话框


IMPLEMENT_DYNCREATE(CArbiReportDlg, CDialogEx)
CArbiReportDlg::CArbiReportDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CArbiReportDlg::IDD, pParent)
{
	AfxInitRichEdit();
	m_runflag = TRUE;
}

CArbiReportDlg::~CArbiReportDlg()
{
	m_runflag = FALSE;
}

void CArbiReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	m_editLog= (CEdit*)GetDlgItem(IDC_EDIT_LOG);
}


BEGIN_MESSAGE_MAP(CArbiReportDlg, CDialogEx)
	ON_WM_SIZE()
	ON_EN_CHANGE(IDC_EDIT_LOG, &CArbiReportDlg::OnEnChangeEditLog)
END_MESSAGE_MAP()


// CArbiReportDlg 消息处理程序
void CArbiReportDlg::OnSize(UINT nType, int cx, int cy) 
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
void ArbiReport( void* pParams )
{ 
    CArbiReportDlg *pArbiLogDlg = (CArbiReportDlg *) pParams;
	if (pArbiLogDlg == NULL) {
		TRACE_LOG(" err: pArbiReportDlg is NULL");
		return;
	}
/*	int remberSize = 0;
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
	*/
	_endthread();
}
BOOL CArbiReportDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CMainFrame * pMainFrame = NULL;
	pMainFrame = ((CMainFrame*)::AfxGetMainWnd());
	pMainFrame->m_pArbiReportDlg = this;
	// TODO: Add extra initialization here
	m_strLog = "";
	CEdit * peditLog = (CEdit*)GetDlgItem(IDC_EDIT_LOG);
	peditLog->SetWindowText(m_strLog);
	_beginthread(ArbiReport, 0, (void *)this);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CArbiReportDlg::OnEnChangeEditLog()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
