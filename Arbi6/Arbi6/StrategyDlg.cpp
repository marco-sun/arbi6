// StrategyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Arbi6.h"
#include "StrategyDlg.h"
#include <assert.h>
#include "LogStore.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStrategyDlg dialog


CStrategyDlg::CStrategyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStrategyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStrategyDlg)
	m_strName = _T("");
	m_strBSOversea = _T("");
	m_strOverseaType = _T("");
	m_strOverseatype2 = _T("");
	m_strBSDomestic = _T("");
	m_strDomesticType = _T("");
	m_strDomesticType2 = _T("");
	m_strCompare = _T("");
	m_strCompare2 = _T("");
	m_nAddDomestic = 2;
	m_nAddOversea = 4;
	m_nAddDomestic2 = 2;
	m_nAddOversea2 = 4;
	m_strContractDomestic = _T("");
	m_strContractOversea = _T("");
	m_dDiff1 = 0.0;
	m_nVolumeDomestic = 0;
	m_nVolumeDomestic2 = 0;
	m_nVolumeOversea = 0;
	m_nVolumeOversea2 = 0;
	m_dDiff2 = 0.0;
	m_strBSOversea2 = _T("");
	m_strBSDomestic2 = _T("");
	m_maxPostions = 1;
	//}}AFX_DATA_INIT
	m_bRadio = 1;
}
CStrategyDlg::~CStrategyDlg()
{

}

void CStrategyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStrategyDlg)
	DDX_Control(pDX, IDC_COM_DOMESTIC_TYPE2, m_DomesticType2);
	DDX_Control(pDX, IDC_COM_DOMESTIC_TYPE, m_DomesticType);
	DDX_Control(pDX, ID_COM_OVERSEAS_TYPE2, m_Overseatype2);
	DDX_Control(pDX, ID_COM_OVERSEAS_TYPE, m_Overseatype);
	DDX_Control(pDX, IDC_COMBO_COMPARE2, m_BCompare2);
	DDX_Control(pDX, IDC_COMBO_COMPARE1, m_BCompare);
	DDX_Control(pDX, ID_COM_OVERSEAS2, m_BSOversea2);
	DDX_Control(pDX, IDC_COM_DOMESTIC2, m_BSDomestic2);
	DDX_Control(pDX, IDC_COM_DOMESTIC, m_BSDomestic);
	DDX_Control(pDX, ID_COM_OVERSEAS, m_BSOversea);
	DDX_Text(pDX, ID_EDIT_NAME, m_strName);
	DDX_CBString(pDX, ID_COM_OVERSEAS, m_strBSOversea);
	DDX_CBString(pDX, ID_COM_OVERSEAS_TYPE, m_strOverseaType);
	DDX_CBString(pDX, ID_COM_OVERSEAS_TYPE2, m_strOverseatype2);
	DDX_CBString(pDX, IDC_COM_DOMESTIC, m_strBSDomestic);
	DDX_CBString(pDX, IDC_COM_DOMESTIC_TYPE, m_strDomesticType);
	DDX_CBString(pDX, IDC_COM_DOMESTIC_TYPE2, m_strDomesticType2);
	DDX_CBString(pDX, IDC_COMBO_COMPARE1, m_strCompare);
	DDX_CBString(pDX, IDC_COMBO_COMPARE2, m_strCompare2);
	DDX_Text(pDX, IDC_EDIT_ADD_DOMESTIC, m_nAddDomestic);
	DDX_Text(pDX, IDC_EDIT_ADD_OVERSEA, m_nAddOversea);
	DDX_Text(pDX, IDC_EDIT_ADD_DOMESTIC2, m_nAddDomestic2);
	DDX_Text(pDX, IDC_EDIT_ADD_OVERSEA2, m_nAddOversea2);
	DDX_Text(pDX, IDC_EDIT_CONTRACT_DOMESTIC, m_strContractDomestic);
	DDX_Text(pDX, IDC_EDIT_CONTRACT_OVERSEA, m_strContractOversea);
	DDX_Text(pDX, IDC_EDIT_DIFF1, m_dDiff1);
	DDX_Text(pDX, IDC_EDIT_VOLUME_DOMESTIC, m_nVolumeDomestic);
	DDX_Text(pDX, IDC_EDIT_VOLUME_DOMESTIC2, m_nVolumeDomestic2);
	DDX_Text(pDX, IDC_EDIT_VOLUME_OVERSEA, m_nVolumeOversea);
	DDX_Text(pDX, IDC_EDIT_VOLUME_OVERSEA2, m_nVolumeOversea2);
	DDX_Text(pDX, IDC_EDIT_DIFF2, m_dDiff2);
	DDX_CBString(pDX, ID_COM_OVERSEAS2, m_strBSOversea2);
	DDX_CBString(pDX, IDC_COM_DOMESTIC2, m_strBSDomestic2);
	DDX_Text(pDX, IDC_EDIT_MAX_POSITIONS, m_maxPostions);
	DDV_MinMaxUInt(pDX, m_maxPostions, 0, 100);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHECK_FISH1, m_checkFish1);
	DDX_Control(pDX, IDC_CHECK_FISH2, m_checkFish2);
}


