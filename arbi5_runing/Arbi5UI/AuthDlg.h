#if !defined(AFX_AUTHDLG_H__525876EC_5B52_4603_9B0C_8E53AB87266A__INCLUDED_)
#define AFX_AUTHDLG_H__525876EC_5B52_4603_9B0C_8E53AB87266A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AuthDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAuthDlg dialog

class CAuthDlg : public CDialog
{
// Construction
public:
	CAuthDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAuthDlg)
	enum { IDD = IDD_DIALOG_AUTH };
	CEdit	m_conLocalCode;
	CString	m_strAuth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAuthDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL EnCodeID(CString strID, CString &strEncode);
	
	void CreateKey() ;
	// Generated message map functions
	//{{AFX_MSG(CAuthDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTHDLG_H__525876EC_5B52_4603_9B0C_8E53AB87266A__INCLUDED_)
