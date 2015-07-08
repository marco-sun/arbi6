#if !defined(AFX_STRATEGYINFODLG_H__F2063C3E_9EA9_4C8B_9F7D_0FA59FE38288__INCLUDED_)
#define AFX_STRATEGYINFODLG_H__F2063C3E_9EA9_4C8B_9F7D_0FA59FE38288__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StrategyInfoDlg.h : header file
//
#include "ListCtrlCl.h"
#include "Info.h"
/////////////////////////////////////////////////////////////////////////////
// CStrategyInfoDlg dialog

class CStrategyInfoDlg : public CDialog
{
// Construction
public:
	CStrategyInfoDlg(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CStrategyInfoDlg)
// Dialog Data
	//{{AFX_DATA(CStrategyInfoDlg)
	enum { IDD = IDD_DIALOG_STRATEGY_INFO };
	CListCtrlCl	m_listStrategyInfo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStrategyInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	IPStateMap m_mapIPState;
	TIPState* FindIPstatemap(string strategyName);
	TStrategyInfo* FindSGstatemap(string szname);
	BOOL PreTranslateMessage(MSG* pMsg); 
protected:

//	void CloneStructStrategyUI(TStrategyUI *tOutStrategyUI, TStrategyUI tInStrategyUI);
	// Generated message map functions
	//{{AFX_MSG(CStrategyInfoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRclickListStrategyInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditStrategy(); 
	afx_msg void OnStopStrategy();
	afx_msg void OnStartStrategy();
	afx_msg void OnDelStrategy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	LISTSelectItem m_listSeletcItem;
	int	m_nRSelItem;
	void SetMenuState(int nItem, CMenu*   pPopup);
public:
	afx_msg void OnDblclkListStrategyInfo(NMHDR *pNMHDR, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STRATEGYINFODLG_H__F2063C3E_9EA9_4C8B_9F7D_0FA59FE38288__INCLUDED_)