BEGIN_MESSAGE_MAP(CStrategyDlg, CDialog)
	//{{AFX_MSG_MAP(CStrategyDlg)
	ON_BN_CLICKED(ID_BUTTON_OK, OnButtonAdd)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDC_CHECK_EDIT_AND_START, OnCheckEditAndStart)
	ON_CBN_SELCHANGE(ID_COM_OVERSEAS, OnSelchangeComOverseas)
	ON_CBN_SELCHANGE(IDC_COM_DOMESTIC, OnSelchangeComDomestic)
	ON_CBN_SELCHANGE(ID_COM_OVERSEAS_TYPE, OnSelchangeComOverseasType)
	ON_CBN_SELCHANGE(ID_COM_OVERSEAS_TYPE2, OnSelchangeComOverseasType2)
	ON_CBN_SELCHANGE(ID_COM_OVERSEAS2, OnSelchangeComOverseas2)
	ON_CBN_SELCHANGE(IDC_COM_DOMESTIC_TYPE, OnSelchangeComDomesticType)
	ON_CBN_SELCHANGE(IDC_COM_DOMESTIC_TYPE2, OnSelchangeComDomesticType2)
	ON_CBN_SELCHANGE(IDC_COM_DOMESTIC2, OnSelchangeComDomestic2)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMBO_COMPARE1, &CStrategyDlg::OnCbnSelchangeComboCompare1)
	ON_CBN_SELCHANGE(IDC_COMBO_COMPARE2, &CStrategyDlg::OnCbnSelchangeComboCompare2)
	ON_BN_CLICKED(IDC_CHECK_FISH1, &CStrategyDlg::OnBnClickedCheckFish1)
	ON_BN_CLICKED(IDC_CHECK_FISH2, &CStrategyDlg::OnBnClickedCheckFish2)
	ON_BN_CLICKED(ID_BUTTON_TURN_OVER, &CStrategyDlg::OnBnClickedButtonTurnOver)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStrategyDlg message handlers

