#if !defined(AFX_STRATEGYDLG_H__C0723D20_802C_4749_8556_19FF130BCD8D__INCLUDED_)
#define AFX_STRATEGYDLG_H__C0723D20_802C_4749_8556_19FF130BCD8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StrategyDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStrategyDlg dialog
#include "Info.h"
class CStrategyDlg : public CDialog
{
// Construction
public:
	CStrategyDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStrategyDlg();
	int m_buttonstate;  //  1 add 2 modify 3 del
	int m_state;		//  1 start 2 stop 

	UINT	m_nBSOversea;
	UINT	m_nBSDomestic;

	int m_bb1;
	int m_bb2;
	
	int m_nOP;//Strategy start or stop

//	CString m_strStrategy1;
//	CString m_strStrategy2;

	TStrategyUI m_tStrategyUI;

	int m_bRadio;
// Dialog Data
	//{{AFX_DATA(CStrategyDlg)
	enum { IDD = IDD_DIALOG_STRATEGY };
	CComboBox	m_DomesticType2;
	CComboBox	m_DomesticType;
	CComboBox	m_Overseatype2;
	CComboBox	m_Overseatype;
	CComboBox	m_BCompare2;
	CComboBox	m_BCompare;
	CComboBox	m_BSOversea2;
	CComboBox	m_BSDomestic2;
	CComboBox	m_BSDomestic;
	CComboBox	m_BSOversea;
	CString	m_strName;
	CString	m_strBSOversea;
	CString	m_strOverseaType;
	CString	m_strOverseatype2;
	CString	m_strBSDomestic;
	CString	m_strDomesticType;
	CString	m_strDomesticType2;
	CString	m_strCompare;
	CString	m_strCompare2;
	UINT	m_nAddDomestic;
	UINT	m_nAddOversea;
	UINT	m_nAddDomestic2;
	UINT	m_nAddOversea2;
	CString	m_strContractDomestic;
	CString	m_strContractOversea;
	double	m_dDiff1;
	UINT	m_nVolumeDomestic;
	UINT	m_nVolumeDomestic2;
	UINT	m_nVolumeOversea;
	UINT	m_nVolumeOversea2;
	double	m_dDiff2;
	CString	m_strBSOversea2;
	CString	m_strBSDomestic2;
	UINT	m_maxPostions;
	//}}AFX_DATA
    

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStrategyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void SetUI();
	void SetCheck2(); 
	void checkCondition();
	// Generated message map functions
	//{{AFX_MSG(CStrategyDlg)
	afx_msg void OnButtonAdd();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheck1();
	afx_msg void OnCheck2();
	afx_msg void OnCheckEditAndStart();
	afx_msg void OnSelchangeComOverseas();
	afx_msg void OnSelchangeComDomestic();
	afx_msg void OnSelchangeComOverseasType();
	afx_msg void OnSelchangeComOverseasType2();
	afx_msg void OnSelchangeComOverseas2();
	afx_msg void OnSelchangeComDomesticType();
	afx_msg void OnSelchangeComDomesticType2();
	afx_msg void OnSelchangeComDomestic2();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComboCompare1();
	afx_msg void OnCbnSelchangeComboCompare2();
	afx_msg void OnBnClickedCheckFish1();
	afx_msg void OnBnClickedCheckFish2();
	CButton m_checkFish1;
	CButton m_checkFish2;
	afx_msg void OnBnClickedButtonTurnOver();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STRATEGYDLG_H__C0723D20_802C_4749_8556_19FF130BCD8D__INCLUDED_)
