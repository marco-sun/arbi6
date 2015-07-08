#pragma once


// CDataCurve 对话框
#include "Oscillogram.h"

class CDataCurve : public CDialog
{
	DECLARE_DYNAMIC(CDataCurve)

public:
	CDataCurve(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDataCurve();

	COscillogram m_osc;
// 对话框数据
	enum { IDD = IDD_DIALOG_DATA_CURVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
