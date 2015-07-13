// DealWarningDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Arbi6.h"
#include "DealWarningDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDealWarningDlg dialog


CDealWarningDlg::CDealWarningDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDealWarningDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDealWarningDlg)
	m_strWarning = _T("");
	//}}AFX_DATA_INIT
}


void CDealWarningDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDealWarningDlg)
	DDX_Text(pDX, IDC_STATIC_WARNING, m_strWarning);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDealWarningDlg, CDialog)
	//{{AFX_MSG_MAP(CDealWarningDlg)
	ON_BN_CLICKED(ID_PROSESS_FIRST_OK, OnProsessFirstOk)
	ON_BN_CLICKED(ID_ADD_SECOND_OK, OnAddSecondOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDealWarningDlg message handlers

void CDealWarningDlg::OnProsessFirstOk() 
{
	// TODO: Add your control notification handler code here
		
	

	CDialog::OnOK();
}

void CDealWarningDlg::OnAddSecondOk() 
{
	// TODO: Add your control notification handler code here
	

	CDialog::OnCancel();
}
