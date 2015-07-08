// Arbi5UIDoc.h : interface of the CMyArbi5UIDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_Arbi5UIDOC_H__FB576A6F_0B01_479D_BECF_10A75C5D06C1__INCLUDED_)
#define AFX_Arbi5UIDOC_H__FB576A6F_0B01_479D_BECF_10A75C5D06C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMyArbi5UIDoc : public CDocument
{
protected: // create from serialization only
	CMyArbi5UIDoc();
	DECLARE_DYNCREATE(CMyArbi5UIDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyArbi5UIDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyArbi5UIDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMyArbi5UIDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Arbi5UIDOC_H__FB576A6F_0B01_479D_BECF_10A75C5D06C1__INCLUDED_)
