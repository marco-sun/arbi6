#if !defined(AFX_ARBILOGDLG_H__233146D3_5423_4037_9DF5_556AE550A8BC__INCLUDED_)
#define AFX_ARBILOGDLG_H__233146D3_5423_4037_9DF5_556AE550A8BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ArbiLogDlg.h : header file
//
#pragma warning(disable : 4786)
//#include "..\StrategyListener.h"
#include <list>
#include <process.h>
#include "afxmt.h"
using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CArbiLogDlg dialog

class CArbiLogDlg : public CDialog
{
// Construction
public:
	CArbiLogDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CArbiLogDlg();
	DECLARE_DYNCREATE(CArbiLogDlg)
// Dialog Data
	//{{AFX_DATA(CArbiLogDlg)
	enum { IDD = IDD_DIALOG_ARBI_LOG };
	CEdit	*m_editLog;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CArbiLogDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
//	void onChange(Strategy * strategy);
	CString m_strLog;
	BOOL m_runflag;
	CCriticalSection   m_mutex; 
///	list<Strategy * > m_listStrategy;
protected:

	// Generated message map functions
	//{{AFX_MSG(CArbiLogDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ARBILOGDLG_H__233146D3_5423_4037_9DF5_556AE550A8BC__INCLUDED_)
