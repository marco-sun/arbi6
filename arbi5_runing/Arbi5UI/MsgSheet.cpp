// MsgSheet.cpp : implementation file
//

#include "stdafx.h"
#include "Arbi5UI.h"
#include "MsgSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMsgSheet

IMPLEMENT_DYNAMIC(CMsgSheet, CPropertySheet)

CMsgSheet::CMsgSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CMsgSheet::CMsgSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
//	AddPage( &m_listpage );
}

CMsgSheet::~CMsgSheet()
{
}


BEGIN_MESSAGE_MAP(CMsgSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CMsgSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgSheet message handlers
