// Arbi5UI.h : main header file for the Arbi5UI application
//

#if !defined(AFX_Arbi5UI_H__6AF63619_51B5_4857_BFEA_8D04DA8FE21A__INCLUDED_)
#define AFX_Arbi5UI_H__6AF63619_51B5_4857_BFEA_8D04DA8FE21A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMyArbi5UIApp:
// See Arbi5UI.cpp for the implementation of this class
//

class CMyArbi5UIApp : public CWinApp
{
public:
	CMyArbi5UIApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyArbi5UIApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMyArbi5UIApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Arbi5UI_H__6AF63619_51B5_4857_BFEA_8D04DA8FE21A__INCLUDED_)
