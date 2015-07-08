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
	//获取曲线颜色
	COLORREF GetCurveColor(int nIndex);
	//颜色取反
	COLORREF ConverseColor(COLORREF nColor);
	COLORREF SetCurveColor(int nIndex, COLORREF nColor);
	//设置X轴用时间格式表示,需要SetXVal后一起使用
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
	//打印函数，可以在VIEW.OnPrint()调用
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
	//保留时间对象
	CTime m_PreTime;
	//X轴以时间模式显示参数
	CTime m_beginTime;
	CTime m_endTime;
	BOOL  m_showTime;
	//显示提示信息浮动窗体
	BOOL m_showTitle;
	//跟随鼠标移动的坐标
	CPoint m_bPt;
	//提示信息浮动窗体
	CTitleTips m_TitleTip;
	//波形曲线链表
	CCurveLink m_CurevLink;
	//波形图网格颜色 字体颜色
	COLORREF m_GridColor;
	COLORREF m_xyTextColor;
	COLORREF m_CaptionColor;
	COLORREF m_bkColor;
	//波形图网格矩形
	CRect m_GridRect;
	//标题文字矩形
	CRect m_CaptionRect;
	//标题文字  X轴文字  Y轴文字
	CString m_Caption , m_xText , m_yText ;
	//X轴单元总数 Y轴单元总数
	//int m_xSegment , m_ySegment;
	int m_xCount , m_yCount;
	//X轴单元宽度 Y轴单元宽度
	float m_xSpan , m_ySpan;
	//X轴数值
	float m_xMaxVal,m_xMinVal;
	//Y轴数值
	float m_yMaxVal,m_yMinVal;
	//显示网格间隔数
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
