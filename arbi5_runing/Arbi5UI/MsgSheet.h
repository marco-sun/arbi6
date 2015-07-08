#if !defined(AFX_MSGSHEET_H__9DA7C2DA_8953_4CE1_846A_6FBCB947C839__INCLUDED_)
#define AFX_MSGSHEET_H__9DA7C2DA_8953_4CE1_846A_6FBCB947C839__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MsgSheet.h : header file
//
//#include "ListPage.h"
/////////////////////////////////////////////////////////////////////////////
// CMsgSheet

class CMsgSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CMsgSheet)

// Construction
public:
	CMsgSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CMsgSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
//	CListPage m_listpage;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMsgSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMsgSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMsgSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGSHEET_H__9DA7C2DA_8953_4CE1_846A_6FBCB947C839__INCLUDED_)
