#pragma once


// CDataCurve �Ի���
#include "Oscillogram.h"

class CDataCurve : public CDialog
{
	DECLARE_DYNAMIC(CDataCurve)

public:
	CDataCurve(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDataCurve();

	COscillogram m_osc;
// �Ի�������
	enum { IDD = IDD_DIALOG_DATA_CURVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