void CStrategyDlg::OnButtonAdd() 
{
// 	TODO: Add your control notification handler code here
 		m_nBSOversea  = m_BSOversea.GetCurSel();
 		m_nBSDomestic = m_BSDomestic.GetCurSel();
 		UpdateData(TRUE);
 		if((m_bb1 == 1) && (m_bb2 == 1) ){
 			if(m_strCompare == m_strCompare2){
 				::AfxMessageBox(_T("两腿策略价差不能同方向设置！"));
 				return;
 			}
 			if(m_strCompare == "小于等于" && m_dDiff1 > m_dDiff2){
 				::AfxMessageBox(_T("价差一应小于价差二"));
 				return;
 			}
 			if(m_strCompare == "大于等于" && m_dDiff1 < m_dDiff2){
 				::AfxMessageBox(_T("价差一应大于价差二"));
 				return;
 			}
 		}

 		m_tStrategyUI.maxPostions = m_maxPostions;
 		m_tStrategyUI.fistname = m_strContractOversea;
 		m_tStrategyUI.secondname = m_strContractDomestic;
 		if (m_bb1 == 1 && m_bb2 == 1)
 		{
 			m_tStrategyUI.type = 3;
 			if (m_strCompare == "大于等于")
 				m_tStrategyUI.tStrategyPrarm[0].compare = ">=";
 			else
 				m_tStrategyUI.tStrategyPrarm[0].compare = "<=";
 			m_tStrategyUI.tStrategyPrarm[0].diff = m_dDiff1;
 			m_tStrategyUI.tStrategyPrarm[0].domesticAdd = m_nAddDomestic;
 			if (m_strBSDomestic == "买")
 				m_tStrategyUI.tStrategyPrarm[0].domesticBuySell = BUY;
 			else
 				m_tStrategyUI.tStrategyPrarm[0].domesticBuySell = SELL;
 			m_tStrategyUI.tStrategyPrarm[0].domesticVolume = m_nVolumeDomestic;
 			m_tStrategyUI.tStrategyPrarm[0].overseaAdd = m_nAddOversea;
 			if (m_strBSOversea == "买")
 				m_tStrategyUI.tStrategyPrarm[0].overseaBuySell = BUY;
 			else
 				m_tStrategyUI.tStrategyPrarm[0].overseaBuySell = SELL;
 			m_tStrategyUI.tStrategyPrarm[0].overseaVolume = m_nVolumeOversea;
 	
 			if (m_strOverseaType == "开仓")
 				m_tStrategyUI.tStrategyPrarm[0].overseaType = KAI_CANG;
 			else if (m_strOverseaType == "平仓")
 				m_tStrategyUI.tStrategyPrarm[0].overseaType = PING_CANG;
 			else if (m_strOverseaType == "平今")
 				m_tStrategyUI.tStrategyPrarm[0].overseaType = PING_JIN;
 	
 			if (m_strDomesticType == "开仓")
 				m_tStrategyUI.tStrategyPrarm[0].domesticType = KAI_CANG;
 			else if (m_strDomesticType == "平仓")
 				m_tStrategyUI.tStrategyPrarm[0].domesticType = PING_CANG;
 			else if (m_strDomesticType == "平今")
 				m_tStrategyUI.tStrategyPrarm[0].domesticType = PING_JIN;
 			
 				m_tStrategyUI.tStrategyPrarm[1].diff = m_dDiff2;
 			if (m_strCompare2 == "大于等于")
 				m_tStrategyUI.tStrategyPrarm[1].compare = ">=";
 			else
 				m_tStrategyUI.tStrategyPrarm[1].compare = "<=";
 	
 			m_tStrategyUI.tStrategyPrarm[1].domesticAdd = m_nAddDomestic2;
 			if (m_strBSDomestic2 == "买")
 				m_tStrategyUI.tStrategyPrarm[1].domesticBuySell = BUY;
 			else
 				m_tStrategyUI.tStrategyPrarm[1].domesticBuySell = SELL;
 			m_tStrategyUI.tStrategyPrarm[1].domesticVolume = m_nVolumeDomestic2;
 			m_tStrategyUI.tStrategyPrarm[1].overseaAdd = m_nAddOversea2;
 			if (m_strBSOversea2 == "买")
 				m_tStrategyUI.tStrategyPrarm[1].overseaBuySell = BUY;
 			else
 				m_tStrategyUI.tStrategyPrarm[1].overseaBuySell = SELL;
 			m_tStrategyUI.tStrategyPrarm[1].overseaVolume = m_nVolumeOversea2;
 	
 			if (m_strOverseatype2 == "开仓")
 				m_tStrategyUI.tStrategyPrarm[1].overseaType = KAI_CANG;
 			else if (m_strOverseatype2 == "平仓")
 				m_tStrategyUI.tStrategyPrarm[1].overseaType = PING_CANG;
 			else if (m_strOverseatype2 == "平今")
 				m_tStrategyUI.tStrategyPrarm[1].overseaType = PING_JIN;
 	
 			if (m_strDomesticType2 == "开仓")
 				m_tStrategyUI.tStrategyPrarm[1].domesticType = KAI_CANG;
 			else if (m_strDomesticType2 == "平仓")
 				m_tStrategyUI.tStrategyPrarm[1].domesticType = PING_CANG;
 			else if (m_strDomesticType2 == "平今")
 				m_tStrategyUI.tStrategyPrarm[1].domesticType = PING_JIN;
 	
 			if(m_checkFish1.GetCheck() == 1)
 				m_tStrategyUI.tStrategyPrarm[0].fishflag = 1;
 			else
 				m_tStrategyUI.tStrategyPrarm[0].fishflag = 0;
 	
 			if(m_checkFish2.GetCheck() == 1)
 				m_tStrategyUI.tStrategyPrarm[1].fishflag = 1;
 			else
 				m_tStrategyUI.tStrategyPrarm[1].fishflag = 0;
 		}
 		if(m_bb1 == 1 && m_bb2 != 1)
 		{
 			m_tStrategyUI.type = 1;
 			if (m_strCompare == "大于等于")
 				m_tStrategyUI.tStrategyPrarm[0].compare = ">=";
 			else
 				m_tStrategyUI.tStrategyPrarm[0].compare = "<=";
 			m_tStrategyUI.tStrategyPrarm[0].diff = m_dDiff1;
 			m_tStrategyUI.tStrategyPrarm[0].domesticAdd = m_nAddDomestic;
 			if (m_strBSDomestic == "买")
 				m_tStrategyUI.tStrategyPrarm[0].domesticBuySell = BUY;
 			else
 				m_tStrategyUI.tStrategyPrarm[0].domesticBuySell = SELL;
 			m_tStrategyUI.tStrategyPrarm[0].domesticVolume = m_nVolumeDomestic;
 			m_tStrategyUI.tStrategyPrarm[0].overseaAdd = m_nAddOversea;
 			if (m_strBSOversea == "买")
 				m_tStrategyUI.tStrategyPrarm[0].overseaBuySell = BUY;
 			else
 				m_tStrategyUI.tStrategyPrarm[0].overseaBuySell = SELL;
 			m_tStrategyUI.tStrategyPrarm[0].overseaVolume = m_nVolumeOversea;
 	
 			if (m_strOverseaType == "开仓")
 				m_tStrategyUI.tStrategyPrarm[0].overseaType = KAI_CANG;
 			else if (m_strOverseaType == "平仓")
 				m_tStrategyUI.tStrategyPrarm[0].overseaType = PING_CANG;
 			else if (m_strOverseaType == "平今")
 				m_tStrategyUI.tStrategyPrarm[0].overseaType = PING_JIN;
 	
 			if (m_strDomesticType == "开仓")
 				m_tStrategyUI.tStrategyPrarm[0].domesticType = KAI_CANG;
 			else if (m_strDomesticType == "平仓")
 				m_tStrategyUI.tStrategyPrarm[0].domesticType = PING_CANG;
 			else if (m_strDomesticType == "平今")
 				m_tStrategyUI.tStrategyPrarm[0].domesticType = PING_JIN;
 	
 			if(m_checkFish1.GetCheck() == 1)
 				m_tStrategyUI.tStrategyPrarm[0].fishflag = 1;
 			else
 				m_tStrategyUI.tStrategyPrarm[0].fishflag = 0;
 	
 		}
 		if(m_bb1 != 1 && m_bb2 == 1)
 		{
 			m_tStrategyUI.type = 2;
 			m_tStrategyUI.tStrategyPrarm[1].diff = m_dDiff2;
 			if (m_strCompare2 == "大于等于")
 				m_tStrategyUI.tStrategyPrarm[1].compare = ">=";
 			else
 				m_tStrategyUI.tStrategyPrarm[1].compare = "<=";
 	
 			m_tStrategyUI.tStrategyPrarm[1].domesticAdd = m_nAddDomestic2;
 			if (m_strBSDomestic2 == "买")
 				m_tStrategyUI.tStrategyPrarm[1].domesticBuySell = BUY;
 			else
 				m_tStrategyUI.tStrategyPrarm[1].domesticBuySell = SELL;
 			m_tStrategyUI.tStrategyPrarm[1].domesticVolume = m_nVolumeDomestic2;
 			m_tStrategyUI.tStrategyPrarm[1].overseaAdd = m_nAddOversea2;
 			if (m_strBSOversea2 == "买")
 				m_tStrategyUI.tStrategyPrarm[1].overseaBuySell = BUY;
 			else
 				m_tStrategyUI.tStrategyPrarm[1].overseaBuySell = SELL;
 			m_tStrategyUI.tStrategyPrarm[1].overseaVolume = m_nVolumeOversea2;
 	
 			if (m_strOverseatype2 == "开仓")
 				m_tStrategyUI.tStrategyPrarm[1].overseaType = KAI_CANG;
 			else if (m_strOverseatype2 == "平仓")
 				m_tStrategyUI.tStrategyPrarm[1].overseaType = PING_CANG;
 			else if (m_strOverseatype2 == "平今")
 				m_tStrategyUI.tStrategyPrarm[1].overseaType = PING_JIN;
 	
 			if (m_strDomesticType2 == "开仓")
 				m_tStrategyUI.tStrategyPrarm[1].domesticType = KAI_CANG;
 			else if (m_strDomesticType2 == "平仓")
 				m_tStrategyUI.tStrategyPrarm[1].domesticType = PING_CANG;
 			else if (m_strDomesticType2 == "平今")
 				m_tStrategyUI.tStrategyPrarm[1].domesticType = PING_JIN;
 	
 			if(m_checkFish2.GetCheck() == 1)
 				m_tStrategyUI.tStrategyPrarm[1].fishflag = 1;
 			else
 				m_tStrategyUI.tStrategyPrarm[1].fishflag = 0;
 	
 		}
 	
 		if(this->m_bb1 == 1 && this->m_bb2 == 1)
 		{

 		}
 	
 		CDialog::OnOK();
}

