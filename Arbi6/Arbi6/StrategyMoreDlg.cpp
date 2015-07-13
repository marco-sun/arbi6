// StrategyMoreDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Arbi6.h"
#include "StrategyMoreDlg.h"
#include "assert.h"
#include "LogStore.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStrategyMoreDlg dialog


CStrategyMoreDlg::CStrategyMoreDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStrategyMoreDlg::IDD, pParent)
	, m_nAddOversea(0)
	, m_nAddDomestic(0)
{
	//{{AFX_DATA_INIT(CStrategyMoreDlg)
	m_strName = _T("");
	m_strBSOversea = _T("");
	m_strOverseaType = _T("");

	m_strBSDomestic = _T("");
	m_strCompare = _T("");

	m_strContractDomestic = _T("");
	m_strContractOversea = _T("");
	m_dDiff1 = 0.0;
	m_dStep  = 1.0;
	m_nVolumeDomestic = 0;

	m_nVolumeOversea = 0;
	m_nMaxCount = 1;
	m_dDiff2 = 0.0;

	m_maxPostions = 1;
	//}}AFX_DATA_INIT
	m_bRadio = 1;
}
CStrategyMoreDlg::~CStrategyMoreDlg()
{

}

void CStrategyMoreDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStrategyMoreDlg)
	DDX_Control(pDX, IDC_COM_DOMESTIC_TYPE, m_DomesticType);
	DDX_Control(pDX, ID_COM_OVERSEAS_TYPE, m_Overseatype);
	DDX_Control(pDX, IDC_COMBO_COMPARE1, m_BCompare);
	DDX_Control(pDX, IDC_COM_DOMESTIC, m_BSDomestic);
	DDX_Control(pDX, ID_COM_OVERSEAS, m_BSOversea);
	DDX_Text(pDX, ID_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_MAX_COUNT, m_nMaxCount);
	DDX_Text(pDX, IDC_EDIT_STEP, m_dStep);
	DDX_CBString(pDX, ID_COM_OVERSEAS, m_strBSOversea);
	DDX_CBString(pDX, ID_COM_OVERSEAS_TYPE, m_strOverseaType);
	DDX_CBString(pDX, IDC_COM_DOMESTIC, m_strBSDomestic);
	DDX_CBString(pDX, IDC_COM_DOMESTIC_TYPE, m_strDomesticType);
	DDX_CBString(pDX, IDC_COMBO_COMPARE1, m_strCompare);
	DDX_Text(pDX, IDC_EDIT_CONTRACT_DOMESTIC, m_strContractDomestic);
	DDX_Text(pDX, IDC_EDIT_CONTRACT_OVERSEA, m_strContractOversea);
	DDX_Text(pDX, IDC_EDIT_DIFF1, m_dDiff1);
	DDX_Text(pDX, IDC_EDIT_VOLUME_DOMESTIC, m_nVolumeDomestic);
	DDX_Text(pDX, IDC_EDIT_VOLUME_OVERSEA, m_nVolumeOversea);
	DDX_Text(pDX, IDC_EDIT_DIFF2, m_dDiff2);
	DDX_Text(pDX, IDC_EDIT_MAX_POSITIONS, m_maxPostions);
	DDV_MinMaxUInt(pDX, m_maxPostions, 0, 50);
	DDX_Control(pDX, IDC_CHECK_FISH1, m_checkFish1);
	DDX_Control(pDX, IDC_CHECK_PING, m_checkNoPing);
	//}}AFX_DATA_MAP


	DDX_Text(pDX, IDC_EDIT_ADD_OVERSEA, m_nAddOversea);
	DDX_Text(pDX, IDC_EDIT_ADD_DOMESTIC, m_nAddDomestic);
}


