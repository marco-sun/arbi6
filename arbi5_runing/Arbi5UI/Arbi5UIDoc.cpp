// Arbi5UIDoc.cpp : implementation of the CMyArbi5UIDoc class
//

#include "stdafx.h"
#include "Arbi5UI.h"

#include "Arbi5UIDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyArbi5UIDoc

IMPLEMENT_DYNCREATE(CMyArbi5UIDoc, CDocument)

BEGIN_MESSAGE_MAP(CMyArbi5UIDoc, CDocument)
	//{{AFX_MSG_MAP(CMyArbi5UIDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyArbi5UIDoc construction/destruction

CMyArbi5UIDoc::CMyArbi5UIDoc()
{
	// TODO: add one-time construction code here

}

CMyArbi5UIDoc::~CMyArbi5UIDoc()
{
}

BOOL CMyArbi5UIDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMyArbi5UIDoc serialization

void CMyArbi5UIDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMyArbi5UIDoc diagnostics

#ifdef _DEBUG
void CMyArbi5UIDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMyArbi5UIDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyArbi5UIDoc commands