void CStrategyDlg::checkCondition()
{
	if((m_strCompare == "小于等于" && m_dDiff1 > m_dDiff2)
		|| (m_strCompare == "大于等于" && m_dDiff1 < m_dDiff2))
	{

		TRACE_LOG("set wrong Condition!");
		exit(1);
	}
}

BOOL CStrategyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_BSOversea.SetCurSel(0);
	m_BSOversea2.SetCurSel(1);
	m_BSDomestic.SetCurSel(1);
	m_BSDomestic2.SetCurSel(0);
	m_BCompare.SetCurSel(1);
	m_BCompare2.SetCurSel(0);
	m_Overseatype.SetCurSel(0);
	m_DomesticType.SetCurSel(0);
	m_Overseatype2.SetCurSel(1);
	m_DomesticType2.SetCurSel(1);
//	m_concern.SetCurSel(0);
//	CButton* pButton = (CButton *)GetDlgItem(IDC_BUTTON_ADD);

	CButton* pButton = NULL;
	m_bb1 = 1;
	pButton = (CButton *)GetDlgItem(IDC_CHECK1);
	pButton->SetCheck(1);
	m_bb2 = 1;
	pButton = (CButton *)GetDlgItem(IDC_CHECK2);
	pButton->SetCheck(1);
	m_nOP = 0;
	pButton = (CButton *)GetDlgItem(IDC_CHECK_EDIT_AND_START);
	pButton->SetCheck(0);
	pButton = (CButton *)GetDlgItem(IDC_RADIO1);
 	pButton->SetCheck(1);
	switch(m_buttonstate) {
	case 1:
		break;
	case 2:
		SetUI();
		break;
	case 3:
		break;
	default:
		break;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStrategyDlg::SetUI()
{
 	CButton* pButton1 = (CButton *)GetDlgItem(IDC_RADIO1);
 	CButton* pButton2 = (CButton *)GetDlgItem(IDC_RADIO2);
 	m_bRadio = m_tStrategyUI.firstLeg;
 
 	if (m_bRadio == 1){
 		pButton1->SetCheck(1);
 		pButton2->SetCheck(0);}
 	else{ 
 		pButton1->SetCheck(0);
 		pButton2->SetCheck(1);}
 
 	if (m_tStrategyUI.type == 1)
 	{
 		m_bb2 = 1;
 		SetCheck2();
 	}
 	m_maxPostions = m_tStrategyUI.maxPostions;
 	if (m_tStrategyUI.type == 1 || m_tStrategyUI.type == 3)
 	{
 
 		m_dDiff1          = m_tStrategyUI.tStrategyPrarm[0].diff; 
 		m_nVolumeOversea  = m_tStrategyUI.tStrategyPrarm[0].overseaVolume;
 		m_nVolumeDomestic = m_tStrategyUI.tStrategyPrarm[0].domesticVolume;
 		m_nAddOversea     = m_tStrategyUI.tStrategyPrarm[0].overseaAdd;
 		m_nAddDomestic    = m_tStrategyUI.tStrategyPrarm[0].domesticAdd;
 		UpdateData(false);
 		if(m_tStrategyUI.tStrategyPrarm[0].fishflag == 1)
 			m_checkFish1.SetCheck(1);
 		else
 			m_checkFish1.SetCheck(0);
 		
 	}
 
 	if (m_tStrategyUI.type == 2 || m_tStrategyUI.type == 3)
 	{
 		m_dDiff2		   = m_tStrategyUI.tStrategyPrarm[1].diff; 
 		m_nVolumeOversea2  = m_tStrategyUI.tStrategyPrarm[1].overseaVolume;
 		m_nVolumeDomestic2 = m_tStrategyUI.tStrategyPrarm[1].domesticVolume;
 		m_nAddOversea2     = m_tStrategyUI.tStrategyPrarm[1].overseaAdd;
 		m_nAddDomestic2    = m_tStrategyUI.tStrategyPrarm[1].domesticAdd;
 		UpdateData(false);
 		if(m_tStrategyUI.tStrategyPrarm[1].fishflag == 1)
 			m_checkFish2.SetCheck(1);
 		else
 			m_checkFish2.SetCheck(0);
 	}
 	
 	if (m_tStrategyUI.type == 1 || m_tStrategyUI.type == 3)
 	{
 		if(m_tStrategyUI.tStrategyPrarm[0].compare == ">=")
 			m_BCompare.SetCurSel(0);
 		else
 			m_BCompare.SetCurSel(1);
 		if(m_tStrategyUI.tStrategyPrarm[0].overseaBuySell == BUY)
 			m_BSOversea.SetCurSel(0);
 		else
 			m_BSOversea.SetCurSel(1);
 		if(m_tStrategyUI.tStrategyPrarm[0].domesticBuySell == BUY)
 			m_BSDomestic.SetCurSel(0);
 		else
 			m_BSDomestic.SetCurSel(1);
 
 		if(m_tStrategyUI.tStrategyPrarm[0].overseaType == KAI_CANG)
 			m_Overseatype.SetCurSel(0);
 		else if (m_tStrategyUI.tStrategyPrarm[0].overseaType == PING_JIN)
 			m_Overseatype.SetCurSel(1);
 		else//PC
 			m_Overseatype.SetCurSel(2);
 
 		if(m_tStrategyUI.tStrategyPrarm[0].domesticType == KAI_CANG)
 			m_DomesticType.SetCurSel(0);
 		else if (m_tStrategyUI.tStrategyPrarm[0].domesticType == PING_JIN)
 			m_DomesticType.SetCurSel(1);
 		else//PC
 			m_DomesticType.SetCurSel(2);
 
 		if(m_tStrategyUI.tStrategyPrarm[0].fishflag == 1)
 			m_checkFish1.SetCheck(1);
 		else
 			m_checkFish1.SetCheck(0);
 		if(m_tStrategyUI.tStrategyPrarm[1].fishflag == 1)
 			m_checkFish2.SetCheck(1);
 		else
 			m_checkFish2.SetCheck(0);
 	}
 	
 	if (m_tStrategyUI.type == 2 || m_tStrategyUI.type == 3)
 	{
 		if(m_tStrategyUI.tStrategyPrarm[1].compare == ">=")
 			m_BCompare2.SetCurSel(0);
 		else
 			m_BCompare2.SetCurSel(1);
 		if(m_tStrategyUI.tStrategyPrarm[1].overseaBuySell == BUY)
 			m_BSOversea2.SetCurSel(0);
 		else
 			m_BSOversea2.SetCurSel(1);
 		if(m_tStrategyUI.tStrategyPrarm[1].domesticBuySell == BUY)
 			m_BSDomestic2.SetCurSel(0);
 		else
 			m_BSDomestic2.SetCurSel(1);
 		
 		if(m_tStrategyUI.tStrategyPrarm[1].overseaType == KAI_CANG)
 			m_Overseatype2.SetCurSel(0);
 		else if (m_tStrategyUI.tStrategyPrarm[1].overseaType == PING_JIN)
 			m_Overseatype2.SetCurSel(1);
 		else//PC
 			m_Overseatype2.SetCurSel(2);
 
 		if(m_tStrategyUI.tStrategyPrarm[1].domesticType == KAI_CANG)
 			m_DomesticType2.SetCurSel(0);
 		else if (m_tStrategyUI.tStrategyPrarm[1].domesticType == PING_JIN)
 			m_DomesticType2.SetCurSel(1);
 		else//PC
 			m_DomesticType2.SetCurSel(2);
 
 		if(m_tStrategyUI.tStrategyPrarm[0].fishflag == 1)
 			m_checkFish1.SetCheck(1);
 		else
 			m_checkFish1.SetCheck(0);
 		if(m_tStrategyUI.tStrategyPrarm[1].fishflag == 1)
 			m_checkFish2.SetCheck(1);
 		else
 			m_checkFish2.SetCheck(0);
 	}
 	
 	
}

void CStrategyDlg::OnCheck1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_bb1 == 0)
	{
		m_bb1 = 1;
		CButton* pButton = (CButton *)GetDlgItem(IDC_CHECK1);
		pButton->SetCheck(1);
		CComboBox * pComboBox= (CComboBox *)GetDlgItem(IDC_COMBO_COMPARE1);
		pComboBox->EnableWindow(TRUE);
		pComboBox= (CComboBox *)GetDlgItem(ID_COM_OVERSEAS);
		pComboBox->EnableWindow(TRUE);
		pComboBox= (CComboBox *)GetDlgItem(IDC_COM_DOMESTIC);
		pComboBox->EnableWindow(TRUE);
		pComboBox= (CComboBox *)GetDlgItem(ID_COM_OVERSEAS_TYPE);
		pComboBox->EnableWindow(TRUE);
		pComboBox= (CComboBox *)GetDlgItem(IDC_COM_DOMESTIC_TYPE);
		pComboBox->EnableWindow(TRUE);

		CEdit * pEdit =  (CEdit *)GetDlgItem(IDC_EDIT_DIFF1);
		pEdit->EnableWindow(TRUE);
		pEdit =  (CEdit *)GetDlgItem(IDC_EDIT_VOLUME_OVERSEA);
		pEdit->EnableWindow(TRUE);
		pEdit =  (CEdit *)GetDlgItem(IDC_EDIT_VOLUME_DOMESTIC);
		pEdit->EnableWindow(TRUE);
		pEdit =  (CEdit *)GetDlgItem(IDC_EDIT_ADD_OVERSEA);
		pEdit->EnableWindow(TRUE);
		pEdit =  (CEdit *)GetDlgItem(IDC_EDIT_ADD_DOMESTIC);
		pEdit->EnableWindow(TRUE);
	

	}
	else
	{
		m_bb1 = 0;
		CButton* pButton = (CButton *)GetDlgItem(IDC_CHECK1);
		pButton->SetCheck(0);

		CComboBox * pComboBox= (CComboBox *)GetDlgItem(IDC_COMBO_COMPARE1);
		pComboBox->EnableWindow(FALSE);
		pComboBox= (CComboBox *)GetDlgItem(ID_COM_OVERSEAS);
		pComboBox->EnableWindow(FALSE);
		pComboBox= (CComboBox *)GetDlgItem(IDC_COM_DOMESTIC);
		pComboBox->EnableWindow(FALSE);
		pComboBox= (CComboBox *)GetDlgItem(ID_COM_OVERSEAS_TYPE);
		pComboBox->EnableWindow(FALSE);
		pComboBox= (CComboBox *)GetDlgItem(IDC_COM_DOMESTIC_TYPE);
		pComboBox->EnableWindow(FALSE);

		CEdit * pEdit =  (CEdit *)GetDlgItem(IDC_EDIT_DIFF1);
		pEdit->EnableWindow(FALSE);
		pEdit =  (CEdit *)GetDlgItem(IDC_EDIT_VOLUME_OVERSEA);
		pEdit->EnableWindow(FALSE);
		pEdit =  (CEdit *)GetDlgItem(IDC_EDIT_VOLUME_DOMESTIC);
		pEdit->EnableWindow(FALSE);
		pEdit =  (CEdit *)GetDlgItem(IDC_EDIT_ADD_OVERSEA);
		pEdit->EnableWindow(FALSE);
		pEdit =  (CEdit *)GetDlgItem(IDC_EDIT_ADD_DOMESTIC);
		pEdit->EnableWindow(FALSE);
	}
}

