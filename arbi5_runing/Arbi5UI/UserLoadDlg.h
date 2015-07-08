#if !defined(AFX_USERLOADDLG_H__9C526FE9_39F1_40BB_9B7A_6D69419BC44A__INCLUDED_)
#define AFX_USERLOADDLG_H__9C526FE9_39F1_40BB_9B7A_6D69419BC44A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserLoadDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUserLoadDlg dialog

class CUserLoadDlg : public CDialog
{
// Construction
public:
	CUserLoadDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUserLoadDlg)
	enum { IDD = IDD_DIALOG_USER_LOAD };
	CString	m_struser;
	CString	m_strpwd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserLoadDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	BOOL Validation(); 
	// Generated message map functions
	//{{AFX_MSG(CUserLoadDlg)
	afx_msg void OnButtonLoad();
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERLOADDLG_H__9C526FE9_39F1_40BB_9B7A_6D69419BC44A__INCLUDED_)
