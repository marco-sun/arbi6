// TitleTip.h: interface for the CTitleTips class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TITLETIP_H__945CA392_5AE0_46B9_A530_6B822B4AE9A0__INCLUDED_)
#define AFX_TITLETIP_H__945CA392_5AE0_46B9_A530_6B822B4AE9A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTitleTips : public CWnd  
{
public:
	CFont font;
	CRect	m_pRect;
	CStringArray	m_strArray;
	CDWordArray		m_colArray;
public:
	void SetStrArray(CStringArray &strArray, CDWordArray &colArray);
	void SetParentRect(CRect &rect);
	void SetPos(CPoint *point);
	BOOL Create(const RECT& rect );
	CTitleTips();
	virtual ~CTitleTips();

protected:
	// Generated message map functions
	//{{AFX_MSG(CFW_OscillogramDlg)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#endif // !defined(AFX_TITLETIP_H__945CA392_5AE0_46B9_A530_6B822B4AE9A0__INCLUDED_)
