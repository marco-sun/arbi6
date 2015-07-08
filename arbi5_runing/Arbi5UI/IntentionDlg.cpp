// IntentionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Arbi5UI.h"
#include "IntentionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIntentionDlg dialog


CIntentionDlg::CIntentionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIntentionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIntentionDlg)
	m_strName = _T("");
	m_strFirstContract = _T("");
	m_strSecondContract = _T("");
	m_firstVol = 1;
	m_secondVol = 1;
	//}}AFX_DATA_INIT
}


void CIntentionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIntentionDlg)
	DDX_Text(pDX, ID_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_CONTRACT_OVERSEA, m_strFirstContract);
	DDX_Text(pDX, IDC_EDIT_CONTRACT_DOMESTIC, m_strSecondContract);
	DDX_Text(pDX, IDC_EDIT_FIRST_VOL, m_firstVol);
	DDV_MinMaxUInt(pDX, m_firstVol, 1, 20);
	DDX_Text(pDX, IDC_EDIT_SECOND_VOL, m_secondVol);
	DDV_MinMaxUInt(pDX, m_secondVol, 1, 20);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIntentionDlg, CDialog)
	//{{AFX_MSG_MAP(CIntentionDlg)
	ON_BN_CLICKED(IDC_BUTTON_ADD_INTENTION, OnButtonAddIntention)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIntentionDlg message handlers

void CIntentionDlg::OnButtonAddIntention() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_strName.IsEmpty())
	{
		AfxMessageBox("名称不能为空");
		return;
	}
	if (m_strFirstContract.IsEmpty())
	{
		AfxMessageBox("合约1名称不能为空");
		return;
	}
	if (m_strSecondContract.IsEmpty())
	{
		AfxMessageBox("合约2名称不能为空");
		return;
	}
	CDialog::OnOK();
}