void CStrategyDlg::OnCheck2() 
{
	SetCheck2() ;
}

void CStrategyDlg::SetCheck2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_bb2 == 0)
	{
		m_bb2 = 1;
		CButton* pButton = (CButton *)GetDlgItem(IDC_CHECK2);
		pButton->SetCheck(1);

		CComboBox * pComboBox= (CComboBox *)GetDlgItem(IDC_COMBO_COMPARE2);
		pComboBox->EnableWindow(TRUE);
		pComboBox= (CComboBox *)GetDlgItem(ID_COM_OVERSEAS2);
		pComboBox->EnableWindow(TRUE);
		pComboBox= (CComboBox *)GetDlgItem(IDC_COM_DOMESTIC2);
		pComboBox->EnableWindow(TRUE);
		pComboBox= (CComboBox *)GetDlgItem(ID_COM_OVERSEAS_TYPE2);
		pComboBox->EnableWindow(TRUE);
		pComboBox= (CComboBox *)GetDlgItem(IDC_COM_DOMESTIC_TYPE2);
		pComboBox->EnableWindow(TRUE);

		CEdit * pEdit =  (CEdit *)GetDlgItem(IDC_EDIT_DIFF2);
		pEdit->EnableWindow(TRUE);
		pEdit =  (CEdit *)GetDlgItem(IDC_EDIT_VOLUME_OVERSEA2);
		pEdit->EnableWindow(TRUE);
		pEdit =  (CEdit *)GetDlgItem(IDC_EDIT_VOLUME_DOMESTIC2);
		pEdit->EnableWindow(TRUE);
		pEdit =  (CEdit *)GetDlgItem(IDC_EDIT_ADD_OVERSEA2);
		pEdit->EnableWindow(TRUE);
		pEdit =  (CEdit *)GetDlgItem(IDC_EDIT_ADD_DOMESTIC2);
		pEdit->EnableWindow(TRUE);
	
	}
	else
	{
		m_bb2 = 0;
		CButton* pButton = (CButton *)GetDlgItem(IDC_CHECK2);
		pButton->SetCheck(0);

		CComboBox * pComboBox= (CComboBox *)GetDlgItem(IDC_COMBO_COMPARE2);
		pComboBox->EnableWindow(FALSE);
		pComboBox= (CComboBox *)GetDlgItem(ID_COM_OVERSEAS2);
		pComboBox->EnableWindow(FALSE);
		pComboBox= (CComboBox *)GetDlgItem(IDC_COM_DOMESTIC2);
		pComboBox->EnableWindow(FALSE);
		pComboBox= (CComboBox *)GetDlgItem(ID_COM_OVERSEAS_TYPE2);
		pComboBox->EnableWindow(FALSE);
		pComboBox= (CComboBox *)GetDlgItem(IDC_COM_DOMESTIC_TYPE2);
		pComboBox->EnableWindow(FALSE);

		CEdit * pEdit =  (CEdit *)GetDlgItem(IDC_EDIT_DIFF2);
		pEdit->EnableWindow(FALSE);
		pEdit =  (CEdit *)GetDlgItem(IDC_EDIT_VOLUME_OVERSEA2);
		pEdit->EnableWindow(FALSE);
		pEdit =  (CEdit *)GetDlgItem(IDC_EDIT_VOLUME_DOMESTIC2);
		pEdit->EnableWindow(FALSE);
		pEdit =  (CEdit *)GetDlgItem(IDC_EDIT_ADD_OVERSEA2);
		pEdit->EnableWindow(FALSE);
		pEdit =  (CEdit *)GetDlgItem(IDC_EDIT_ADD_DOMESTIC2);
		pEdit->EnableWindow(FALSE);
	}
}

