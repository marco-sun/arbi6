#if !defined(AFX_INTENTIONPAIRDLG_H__2DFC8BFE_AE86_4FBE_A5E7_E782CB5DC9A1__INCLUDED_)
#define AFX_INTENTIONPAIRDLG_H__2DFC8BFE_AE86_4FBE_A5E7_E782CB5DC9A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IntentionPairDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIntentionPairDlg dialog
//#include "..\IntentionPairListener.h"
#include "ListCtrlCl.h"
#include "Info.h"
#include "afxmt.h"
class ArbiRobot;
class CIntentionPairDlg : public CDialog
{
// Construction
public:
	CIntentionPairDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIntentionPairDlg();
	DECLARE_DYNCREATE(CIntentionPairDlg)
// Dialog Data
	//{{AFX_DATA(CIntentionPairDlg)
	enum { IDD = IDD_DIALOG_INTENTIONPAIR };
	CListCtrlCl	m_listIntentionPair;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIntentionPairDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
//	IPStateMap m_mapIPState;
	BOOL m_runflag ;
	BOOL m_sortflag;// 
	CCriticalSection   m_mutex; 
	list<IntentionPair * > m_listIntenPair;

	LISTIPUI m_listIPUI;
	BOOL UpdateIntentionPairUI(IntentionPair* pIp);
	TIPInfo* FindIPstatemap(string strategyName);
	BOOL PreTranslateMessage(MSG* pMsg); 
	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIntentionPairDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRclickListIntentionPair(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAddItem(); 
	afx_msg void OnAddStrategy();
	afx_msg void OnAddMoreStrategy();
	afx_msg void OnClickListIntentionPair(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfoListIntentionPair(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnColumnclickListSp(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	int	m_nRSelItem;
	void SetMenuState(int nItem, CMenu*   pPopup);
	void InitIntentionPair(list<IntentionPair *> listIntenPair);
	void InitIntentionPairTwoLegStrategy();
	void UpdateInitIntentionPair(list<IntentionPair *> listIntenPair);
	void SetTwoLegStrategy(DayTradingStrategy* pTS);
//for Business ENV
public:
	TIPInfo* FindIPInfomap(string strategyName);
//	void OnInfoChange(IntentionPair* intentionPair);
private:
	CRITICAL_SECTION csui;
public:
	afx_msg void OnDataCurve();
	afx_msg void OnDblclkListIntentionPair(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListIntentionPair(NMHDR *pNMHDR, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INTENTIONPAIRDLG_H__2DFC8BFE_AE86_4FBE_A5E7_E782CB5DC9A1__INCLUDED_)
