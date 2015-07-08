// UserLoadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Arbi5UI.h"
#include "UserLoadDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserLoadDlg dialog


CUserLoadDlg::CUserLoadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserLoadDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserLoadDlg)
	m_struser = _T("");
	m_strpwd = _T("");
	//}}AFX_DATA_INIT
}


void CUserLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserLoadDlg)
	DDX_Text(pDX, IDC_EDIT_USER, m_struser);
	DDX_Text(pDX, IDC_EDIT_PWD, m_strpwd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserLoadDlg, CDialog)
	//{{AFX_MSG_MAP(CUserLoadDlg)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnButtonLoad)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserLoadDlg message handlers

void CUserLoadDlg::OnButtonLoad() 
{
	if (Validation() == TRUE)
		CDialog::OnOK();
	else{
		AfxMessageBox("输入信息不正确，请重新输入!");
		return;}
}

BOOL CUserLoadDlg::Validation() 
{
	BOOL bRet = FALSE;
	UpdateData(TRUE);
	char szTemp[32];
	memset(szTemp, 0, sizeof(szTemp));
	GetPrivateProfileString("USER", "PW", "zhujinadmin", szTemp, 32, ".\\AIB.ini");
	CString strpwd = (CString)szTemp;
	if (m_struser == "admin" && m_strpwd == strpwd)
		bRet = TRUE;

	return bRet;
}

void CUserLoadDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	exit(1);
	CDialog::OnClose();
}

BOOL CUserLoadDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_struser = "admin";
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CUserLoadDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->wParam==VK_RETURN) 
	{
		if (Validation() == FALSE)
		{
			AfxMessageBox("输入信息不正确，请重新输入!");
			return TRUE;
		}
		return CDialog::PreTranslateMessage(pMsg); 
	}
	else 
		return CDialog::PreTranslateMessage(pMsg); 

}
