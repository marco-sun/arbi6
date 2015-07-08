// OperManageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Arbi5UI.h"
#include "OperManageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COperManageDlg dialog


COperManageDlg::COperManageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COperManageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COperManageDlg)
	m_strUser = _T("");
	m_strOldPwd = _T("");
	m_strNewPwd = _T("");
	m_strConfirmPwd = _T("");
	//}}AFX_DATA_INIT
}


void COperManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COperManageDlg)
	DDX_Text(pDX, IDC_EDIT_USER, m_strUser);
	DDX_Text(pDX, IDC_EDIT_OLDPWD, m_strOldPwd);
	DDX_Text(pDX, IDC_EDIT_NEWPWD, m_strNewPwd);
	DDX_Text(pDX, IDC_EDIT_CONFIRM_PWD, m_strConfirmPwd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COperManageDlg, CDialog)
	//{{AFX_MSG_MAP(COperManageDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COperManageDlg message handlers

void COperManageDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	char szTemp[32];
	memset(szTemp, 0, sizeof(szTemp));
	GetPrivateProfileString("USER", "PW", "zhujinadmin", szTemp, 32, ".\\AIB.ini");
	CString strpwd = (CString)szTemp;

	if (m_strOldPwd != strpwd)
	{
		AfxMessageBox("旧密码输入不正确，请重新输入!");
		return;
	}
	if (m_strNewPwd.IsEmpty() || m_strNewPwd != m_strConfirmPwd)
	{
		AfxMessageBox("新密码输入不正确，请重新输入!");
		return;
	}
	WritePrivateProfileString("USER", "PW", m_strNewPwd.GetBuffer(m_strNewPwd.GetLength()), ".\\AIB.ini");
	CDialog::OnOK();
}

BOOL COperManageDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_strUser = "admin";
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
