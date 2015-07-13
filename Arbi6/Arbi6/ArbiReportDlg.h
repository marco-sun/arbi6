#pragma once
#include "afxmt.h"
using namespace std;

// CArbiReportDlg 对话框

class CArbiReportDlg : public CDialogEx
{
	DECLARE_DYNCREATE(CArbiReportDlg)
public:
	CArbiReportDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CArbiReportDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_ARBI_REPORT };
	CEdit	*m_editLog;
public:
//	void onChange(Strategy * strategy);
	CString m_strLog;
	BOOL m_runflag;
	CCriticalSection   m_mutex; 
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditLog();
};
