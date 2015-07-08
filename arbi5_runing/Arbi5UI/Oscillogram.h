// Oscillogram.h: interface for the COscillogram class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OSCILLOGRAM_H__725E8D1B_773D_4A0B_909E_B82E72991695__INCLUDED_)
#define AFX_OSCILLOGRAM_H__725E8D1B_773D_4A0B_909E_B82E72991695__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PointValue.h"
#include "CurveLink.h"
#include "TitleTips.h"

class COscillogram : public CWnd  
{
private:
	static VOID CALLBACK LineDDAProc(int X, int Y, LPARAM lpData );

public:
	//��ȡ������ɫ
	COLORREF GetCurveColor(int nIndex);
	//��ɫȡ��
	COLORREF ConverseColor(COLORREF nColor);
	COLORREF SetCurveColor(int nIndex, COLORREF nColor);
	//����X����ʱ���ʽ��ʾ,��ҪSetXVal��һ��ʹ��
	BOOL SetXTime(CTime &beginTime, CTime &endTime, BOOL showTime = TRUE);
	void SetTitleTip(BOOL show);
	void DrawMouseLine(CDC * dc,CPoint &point);
	BOOL SetWindowAlpha(CWnd &wnd, int Alpha);
	CString GetCurveName(int nIndex);
	COLORREF SetBkColor(COLORREF nColor);
	void DeleteCurve(int nIndex);
	Curve * GetCurve(int nIndex);
	int GetCurveCount();
	void AddCurve(COLORREF nColor, LPCTSTR nName,int lineWidth);
	void AddCurveValue(int nIndex, float val,BOOL state);//, CString &nTime);
	void SetOscillogramRect(CDC *dc);
	COLORREF SetCaptionColor(COLORREF nColor);
	COLORREF SetXYTextColor(COLORREF nColor);
	COLORREF SetGridColor(COLORREF nColor);
	void SetCaption(CString Caption);
	BOOL SetXVal(LPCTSTR xText, float minVal, float maxVal, int Count, int showCount);
	BOOL SetYVal(LPCTSTR yText, float minVal, float maxVal, int Count, int showCount);
	void DotLine(CDC *dc,int x, int y, int x1, int y1, COLORREF pixCol, int Span = 10);
	void DotCurveLine(CDC *dc,int x, int y, int x1, int y1, COLORREF pixCol, int Span =10 );
	//��ӡ������������VIEW.OnPrint()����
	void DrawPrint(CDC *dc,CRect *dRect);
	void DrawGrid(CDC *dc);
	void DrawCaption(CDC *dc);
	void DrawOscillogram(CDC *dc);
	void DrawCurve(CDC *dc, Curve * curve);
	void _DrawGrid(CDC* dc);
	BOOL Create(CWnd* lParent,const RECT& rect ,
		DWORD dwStyle = WS_CHILD/* | WS_BORDER */| WS_TABSTOP | WS_VISIBLE);
	COscillogram();
	virtual ~COscillogram();
private:
	CString m_Number;
	CString m_Init;
	//����ʱ�����
	CTime m_PreTime;
	//X����ʱ��ģʽ��ʾ����
	CTime m_beginTime;
	CTime m_endTime;
	BOOL  m_showTime;
	//��ʾ��ʾ��Ϣ��������
	BOOL m_showTitle;
	//��������ƶ�������
	CPoint m_bPt;
	//��ʾ��Ϣ��������
	CTitleTips m_TitleTip;
	//������������
	CCurveLink m_CurevLink;
	//����ͼ������ɫ ������ɫ
	COLORREF m_GridColor;
	COLORREF m_xyTextColor;
	COLORREF m_CaptionColor;
	COLORREF m_bkColor;
	//����ͼ�������
	CRect m_GridRect;
	//�������־���
	CRect m_CaptionRect;
	//��������  X������  Y������
	CString m_Caption , m_xText , m_yText ;
	//X�ᵥԪ���� Y�ᵥԪ����
	//int m_xSegment , m_ySegment;
	int m_xCount , m_yCount;
	//X�ᵥԪ��� Y�ᵥԪ���
	float m_xSpan , m_ySpan;
	//X����ֵ
	float m_xMaxVal,m_xMinVal;
	//Y����ֵ
	float m_yMaxVal,m_yMinVal;
	//��ʾ��������
	int m_yShowCount,m_xShowCount;


protected:
	// Generated message map functions
	//{{AFX_MSG(CFW_OscillogramDlg)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#endif // !defined(AFX_OSCILLOGRAM_H__725E8D1B_773D_4A0B_909E_B82E72991695__INCLUDED_)