void CStrategyDlg::OnCheckEditAndStart() 
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

void CStrategyDlg::OnSelchangeComOverseas() 
{
	// TODO: Add your control notification handler code here
	if (m_BSOversea.GetCurSel() == 0)
	{
		m_BSDomestic.SetCurSel(1);
		m_BSDomestic2.SetCurSel(0);
		m_BSOversea2.SetCurSel(1);
		m_BCompare.SetCurSel(1);
		m_BCompare2.SetCurSel(0);
	}
	else
	{
		m_BSDomestic.SetCurSel(0);
		m_BSDomestic2.SetCurSel(1);
		m_BSOversea2.SetCurSel(0);
	    m_BCompare.SetCurSel(0);
		m_BCompare2.SetCurSel(1);
	}
}

void CStrategyDlg::OnSelchangeComDomestic() 
{
	// TODO: Add your control notification handler code here
	if (m_BSDomestic.GetCurSel() == 0)
	{
		m_BSOversea.SetCurSel(1);
		m_BSDomestic2.SetCurSel(1);
		m_BSOversea2.SetCurSel(0);
		m_BCompare.SetCurSel(0);
		m_BCompare2.SetCurSel(1);
	}
	else
	{
		m_BSOversea.SetCurSel(0);
		m_BSDomestic2.SetCurSel(0);
		m_BSOversea2.SetCurSel(1);
	    m_BCompare.SetCurSel(1);
		m_BCompare2.SetCurSel(0);
	}
}

