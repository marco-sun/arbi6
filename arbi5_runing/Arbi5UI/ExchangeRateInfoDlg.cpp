// ExchangeRateInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Arbi5UI.h"
#include "ExchangeRateInfoDlg.h"
#include "..\Rate.h"
#include "..\LogStore.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExchangeRateInfoDlg dialog


CExchangeRateInfoDlg::CExchangeRateInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExchangeRateInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExchangeRateInfoDlg)
//	m_dDollarExRate = Rate::americanDolar;
//	m_dYenExRate = Rate::yan;
	//}}AFX_DATA_INIT
}


void CExchangeRateInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExchangeRateInfoDlg)
	DDX_Text(pDX, IDC_EDIT_DOLLAR_EXR, m_dDollarExRate);
	DDX_Text(pDX, IDC_EDIT_YEN_EXR, m_dYenExRate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExchangeRateInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CExchangeRateInfoDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExchangeRateInfoDlg message handlers

void CExchangeRateInfoDlg::OnOK() 
{
	UpdateData(TRUE);

//	Rate::americanDolar = m_dDollarExRate;
//	Rate::yan = m_dYenExRate;
//	Rate::save();
	CDialog::OnOK();
}
