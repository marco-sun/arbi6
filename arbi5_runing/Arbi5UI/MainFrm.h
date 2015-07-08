// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__D2522EC9_7B75_4895_AFC8_00AA7AA5A1EF__INCLUDED_)
#define AFX_MAINFRM_H__D2522EC9_7B75_4895_AFC8_00AA7AA5A1EF__INCLUDED_

#include "scbarg.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "CoolTabCtrl.h"

//#include "ListPage.h"
#include "MsgSheet.h"
#include "IntentionPairDlg.h"
#include "ArbiLogDlg.h"
#include "ArbiReportDlg.h"
#include "StrategyInfoDlg.h"
#include "info.h"



#include <map>
using namespace std;
#pragma warning(disable : 4786)
#pragma warning(disable : 4146)

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
   	void DockControlBarLeftOf(CToolBar* Bar, CToolBar* LeftOf);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	CCoolBar  m_LeftCtrBar;    //左面的控制窗口
    CCoolBar  m_BottomCtrBar;  //最下面的控制窗口
	CCoolBar  m_BottomHexBar;  //最下面的控制窗口
    CEdit     m_BottomEdit;    //加在最下面的控制栏中的Edit

	CCoolTabCtrl m_TabCtrl;
    CCoolTabCtrl m_TabDtail;
	CCoolTabCtrl m_TabBottom;
	CMsgSheet *m_psheet;


	CIntentionPairDlg *m_pIntentionPairDlg;
	CArbiLogDlg *m_pArbiLogDlg;
	CArbiReportDlg *m_pArbiReportDlg;
	CStrategyInfoDlg *m_pStrategyInfoDlg;


//流程控制参数：
public:


//各类MAP:
public:
	IPInfoMap m_IPInfoMap;


	virtual ~CMainFrame();
	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
//	CToolBar    m_wndToolBar;

	CImageList	m_ilTB;

	int m_keyfalg;

	int m_iPicCount;

protected:

public:
	void GetCurTime(char* stime);
// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowBottomBar();
	afx_msg void OnUpdateShowBottomBar(CCmdUI* pCmdUI);
	afx_msg void OnShowLeftBar();
	afx_msg void OnUpdateShowLeftBar(CCmdUI* pCmdUI);
	afx_msg void OnButtonFind();
	afx_msg void OnButtonFilter();
	afx_msg void OnPaint();
	afx_msg void OnShowrightbar();
	afx_msg void OnUpdateShowrightbar(CCmdUI* pCmdUI);
	afx_msg void OnExchangeRateConfig();
	afx_msg void OnIdSysConfig();
	afx_msg void OnIdUserConfig();
	afx_msg void OnMenuitemLock();
	afx_msg void OnSugar();
	afx_msg void OnStopBoHai();
	afx_msg void OnStartBoHai();
	afx_msg void OnPauseBoHai();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__D2522EC9_7B75_4895_AFC8_00AA7AA5A1EF__INCLUDED_)