void CStrategyDlg::OnSelchangeComOverseasType() 
{
	// TODO: Add your control notification handler code here
	m_DomesticType.SetCurSel(m_Overseatype.GetCurSel());
}

void CStrategyDlg::OnSelchangeComOverseasType2() 
{
	// TODO: Add your control notification handler code here
	m_DomesticType2.SetCurSel(m_Overseatype2.GetCurSel());
}

void CStrategyDlg::OnSelchangeComOverseas2() 
{
	// TODO: Add your control notification handler code here
	if (m_BSOversea2.GetCurSel() == 0)
	{
		m_BSDomestic2.SetCurSel(1);
		m_BSDomestic.SetCurSel(0);
		m_BSOversea.SetCurSel(1);
		m_BCompare.SetCurSel(0);
		m_BCompare2.SetCurSel(1);
	}
	else
	{
		m_BSDomestic2.SetCurSel(0);
		m_BSDomestic.SetCurSel(1);
		m_BSOversea.SetCurSel(0);
		m_BCompare.SetCurSel(1);
		m_BCompare2.SetCurSel(0);
	}
}

void CStrategyDlg::OnSelchangeComDomesticType() 
{
	// TODO: Add your control notification handler code here

}

void CStrategyDlg::OnSelchangeComDomesticType2() 
{
	// TODO: Add your control notification handler code here
}

