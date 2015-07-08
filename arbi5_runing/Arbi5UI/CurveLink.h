// CurveLink.h: interface for the CCurveLink class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CURVELINK_H__09326DCE_1D8E_4AF0_9A4C_A40FDF4CAC43__INCLUDED_)
#define AFX_CURVELINK_H__09326DCE_1D8E_4AF0_9A4C_A40FDF4CAC43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PointValue.h"

struct Curve
{
	int Index;
	int LineWidth;
	CString name;
	COLORREF lColor;
	CPointValue ptVal;
	Curve * next;
	Curve * previous;
};

class CCurveLink  
{
private:
	Curve * head;
	Curve * end;
	int m_Count;
public:
	void Delete(int nIndex);
	void Release();
	int GetCount();
	Curve * Find(int nIndex);
	int Add(int ptCount,LPCTSTR name,COLORREF nColor, int lineWidth);
	CCurveLink();
	virtual ~CCurveLink();

};

#endif // !defined(AFX_CURVELINK_H__09326DCE_1D8E_4AF0_9A4C_A40FDF4CAC43__INCLUDED_)
