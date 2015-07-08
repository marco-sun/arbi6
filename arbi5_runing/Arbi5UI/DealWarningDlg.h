#if !defined(AFX_DEALWARNINGDLG_H__6572204D_E714_4F8A_996A_EE69B951BC8A__INCLUDED_)
#define AFX_DEALWARNINGDLG_H__6572204D_E714_4F8A_996A_EE69B951BC8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DealWarningDlg.h : header file
//
#include "..\Strategy.h"
/////////////////////////////////////////////////////////////////////////////
// CDealWarningDlg dialog

class CDealWarningDlg : public CDialog
{
// Construction
public:
	CDealWarningDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDealWarningDlg)
	enum { IDD = IDD_DIALOG_DEAL_WARNING };
	CString	m_strWarning;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDealWarningDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDealWarningDlg)
	afx_msg void OnProsessFirstOk();
	afx_msg void OnAddSecondOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEALWARNINGDLG_H__6572204D_E714_4F8A_996A_EE69B951BC8A__INCLUDED_)
