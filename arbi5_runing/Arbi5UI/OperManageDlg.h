#if !defined(AFX_OPERMANAGEDLG_H__0D56A750_3945_4932_AF4C_4CEE8FB89FB1__INCLUDED_)
#define AFX_OPERMANAGEDLG_H__0D56A750_3945_4932_AF4C_4CEE8FB89FB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OperManageDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COperManageDlg dialog

class COperManageDlg : public CDialog
{
// Construction
public:
	COperManageDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COperManageDlg)
	enum { IDD = IDD_DIALOG_USER_MANAGE };
	CString	m_strUser;
	CString	m_strOldPwd;
	CString	m_strNewPwd;
	CString	m_strConfirmPwd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COperManageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COperManageDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPERMANAGEDLG_H__0D56A750_3945_4932_AF4C_4CEE8FB89FB1__INCLUDED_)
