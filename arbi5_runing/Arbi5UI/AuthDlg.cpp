// AuthDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Arbi5UI.h"
#include "AuthDlg.h"
#include "Info.h"
#include "UserLoadDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAuthDlg dialog


CAuthDlg::CAuthDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAuthDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAuthDlg)
	m_strAuth = _T("");
	//}}AFX_DATA_INIT
}


void CAuthDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAuthDlg)
	DDX_Control(pDX, IDC_EDIT_LOCAL_CODE, m_conLocalCode);
	DDX_Text(pDX, IDC_EDIT_AUTH_CODE, m_strAuth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAuthDlg, CDialog)
	//{{AFX_MSG_MAP(CAuthDlg)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAuthDlg message handlers

void CAuthDlg::OnOK() 
{
	// TODO: Add extra validation here
	CDialog::OnOK();
	UpdateData(TRUE);
	if (m_strAuth.IsEmpty())
	{
		AfxMessageBox("授权码不正确！请重新输入！");
		return;
	}
	
    CString strCupID;
	CString strDecode;
	CString strOldkey;
		
    strOldkey = m_strAuth;
	
	DecodeKey(strOldkey, strDecode);
	GetCUPID(strCupID);
	strCupID = strCupID.Mid(0,26);
	if (strCupID.Compare(strDecode.GetBuffer(strDecode.GetLength())) == 0)
	{
		WritePrivateProfileString("KEY", "key1", strOldkey, ".\\AIB.ini");
		CUserLoadDlg userLoadDlg;
		userLoadDlg.DoModal();
	}
	else
	{
		AfxMessageBox("授权码不正确！请重新输入！");
		return;
	}
	
}

BOOL CAuthDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//CreateKey() ;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CAuthDlg::CreateKey() 
{
	// TODO: Add your control notification handler code here
		// TODO: Add extra validation here
	unsigned long s1,s2;
	unsigned char vendor_id[]="------------------";
	CString str1,str2,str3;
	__asm
	{
		xor eax,eax
			cpuid
			mov dword ptr vendor_id,ebx
			mov dword ptr vendor_id[+4],edx
			mov dword ptr vendor_id[+8],ecx
	}
	str1.Format("%s",vendor_id);
	__asm  //取得CPU ID的高32位
	{
		mov eax,01h
			xor edx,edx
			cpuid
			mov s2,eax
	}
	str2.Format("%08X-",s2);
	
	__asm //取得CPU ID的低64位
	{
		mov eax,03h
			xor ecx,ecx
			xor edx,edx
			cpuid 
			mov s1,edx
			mov s2,ecx
	}
	
	str3.Format("%08X-%08X\n",s1,s2);
	str2+=str3;
	CString strEncode = _T("");
	EnCodeID(str2,strEncode);
	m_conLocalCode.SetWindowText(strEncode);
}

BOOL CAuthDlg::EnCodeID(CString strID, CString &strEncode)
{
	int nkey[17];
	int nID[16];
	CString szTemp;
	char szEncode[10];
	memset(szEncode,0,10);

	nkey[0] = 1;
	nkey[1] = 0;
	nkey[2] = 0;
	nkey[3] = 9;
	nkey[4] = 2;
	nkey[5] = 0;
	nkey[6] = 0;
	nkey[7] = 5;

	nkey[9]  = 2;
	nkey[10] = 0;
	nkey[11] = 0;
	nkey[12] = 5;
	nkey[13] = 1;
	nkey[14] = 0;
	nkey[15] = 0;
	nkey[16] = 9;
	
    if (strID.GetLength() < 26)
    {
		return FALSE;
    }
	int i;
	for (i = 0; i < 8; i++)
	{
		szTemp = strID.Mid(i,1);
		if((strID[i] >= '0') && (strID[i] <= '9')) 
		{
			nID[i] = atoi(szTemp);
			if (nID[i] < 10 )
			{
				nID[i] = nID[i] + nkey[i];
				if (nID[i] >= 10)
				{
					nID[i] = nID[i] - 10;
				}
				_itoa_s(nID[i],szEncode,10,10);
				strEncode = strEncode + (CString)szEncode;
			}
		}
		else
		{
			strEncode = strEncode + szTemp;
		}
	}
	strEncode = strEncode + "-";
	for (i = 9; i < 17; i++)
	{
		szTemp = strID.Mid(i,1);
		if((strID[i] >= '0') && (strID[i] <= '9')) 
		{
			nID[i] = atoi(szTemp);
			if (nID[i] < 10 )
			{
				if (nID[i] < nkey[i])
				{
					nID[i] = nID[i] + 10 - nkey[i];
				}
				else
				{
					nID[i] = nID[i] - nkey[i];
				}
				_itoa_s(nID[i],szEncode,10,10);
				strEncode = strEncode + (CString)szEncode;
			}
		}
		else
		{
			strEncode = strEncode + szTemp;
		}
	}
	szTemp = strID.Mid(18,8);
	strEncode = strEncode + "-" + szTemp;
	
	return TRUE;
}

void CAuthDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	exit(1);
	CDialog::OnClose();
}
