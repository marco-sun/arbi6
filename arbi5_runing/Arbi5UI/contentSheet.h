#if !defined(AFX_CONTENTSHEET_H__5C34ED29_75C0_44DA_BFAD_C9376A662FCD__INCLUDED_)
#define AFX_CONTENTSHEET_H__5C34ED29_75C0_44DA_BFAD_C9376A662FCD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// contentSheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// contentSheet


class contentSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(contentSheet)

// Construction
public:
	contentSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	contentSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
//	CListPage m_listpage;
//	reportPage m_report;
//	scriptPage m_script;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(contentSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~contentSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(contentSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTENTSHEET_H__5C34ED29_75C0_44DA_BFAD_C9376A662FCD__INCLUDED_)
