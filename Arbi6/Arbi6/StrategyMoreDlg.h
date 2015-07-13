#pragma once


// CStrategyMoreDlg 对话框

#include "Info.h"
class CStrategyMoreDlg : public CDialog
{
// Construction
public:
	CStrategyMoreDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStrategyMoreDlg();
	int m_buttonstate;  //  1 add 2 modify 3 del
	int m_state;		//  1 start 2 stop 

	UINT	m_nBSOversea;
	UINT	m_nBSDomestic;

	int m_bb1;
	int m_bb2;
	
	int m_nOP;//Strategy start or stop

	TStrategyUI m_tStrategyUI[50];
	int m_count;
	int m_bRadio;
// Dialog Data
	//{{AFX_DATA(CStrategyMoreDlg)
	enum { IDD = IDD_DIALOG_STRATEGY_MORE };
	CComboBox	m_DomesticType;
	CComboBox	m_Overseatype;
	CComboBox	m_BCompare;
	CComboBox	m_BSDomestic;
	CComboBox	m_BSOversea;
	CString	m_strName;
	UINT m_nMaxCount;
	double m_dStep;
	CString	m_strBSOversea;
	CString	m_strOverseaType;
	CString	m_strBSDomestic;
	CString	m_strDomesticType;
	CString	m_strCompare;
	CString	m_strContractDomestic;
	CString	m_strContractOversea;
	double	m_dDiff1;
	UINT	m_nVolumeDomestic;
	UINT	m_nVolumeOversea;

	double	m_dDiff2;

	UINT	m_maxPostions;
	//}}AFX_DATA
    

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStrategyMoreDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:


	void SetStrategy(); 
	void checkCondition();
	// Generated message map functions
	//{{AFX_MSG(CStrategyMoreDlg)
	afx_msg void OnButtonAdd();
	virtual BOOL OnInitDialog();

	afx_msg void OnCheckEditAndStart();
	afx_msg void OnSelchangeComOverseas();
	afx_msg void OnSelchangeComDomestic();
	afx_msg void OnSelchangeComOverseasType();
	afx_msg void OnSelchangeComDomesticType();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComboCompare1();

	afx_msg void OnBnClickedCheckFish1();

	CButton m_checkFish1;
	CButton m_checkNoPing;

	// 合约1加档
	UINT m_nAddOversea;
	// 合约2加档
	UINT m_nAddDomestic;
	afx_msg void OnEnChangeEditAddOversea();
	afx_msg void OnBnClickedCheckPing();
	afx_msg void OnEnChangeEditDiff2();
};