BEGIN_MESSAGE_MAP(CStrategyMoreDlg, CDialog)
	//{{AFX_MSG_MAP(CStrategyMoreDlg)
	ON_BN_CLICKED(ID_BUTTON_OK, OnButtonAdd)
	ON_BN_CLICKED(IDC_CHECK_EDIT_AND_START, OnCheckEditAndStart)
	ON_CBN_SELCHANGE(ID_COM_OVERSEAS, OnSelchangeComOverseas)
	ON_CBN_SELCHANGE(IDC_COM_DOMESTIC, OnSelchangeComDomestic)
	ON_CBN_SELCHANGE(ID_COM_OVERSEAS_TYPE, OnSelchangeComOverseasType)
	ON_CBN_SELCHANGE(IDC_COM_DOMESTIC_TYPE, OnSelchangeComDomesticType)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMBO_COMPARE1, &CStrategyMoreDlg::OnCbnSelchangeComboCompare1)
	ON_BN_CLICKED(IDC_CHECK_FISH1, &CStrategyMoreDlg::OnBnClickedCheckFish1)
	ON_EN_CHANGE(IDC_EDIT_ADD_OVERSEA, &CStrategyMoreDlg::OnEnChangeEditAddOversea)
	ON_BN_CLICKED(IDC_CHECK_PING, &CStrategyMoreDlg::OnBnClickedCheckPing)
	ON_EN_CHANGE(IDC_EDIT_DIFF2, &CStrategyMoreDlg::OnEnChangeEditDiff2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStrategyMoreDlg message handlers

void CStrategyMoreDlg::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	m_nBSOversea  = m_BSOversea.GetCurSel();
	m_nBSDomestic = m_BSDomestic.GetCurSel();
	UpdateData(TRUE);
	if(m_checkNoPing.GetCheck() != 1){
		if(m_strCompare == "小于等于" && m_dDiff1 > m_dDiff2){
			::AfxMessageBox(_T("价差应小于止赢点价差"));
			return;
		}
		if(m_strCompare == "大于等于" && m_dDiff1 < m_dDiff2){
			::AfxMessageBox(_T("价差应大于止赢点价差"));
			return;
		}
	}

	SetStrategy() ;
	CDialog::OnOK();
}
void CStrategyMoreDlg::SetStrategy() 
{
 	UpdateData(TRUE);
 	m_count = m_nMaxCount;
 //	m_tStrategyUI[i].sName = m_strName;
 	if(m_count < 1 && m_count > 50){
 		::AfxMessageBox(_T("总档数范围0～50"));
 		return;
 	}
 	for(int i = 0; i<m_count;i++)
 	{
 		m_tStrategyUI[i].maxPostions = m_maxPostions;
 		m_tStrategyUI[i].fistname = m_strContractOversea;
 		m_tStrategyUI[i].secondname = m_strContractDomestic;
 
 		m_tStrategyUI[i].type = 3;
 		if (m_strCompare == "大于等于"){
 			m_tStrategyUI[i].tStrategyPrarm[0].compare = ">=";
 			m_tStrategyUI[i].tStrategyPrarm[1].compare = "<=";
 			m_tStrategyUI[i].tStrategyPrarm[0].diff = m_dDiff1 + i*m_dStep;
 			m_tStrategyUI[i].tStrategyPrarm[1].diff = m_dDiff2 + i*m_dStep;
 		}
 		else
 		{
 			m_tStrategyUI[i].tStrategyPrarm[0].compare = "<=";
 			m_tStrategyUI[i].tStrategyPrarm[1].compare = ">=";
 			m_tStrategyUI[i].tStrategyPrarm[0].diff = m_dDiff1 - i*m_dStep;
 			m_tStrategyUI[i].tStrategyPrarm[1].diff = m_dDiff2 - i*m_dStep;
 		}
 		
 
 		m_tStrategyUI[i].tStrategyPrarm[0].domesticAdd = m_nAddDomestic;
 		m_tStrategyUI[i].tStrategyPrarm[1].domesticAdd = m_nAddDomestic;
 
 		if (m_strBSDomestic == "买"){
 			m_tStrategyUI[i].tStrategyPrarm[0].domesticBuySell = BUY;
 			m_tStrategyUI[i].tStrategyPrarm[1].domesticBuySell = SELL;
 		}
 		else{
 			m_tStrategyUI[i].tStrategyPrarm[0].domesticBuySell = SELL;
 			m_tStrategyUI[i].tStrategyPrarm[1].domesticBuySell = BUY;
 		}
 
 		m_tStrategyUI[i].tStrategyPrarm[0].domesticVolume = m_nVolumeDomestic;
 		m_tStrategyUI[i].tStrategyPrarm[1].domesticVolume = m_nVolumeDomestic;
 
 		m_tStrategyUI[i].tStrategyPrarm[0].overseaAdd = m_nAddOversea;
 		m_tStrategyUI[i].tStrategyPrarm[1].overseaAdd = m_nAddOversea;
 
 		if (m_strBSOversea == "买"){
 			m_tStrategyUI[i].tStrategyPrarm[0].overseaBuySell = BUY;
 			m_tStrategyUI[i].tStrategyPrarm[1].overseaBuySell = SELL;
 		}
 		else{
 			m_tStrategyUI[i].tStrategyPrarm[0].overseaBuySell = SELL;
			m_tStrategyUI[i].tStrategyPrarm[1].overseaBuySell = BUY;
 		}
 		m_tStrategyUI[i].tStrategyPrarm[0].overseaVolume = m_nVolumeOversea;
 		m_tStrategyUI[i].tStrategyPrarm[1].overseaVolume = m_nVolumeOversea;
 
 		if (m_strOverseaType == "开仓"){
 			m_tStrategyUI[i].tStrategyPrarm[0].overseaType = KAI_CANG;
 			m_tStrategyUI[i].tStrategyPrarm[1].overseaType = PING_JIN;
 		}
 		else if (m_strOverseaType == "平仓"){
 			m_tStrategyUI[i].tStrategyPrarm[0].overseaType = PING_CANG;
 			m_tStrategyUI[i].tStrategyPrarm[1].overseaType = KAI_CANG;
 		}
 		else if (m_strOverseaType == "平今"){
 			m_tStrategyUI[i].tStrategyPrarm[0].overseaType = PING_JIN;
 			m_tStrategyUI[i].tStrategyPrarm[1].overseaType = KAI_CANG;
 		}
 
 		if (m_strDomesticType == "开仓"){
 			m_tStrategyUI[i].tStrategyPrarm[0].domesticType = KAI_CANG;
 			m_tStrategyUI[i].tStrategyPrarm[1].domesticType = PING_JIN;
 		}
 		else if (m_strDomesticType == "平仓"){
 			m_tStrategyUI[i].tStrategyPrarm[0].domesticType = PING_CANG;
 			m_tStrategyUI[i].tStrategyPrarm[1].domesticType = KAI_CANG;
 		}
 		else if (m_strDomesticType == "平今"){
 			m_tStrategyUI[i].tStrategyPrarm[0].domesticType = PING_JIN;
 			m_tStrategyUI[i].tStrategyPrarm[1].domesticType = KAI_CANG;
 		}
 		
 
 		if(m_checkFish1.GetCheck() == 1){
 			m_tStrategyUI[i].tStrategyPrarm[0].fishflag = 1;
 			m_tStrategyUI[i].tStrategyPrarm[1].fishflag = 1;
 		}
 		else{
 			m_tStrategyUI[i].tStrategyPrarm[0].fishflag = 0;
 			m_tStrategyUI[i].tStrategyPrarm[1].fishflag = 0;
 		}
 		if(m_checkNoPing.GetCheck() == 1){
 			m_tStrategyUI[i].type = 1;
 		}else{
 			m_tStrategyUI[i].type = 3;
 		}
 
 		//checkCondition();
 	}
}

