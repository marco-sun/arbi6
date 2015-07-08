// DialogSugar.cpp : 实现文件
//

#include "stdafx.h"
#include "Arbi5UI.h"
#include "DialogSugar.h"
#include "afxdialogex.h"
#include "resource.h"
#include "..\Price.h"


// DialogSugar 对话框

IMPLEMENT_DYNAMIC(DialogSugar, CDialogEx)

DialogSugar::DialogSugar(CWnd* pParent /*=NULL*/)
	: CDialogEx(DialogSugar::IDD, pParent)
{
	
}

DialogSugar::~DialogSugar()
{
}

void DialogSugar::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DialogSugar, CDialogEx)
	ON_BN_CLICKED(IDOK, &DialogSugar::OnBnClickedOk)
END_MESSAGE_MAP()


// DialogSugar 消息处理程序

void DialogSugar::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	CString s1;
	CString s2;
	GetDlgItemText(IDC_EDIT_SUGAR, s1);
	GetDlgItemText(IDC_EDIT_SUGAR2, s2);
	Price::SBH4.set(time(NULL), atof(s1), atof(s1));
	Price::SBK4.set(time(NULL), atof(s2), atof(s2));
	CDialog::OnOK();
}
