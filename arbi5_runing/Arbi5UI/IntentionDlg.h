#if !defined(AFX_INTENTIONDLG_H__5070551F_28E0_4AD2_A18B_AC57781400EF__INCLUDED_)
#define AFX_INTENTIONDLG_H__5070551F_28E0_4AD2_A18B_AC57781400EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IntentionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIntentionDlg dialog

class CIntentionDlg : public CDialog
{
// Construction
public:
	CIntentionDlg(CWnd* pParent = NULL);   // standard constructor
	int m_state;// 1 add 2 del 
// Dialog Data
	//{{AFX_DATA(CIntentionDlg)
	enum { IDD = IDD_DIALOG_INTENTION };
	CString	m_strName;
	CString	m_strFirstContract;
	CString	m_strSecondContract;
	UINT	m_firstVol;
	UINT	m_secondVol;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIntentionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIntentionDlg)
	afx_msg void OnButtonAddIntention();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INTENTIONDLG_H__5070551F_28E0_4AD2_A18B_AC57781400EF__INCLUDED_)