void CStrategyDlg::OnSelchangeComDomestic2() 
{
	// TODO: Add your control notification handler code here
		if (m_BSDomestic2.GetCurSel() == 0)
	{
		m_BSOversea2.SetCurSel(1);
		m_BSDomestic.SetCurSel(1);
		m_BSOversea.SetCurSel(0);
		m_BCompare.SetCurSel(1);
		m_BCompare2.SetCurSel(0);
	}
	else
	{
		m_BSOversea2.SetCurSel(0);
		m_BSDomestic.SetCurSel(0);
		m_BSOversea.SetCurSel(1);
		m_BCompare.SetCurSel(0);
		m_BCompare2.SetCurSel(1);
	}
}

void CStrategyDlg::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	CButton* pButton = (CButton *)GetDlgItem(IDC_RADIO2);
	pButton->SetCheck(0);
	m_bRadio = 1;
}

void CStrategyDlg::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	CButton* pButton = (CButton *)GetDlgItem(IDC_RADIO1);
	pButton->SetCheck(0);
	m_bRadio = 2;
}


void CStrategyDlg::OnCbnSelchangeComboCompare1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_BCompare.GetCurSel() == 0)
	{
		this->m_BCompare2.SetCurSel(1);
		m_BSOversea.SetCurSel(1);
        OnSelchangeComOverseas() ;

	}
	else
	{
		this->m_BCompare2.SetCurSel(0);
		m_BSOversea.SetCurSel(0);
        OnSelchangeComOverseas() ;
	}
}


void CStrategyDlg::OnCbnSelchangeComboCompare2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_BCompare2.GetCurSel() == 0)
	{
		this->m_BCompare.SetCurSel(1);
		m_BSOversea2.SetCurSel(1);
        OnSelchangeComOverseas2() ;
	}
	else
	{
		this->m_BCompare.SetCurSel(0);
		m_BSOversea2.SetCurSel(0);
        OnSelchangeComOverseas2() ;
	}
}


void CStrategyDlg::OnBnClickedCheckFish1()
{
	// TODO: 在此添加控件通知处理程序代码
	;
}


void CStrategyDlg::OnBnClickedCheckFish2()
{
	// TODO: 在此添加控件通知处理程序代码
	;
}


void CStrategyDlg::OnBnClickedButtonTurnOver()
{
	TStrategyUI tStrategyUI;
	tStrategyUI = m_tStrategyUI;

	m_tStrategyUI.tStrategyPrarm[1] = tStrategyUI.tStrategyPrarm[0];
	m_tStrategyUI.tStrategyPrarm[0] = tStrategyUI.tStrategyPrarm[1];

	SetUI();
}