void CStrategyMoreDlg::checkCondition()
{
	if((m_strCompare == "小于等于" && m_dDiff1 > m_dDiff2)
		|| (m_strCompare == "大于等于" && m_dDiff1 < m_dDiff2))
	{

		TRACE_LOG("set wrong Condition!");
		exit(1);
	}
}

BOOL CStrategyMoreDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_BSOversea.SetCurSel(0);
	m_BSDomestic.SetCurSel(1);
	m_BCompare.SetCurSel(1);
	m_Overseatype.SetCurSel(0);
	m_DomesticType.SetCurSel(0);

	CButton* pButton = NULL;
	m_nOP = 0;
	pButton = (CButton *)GetDlgItem(IDC_CHECK_EDIT_AND_START);
	pButton->SetCheck(0);
	pButton = (CButton *)GetDlgItem(IDC_RADIO1);
 	pButton->SetCheck(1);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStrategyMoreDlg::OnCheckEditAndStart() 
{
	// TODO: Add your control notification handler code here
	if (m_nOP == 0)
	{
		m_nOP = 1;
		CButton* pButton = (CButton *)GetDlgItem(IDC_CHECK_EDIT_AND_START);
		pButton->SetCheck(1);
	}else
	{
		m_nOP = 0;
		CButton* pButton = (CButton *)GetDlgItem(IDC_CHECK_EDIT_AND_START);
		pButton->SetCheck(0);
	}
}

