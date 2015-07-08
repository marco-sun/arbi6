// contentSheet.cpp : implementation file
//

#include "stdafx.h"
#include "contentSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// contentSheet

IMPLEMENT_DYNAMIC(contentSheet, CPropertySheet)

contentSheet::contentSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

contentSheet::contentSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
//	AddPage( &m_listpage );	// First page
//	AddPage( &m_script );	// First page
//	AddPage( &m_report );	// Second page
}

contentSheet::~contentSheet()
{
}


BEGIN_MESSAGE_MAP(contentSheet, CPropertySheet)
	//{{AFX_MSG_MAP(contentSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// contentSheet message handlers
