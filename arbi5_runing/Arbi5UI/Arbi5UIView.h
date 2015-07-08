// Arbi5UIView.h : interface of the CMyArbi5UIView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_Arbi5UIVIEW_H__FC71AC71_F7F4_4B3F_B387_22BB3E190B65__INCLUDED_)
#define AFX_Arbi5UIVIEW_H__FC71AC71_F7F4_4B3F_B387_22BB3E190B65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "MsgSheet.h"

class CMyArbi5UIView : public CView
{
protected: // create from serialization only
	CMyArbi5UIView();
	DECLARE_DYNCREATE(CMyArbi5UIView)

// Attributes
public:
	CMyArbi5UIDoc* GetDocument();

// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyArbi5UIView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// Implementation
public:
//    CMsgSheet m_sheet; 
public:

	virtual ~CMyArbi5UIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMyArbi5UIView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:

	BOOL m_bLButtonDown;
	HCURSOR m_hCross;
	CPoint m_ptOld, m_ptStart;
};

#ifndef _DEBUG  // debug version in Arbi5UIView.cpp
inline CMyArbi5UIDoc* CMyArbi5UIView::GetDocument()
   { return (CMyArbi5UIDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Arbi5UIVIEW_H__FC71AC71_F7F4_4B3F_B387_22BB3E190B65__INCLUDED_)