void CStrategyMoreDlg::OnSelchangeComOverseas() 
{
	// TODO: Add your control notification handler code here
	if (m_BSOversea.GetCurSel() == 0)
	{
		m_BSDomestic.SetCurSel(1);
		//m_BSDomestic2.SetCurSel(0);
		//m_BSOversea2.SetCurSel(1);
		m_BCompare.SetCurSel(1);
		//m_BCompare2.SetCurSel(0);
	}
	else
	{
		m_BSDomestic.SetCurSel(0);
		//m_BSDomestic2.SetCurSel(1);
		//m_BSOversea2.SetCurSel(0);
	    m_BCompare.SetCurSel(0);
		//m_BCompare2.SetCurSel(1);
	}
}

void CStrategyMoreDlg::OnSelchangeComDomestic() 
{
	// TODO: Add your control notification handler code here
	if (m_BSDomestic.GetCurSel() == 0)
	{
		m_BSOversea.SetCurSel(1);
		//m_BSDomestic2.SetCurSel(1);
		//m_BSOversea2.SetCurSel(0);
		m_BCompare.SetCurSel(0);
		//m_BCompare2.SetCurSel(1);
	}
	else
	{
		m_BSOversea.SetCurSel(0);
		//m_BSDomestic2.SetCurSel(0);
		//m_BSOversea2.SetCurSel(1);
	    m_BCompare.SetCurSel(1);
		//m_BCompare2.SetCurSel(0);
	}
}

void CStrategyMoreDlg::OnSelchangeComOverseasType() 
{
	// TODO: Add your control notification handler code here
	m_DomesticType.SetCurSel(m_Overseatype.GetCurSel());
}



void CStrategyMoreDlg::OnSelchangeComDomesticType() 
{
	// TODO: Add your control notification handler code here

}


void CStrategyMoreDlg::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	CButton* pButton = (CButton *)GetDlgItem(IDC_RADIO2);
	pButton->SetCheck(0);
	m_bRadio = 1;
}

void CStrategyMoreDlg::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	CButton* pButton = (CButton *)GetDlgItem(IDC_RADIO1);
	pButton->SetCheck(0);
	m_bRadio = 2;
}


void CStrategyMoreDlg::OnCbnSelchangeComboCompare1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_BCompare.GetCurSel() == 0)
	{
		//this->m_BCompare2.SetCurSel(1);
		m_BSOversea.SetCurSel(1);
        OnSelchangeComOverseas() ;

	}
	else
	{
		//this->m_BCompare2.SetCurSel(0);
		m_BSOversea.SetCurSel(0);
        OnSelchangeComOverseas() ;
	}
}




void CStrategyMoreDlg::OnBnClickedCheckFish1()
{
	// TODO: 在此添加控件通知处理程序代码
	;
}




void CStrategyMoreDlg::OnEnChangeEditAddOversea()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CStrategyMoreDlg::OnBnClickedCheckPing()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CStrategyMoreDlg::OnEnChangeEditDiff2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
