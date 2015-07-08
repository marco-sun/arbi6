// IntentionPairDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Arbi5UI.h"
#include "IntentionPairDlg.h"
#include "StrategyDlg.h"
#include "StrategyMoreDlg.h"
#include "IntentionDlg.h"
#include <assert.h>
#include "MainFrm.h"
#include "DataCurve.h"
//*****business
#include "..\TwoLegStrategy.h"
#include "..\IntentionPair.h"
#include "..\Rate.h"
#include "..\LogStore.h"
#include "..\Persistence.h"
#include "..\DayTradingStrategy.h"

#include "..\PriceSource.h"
//**************
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMainFrame * gpMainFrame;
/////////////////////////////////////////////////////////////////////////////
// CIntentionPairDlg dialog
#define IMPOSSIBLE_DIFF (-1000000)

IMPLEMENT_DYNCREATE(CIntentionPairDlg, CDialog)
CIntentionPairDlg::CIntentionPairDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIntentionPairDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIntentionPairDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	InitializeCriticalSection(&csui);
	m_runflag = TRUE;
	m_sortflag = 0;
}

CIntentionPairDlg::~CIntentionPairDlg()
{
	DeleteCriticalSection(&csui);
	m_runflag = FALSE;
}
void CIntentionPairDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIntentionPairDlg)
	DDX_Control(pDX, IDC_LIST_INTENTION_PAIR, m_listIntentionPair);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIntentionPairDlg, CDialog)
	//{{AFX_MSG_MAP(CIntentionPairDlg)
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_INTENTION_PAIR, OnRclickListIntentionPair)
	ON_COMMAND(ID_INTENTION_ADD, OnAddItem)
	ON_COMMAND(ID_ADD_STRATEGY, OnAddStrategy)
	ON_COMMAND(ID__32803, &CIntentionPairDlg::OnAddMoreStrategy)
	ON_NOTIFY(NM_CLICK, IDC_LIST_INTENTION_PAIR, OnClickListIntentionPair)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_INTENTION_PAIR, OnColumnclickListSp)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_INTENTION_PAIR, OnGetdispinfoListIntentionPair)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_DATA_CURVE, &CIntentionPairDlg::OnDataCurve)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_INTENTION_PAIR, &CIntentionPairDlg::OnDblclkListIntentionPair)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_INTENTION_PAIR, &CIntentionPairDlg::OnLvnItemchangedListIntentionPair)
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIntentionPairDlg message handlers
void IntentionPairChange( void* pParams )
{ 
    CIntentionPairDlg *pIntentionPairDlg = (CIntentionPairDlg *) pParams;
	if (pIntentionPairDlg == NULL) {
		TRACE_LOG(" err: pIntentionPairDlg is NULL");
		return;
	}
//	CSingleLock   lock(&pIntentionPairDlg->m_mutex); 
	while(pIntentionPairDlg->m_runflag == TRUE)
	{
		list<IntentionPair *> listIntenPair;
		if(pIntentionPairDlg->m_sortflag == 0)
			listIntenPair = IntentionPair::listAll();
		else if(pIntentionPairDlg->m_sortflag == 1)
			listIntenPair = IntentionPair::sortListAll(DOWN);
		else if(pIntentionPairDlg->m_sortflag == 2)
			listIntenPair = IntentionPair::sortListAll(UP);

		list<IntentionPair*> ::iterator it ;
		int rushfalg = 0;
		for(it = listIntenPair.begin(); it != listIntenPair.end(); ++it)// list
		{
			if(TRUE == pIntentionPairDlg->UpdateIntentionPairUI((*it)))
				rushfalg = 1;
		}
//		if (1 == rushfalg)
//			pIntentionPairDlg->m_listIntentionPair.SetItemCount( pIntentionPairDlg->m_listIPUI.size());
		Sleep(500);
        //Sleep(50); 
	}
	
	_endthread();
}
BOOL CIntentionPairDlg::UpdateIntentionPairUI( IntentionPair* pIp)
{
	BOOL ret = FALSE; 
	int findflag = 0;
	vector<TIPUI>::iterator iter;
	CString strtemp;
	
	char sztemp[64];
	int index = 0;
	
	for(iter = m_listIPUI.begin(); iter != m_listIPUI.end(); ++iter)//query list
	{
		if ((*iter).sName == pIp->getName())
		{
			findflag = 1;

			memset(sztemp, 0, sizeof(sztemp));
		    sprintf_s(sztemp,64, "%.2f",pIp->importProfit());
			if ((*iter).fistbuy.compare(sztemp) !=0)
			{
				m_listIntentionPair.SetItem(index,1,LVIF_TEXT,sztemp, 0, 0, 0,0);
				(*iter).fistbuy = sztemp;
				ret = TRUE;
			}

			memset(sztemp, 0, sizeof(sztemp));
		    sprintf_s(sztemp,64, "%g",pIp->getFirstBid());
			if ((*iter).fistbuy.compare(sztemp) !=0)
			{
				m_listIntentionPair.SetItem(index,2,LVIF_TEXT,sztemp, 0, 0, 0,0);
				(*iter).fistbuy = sztemp;
				ret = TRUE;
			}
			
			memset(sztemp, 0, sizeof(sztemp));
		    sprintf_s(sztemp,64, "%g",pIp->getFirstAsk());
			if ((*iter).fistsell.compare(sztemp) !=0)
			{
				m_listIntentionPair.SetItem(index,3,LVIF_TEXT,sztemp, 0, 0, 0,0);
				(*iter).fistsell = sztemp;
				ret = TRUE;
			}

			memset(sztemp, 0, sizeof(sztemp));
		    sprintf_s(sztemp,64, "%g",pIp->getSecondBid());
			if ((*iter).secondbuy.compare(sztemp) !=0)
			{
				m_listIntentionPair.SetItem(index,9,LVIF_TEXT,sztemp, 0, 0, 0,0);
				(*iter).secondbuy = sztemp;
				ret = TRUE;
			}
			
			memset(sztemp, 0, sizeof(sztemp));
		    sprintf_s(sztemp,64, "%g",pIp->getSecondAsk());
			if ((*iter).secondsell.compare(sztemp) !=0)
			{
				m_listIntentionPair.SetItem(index,10,LVIF_TEXT,sztemp, 0, 0, 0,0);
				(*iter).secondsell = sztemp;
				ret = TRUE;
			}
			if (ret != TRUE)
				break;
			
			memset(sztemp, 0, sizeof(sztemp));
//		    sprintf(sztemp, "%.2f",pIp->getFirstDiff());
			sprintf_s(sztemp,64, "%g",pIp->getSecondDiff());
			m_listIntentionPair.SetItem(index,4,LVIF_TEXT,sztemp, 0, 0, 0,0);
			(*iter).buydiff = sztemp;

			memset(sztemp, 0, sizeof(sztemp));
		    sprintf_s(sztemp,64, "%g",pIp->getFirstRatio());
			m_listIntentionPair.SetItem(index,7,LVIF_TEXT,sztemp, 0, 0, 0,0);
			(*iter).kc = sztemp;

			memset(sztemp, 0, sizeof(sztemp));
		    sprintf_s(sztemp,64, "%g",pIp->getlastPriceDiff());
			m_listIntentionPair.SetItem(index,8,LVIF_TEXT,sztemp, 0, 0, 0,0);
			(*iter).lastPriceDiff = sztemp;

			memset(sztemp, 0, sizeof(sztemp));
//		    sprintf(sztemp, "%.2f",pIp->getSecondDiff());
			sprintf_s(sztemp,64, "%g",pIp->getFirstDiff());
			m_listIntentionPair.SetItem(index,6,LVIF_TEXT,sztemp, 0, 0, 0,0);
			(*iter).selldiff = sztemp;

			memset(sztemp, 0, sizeof(sztemp));
		    sprintf_s(sztemp,64, "%g",pIp->getSecondRatio());
			m_listIntentionPair.SetItem(index,5,LVIF_TEXT,sztemp, 0, 0, 0,0);
			(*iter).pc = sztemp;

			if (pIp->getFirstDiff() > (IMPOSSIBLE_DIFF+1))
			{ // 有效价差
				double dtemp = atof((*iter).buyMaxdiff.c_str());
				if ((pIp->getFirstDiff() >= dtemp)
					|| (dtemp > 999999)){
					memset(sztemp, 0, sizeof(sztemp));
					sprintf_s(sztemp,64, "%g",pIp->getFirstDiff());
					(*iter).buyMaxdiff = sztemp;
				}
			}
			m_listIntentionPair.SetItem(index,13,LVIF_TEXT,(*iter).buyMaxdiff.c_str(), 0, 0, 0,0);
			
			if (pIp->getFirstDiff() > (IMPOSSIBLE_DIFF+1))
			{ // 有效价差
				double dtemp = atof((*iter).buyMindiff.c_str());
				if ((pIp->getFirstDiff() <= dtemp)
					||  dtemp < -999999){
					memset(sztemp, 0, sizeof(sztemp));
					sprintf_s(sztemp,64, "%g",pIp->getFirstDiff());
					(*iter).buyMindiff = sztemp;
				}
			}
			m_listIntentionPair.SetItem(index,14,LVIF_TEXT,(*iter).buyMindiff.c_str(), 0, 0, 0,0);
			
			if (pIp->getSecondDiff() > (IMPOSSIBLE_DIFF+1))
			{ // 有效价差
				double dtemp = atof((*iter).sellMaxdiff.c_str());
				if ((pIp->getSecondDiff() >= dtemp)
					|| (dtemp > 999999)){
					memset(sztemp, 0, sizeof(sztemp));
					sprintf_s(sztemp,64, "%g",pIp->getSecondDiff());
					(*iter).sellMaxdiff = sztemp;
				}
			}
			m_listIntentionPair.SetItem(index,11,LVIF_TEXT,(*iter).sellMaxdiff.c_str(), 0, 0, 0,0);
			
			if (pIp->getSecondDiff() > (IMPOSSIBLE_DIFF+1))
			{ // 有效价差
				double dtemp = atof((*iter).sellMindiff.c_str());
				if ((pIp->getSecondDiff() <= dtemp)
					||  dtemp < -999999){
					memset(sztemp, 0, sizeof(sztemp));
					sprintf_s(sztemp,64, "%g",pIp->getSecondDiff());
					(*iter).sellMindiff = sztemp;
				}
			}
			m_listIntentionPair.SetItem(index,12,LVIF_TEXT,(*iter).sellMindiff.c_str(), 0, 0, 0,0);
			
			break;
		}
		index++;
	}
	if (findflag != 1)
	{
		TRACE_LOG("Find UnMatch IntentionPair name (%s)", pIp->getName().c_str());
	}
		
	return ret;
}
BOOL CIntentionPairDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CMainFrame * pMainFrame = NULL;
	pMainFrame = ((CMainFrame*)::AfxGetMainWnd());

	m_listIntentionPair.SetBkColor(RGB(0,0,0));        //设置背景色
	m_listIntentionPair.SetRowHeigt(25);               //设置行高度
	m_listIntentionPair.SetHeaderHeight(1.5);          //设置头部高度
	m_listIntentionPair.SetHeaderFontHW(18,0);         //设置头部字体高度,和宽度,0表示缺省，自适应 
	m_listIntentionPair.SetHeaderTextColor(RGB(255,255,255)); //设置头部字体颜色
	m_listIntentionPair.SetTextColor(RGB(255,0,0));  //设置文本颜色
	m_listIntentionPair.SetFontHW(18,0);               //设置字体高度，和宽度,0表示缺省宽度
	m_listIntentionPair.SetColTextColor(4,RGB(255,255,0)); //设置列文本颜色
	m_listIntentionPair.SetColTextColor(6,RGB(255,255,0)); //设置列背景色
	m_listIntentionPair.SetColTextColor(8,RGB(255,255,0)); //设置列背景色


   //设置字体高度，和宽度,0表示缺省宽度

	int   nFullWidth=GetSystemMetrics(SM_CXSCREEN); 
	int base = (nFullWidth - 25) /15;
	m_listIntentionPair.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listIntentionPair.InsertColumn(0, "名称", LVCFMT_CENTER, base*2);
	m_listIntentionPair.InsertColumn(1, "进口盈亏", LVCFMT_CENTER, base);
	m_listIntentionPair.InsertColumn(2, "合约1买价", LVCFMT_CENTER, base);
	m_listIntentionPair.InsertColumn(3, "合约1卖价", LVCFMT_CENTER, base);
	m_listIntentionPair.InsertColumn(4, "买差价", LVCFMT_CENTER, base);
	m_listIntentionPair.InsertColumn(5, "开仓比价", LVCFMT_CENTER, base);
	m_listIntentionPair.InsertColumn(6, "抛差价", LVCFMT_CENTER, base);
	m_listIntentionPair.InsertColumn(7, "平仓比价", LVCFMT_CENTER, base);
	m_listIntentionPair.InsertColumn(8, "最新差价", LVCFMT_CENTER, base);
	m_listIntentionPair.InsertColumn(9, "合约2买价", LVCFMT_CENTER, base);
	m_listIntentionPair.InsertColumn(10, "合约2卖价", LVCFMT_CENTER, base);
	m_listIntentionPair.InsertColumn(11, "买最高差价", LVCFMT_CENTER, base);
	m_listIntentionPair.InsertColumn(12, "买最低差价", LVCFMT_CENTER, base);
	m_listIntentionPair.InsertColumn(13, "卖最高差价", LVCFMT_CENTER, base);
	m_listIntentionPair.InsertColumn(14, "卖最低差价", LVCFMT_CENTER, base);


	//TODO: comment this for real env.
	//PriceSource::useMockPriceSource();
	//Dealer::useRealDealer = false;

	InitIntentionPairTwoLegStrategy();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CIntentionPairDlg::UpdateInitIntentionPair(list<IntentionPair *> listIntenPair)
{
	list<IntentionPair*> ::iterator it ;
	m_listIntentionPair.DeleteAllItems();
	m_listIPUI.clear();
	int index = 0;
	for(it = listIntenPair.begin(); it != listIntenPair.end(); ++it)// list
	{
		m_listIntentionPair.InsertItem(index,(*it)->getName().c_str());
		TIPUI ttIPUI;
		ttIPUI.buyMaxdiff = "-999999";
		ttIPUI.buyMindiff = "999999";
		ttIPUI.sellMaxdiff = "-999999";
		ttIPUI.sellMindiff = "999999";
		ttIPUI.sName = (*it)->getName();
		m_listIPUI.push_back(ttIPUI);
		index++;
	}
}

void CIntentionPairDlg::InitIntentionPair(list<IntentionPair *> listIntenPair)
{
	list<IntentionPair*> ::iterator it ;
	for(it = listIntenPair.begin(); it != listIntenPair.end(); ++it)// list
	{
		TIPUI ttIPUI;
		ttIPUI.buyMaxdiff = "-999999";
		ttIPUI.buyMindiff = "999999";
		ttIPUI.sellMaxdiff = "-999999";
		ttIPUI.sellMindiff = "999999";
		ttIPUI.sName = (*it)->getName();
		m_listIPUI.push_back(ttIPUI);
		TIPInfo *ptIPInfo = new TIPInfo;
		ptIPInfo->sName = (*it)->getName();
		ptIPInfo->firstname = (*it)->getContractA();
		ptIPInfo->secondname = (*it)->getContractB();
		ptIPInfo->firstVol = (*it)->getLotA();
		ptIPInfo->secondVol = (*it)->getLotB();
		
		gpMainFrame->m_IPInfoMap.insert(IPInfoMap::value_type(ptIPInfo->sName, ptIPInfo));
		int index = gpMainFrame->m_IPInfoMap.size() - 1;
		m_listIntentionPair.InsertItem(index,ttIPUI.sName.c_str());
	}
	//Sleep(500);
}
void CIntentionPairDlg::InitIntentionPairTwoLegStrategy()
{
	InitIntentionPair(IntentionPair::listAll());
	list<DayTradingStrategy*> lts = DayTradingStrategy::listAll();
	list<DayTradingStrategy*> ::iterator itt ;
	for(itt = lts.begin(); itt != lts.end(); ++itt)// list
	{
		SetTwoLegStrategy((*itt));
	}
	_beginthread(IntentionPairChange, 0, (void *)this);
	SetTimer(1,500,NULL);
}

void CIntentionPairDlg::SetTwoLegStrategy(DayTradingStrategy* pDTS)
{
	map<string, TIPInfo*>::iterator iter = gpMainFrame->m_IPInfoMap.begin();
    while( iter != gpMainFrame->m_IPInfoMap.end() ) {
        CString strmap = iter->second->sName.c_str();
		CString strtw  = pDTS->getId().c_str();
		strmap += "_";
        if (strtw.Find(strmap) >= 0)
        {
			TStrategyInfo *ptStrategyInfo = new TStrategyInfo;
			ptStrategyInfo->dts = pDTS;
//			ptStrategyInfo->tStrategyUI.firstLeg = 1;
			string sContractname = pDTS->getStrategyA()->getLegA().getContract();
			ptStrategyInfo->tStrategyUI.fistname = iter->second->firstname;//pDTS->getStrategyA()->getLegA().getContract();
			ptStrategyInfo->tStrategyUI.secondname = iter->second->secondname;// pDTS->getStrategyA()->getLegB().getContract();
			
			if(iter->second->firstname == sContractname)
				ptStrategyInfo->tStrategyUI.firstLeg = 1;
			else
				ptStrategyInfo->tStrategyUI.firstLeg = 2;

			ptStrategyInfo->tStrategyUI.maxPostions = pDTS->getHighHoldingLimit();
			strcpy_s(ptStrategyInfo->tStrategyUI.sName,32, pDTS->getId().c_str());
			if (ptStrategyInfo->dts->getStrategyA() != NULL)
			{
				if(ptStrategyInfo->dts->getStrategyA()->isFished())
					ptStrategyInfo->tStrategyUI.tStrategyPrarm[0].fishflag = 1;
				else
					ptStrategyInfo->tStrategyUI.tStrategyPrarm[0].fishflag = 0;
				ptStrategyInfo->tStrategyUI.type = 1;
				ptStrategyInfo->tStrategyUI.tStrategyPrarm[0].compare         = pDTS->getStrategyA()->getCondition().getComparator();
				ptStrategyInfo->tStrategyUI.tStrategyPrarm[0].diff            = pDTS->getStrategyA()->getCondition().getDiff() ;
				if(ptStrategyInfo->tStrategyUI.firstLeg == 1){
					ptStrategyInfo->tStrategyUI.tStrategyPrarm[0].domesticAdd     = pDTS->getStrategyA()->getLegB().getOffSet(); 
					ptStrategyInfo->tStrategyUI.tStrategyPrarm[0].domesticBuySell = pDTS->getStrategyA()->getLegB().getBuySell();
					ptStrategyInfo->tStrategyUI.tStrategyPrarm[0].domesticType    = pDTS->getStrategyA()->getLegB().getKaiPing();
					ptStrategyInfo->tStrategyUI.tStrategyPrarm[0].domesticVolume  = pDTS->getStrategyA()->getLegB().getLot();
					ptStrategyInfo->tStrategyUI.tStrategyPrarm[0].overseaAdd      = pDTS->getStrategyA()->getLegA().getOffSet();
					ptStrategyInfo->tStrategyUI.tStrategyPrarm[0].overseaBuySell  = pDTS->getStrategyA()->getLegA().getBuySell();;
					ptStrategyInfo->tStrategyUI.tStrategyPrarm[0].overseaType     = pDTS->getStrategyA()->getLegA().getKaiPing();
					ptStrategyInfo->tStrategyUI.tStrategyPrarm[0].overseaVolume   = pDTS->getStrategyA()->getLegA().getLot();
				}else{
					ptStrategyInfo->tStrategyUI.tStrategyPrarm[0].domesticAdd     = pDTS->getStrategyA()->getLegA().getOffSet(); 
					ptStrategyInfo->tStrategyUI.tStrategyPrarm[0].domesticBuySell = pDTS->getStrategyA()->getLegA().getBuySell();
					ptStrategyInfo->tStrategyUI.tStrategyPrarm[0].domesticType    = pDTS->getStrategyA()->getLegA().getKaiPing();
					ptStrategyInfo->tStrategyUI.tStrategyPrarm[0].domesticVolume  = pDTS->getStrategyA()->getLegA().getLot();
					ptStrategyInfo->tStrategyUI.tStrategyPrarm[0].overseaAdd      = pDTS->getStrategyA()->getLegB().getOffSet();
					ptStrategyInfo->tStrategyUI.tStrategyPrarm[0].overseaBuySell  = pDTS->getStrategyA()->getLegB().getBuySell();;
					ptStrategyInfo->tStrategyUI.tStrategyPrarm[0].overseaType     = pDTS->getStrategyA()->getLegB().getKaiPing();
					ptStrategyInfo->tStrategyUI.tStrategyPrarm[0].overseaVolume   = pDTS->getStrategyA()->getLegB().getLot();
				}
			}
			if (ptStrategyInfo->dts->getStrategyB() != NULL)
			{
				if(ptStrategyInfo->dts->getStrategyB()->isFished())
					ptStrategyInfo->tStrategyUI.tStrategyPrarm[1].fishflag = 1;
				else
					ptStrategyInfo->tStrategyUI.tStrategyPrarm[1].fishflag = 0;
				ptStrategyInfo->tStrategyUI.type = 3;
				ptStrategyInfo->tStrategyUI.tStrategyPrarm[1].compare         = pDTS->getStrategyB()->getCondition().getComparator();
				ptStrategyInfo->tStrategyUI.tStrategyPrarm[1].diff            = pDTS->getStrategyB()->getCondition().getDiff() ;
				if(ptStrategyInfo->tStrategyUI.firstLeg == 1){
					ptStrategyInfo->tStrategyUI.tStrategyPrarm[1].domesticAdd     = pDTS->getStrategyB()->getLegB().getOffSet(); 
					ptStrategyInfo->tStrategyUI.tStrategyPrarm[1].domesticBuySell = pDTS->getStrategyB()->getLegB().getBuySell();
					ptStrategyInfo->tStrategyUI.tStrategyPrarm[1].domesticType    = pDTS->getStrategyB()->getLegB().getKaiPing();
					ptStrategyInfo->tStrategyUI.tStrategyPrarm[1].domesticVolume  = pDTS->getStrategyB()->getLegB().getLot();
					ptStrategyInfo->tStrategyUI.tStrategyPrarm[1].overseaAdd      = pDTS->getStrategyB()->getLegA().getOffSet();
					ptStrategyInfo->tStrategyUI.tStrategyPrarm[1].overseaBuySell  = pDTS->getStrategyB()->getLegA().getBuySell();;
					ptStrategyInfo->tStrategyUI.tStrategyPrarm[1].overseaType     = pDTS->getStrategyB()->getLegA().getKaiPing();
					ptStrategyInfo->tStrategyUI.tStrategyPrarm[1].overseaVolume   = pDTS->getStrategyB()->getLegA().getLot();
				}else{
					ptStrategyInfo->tStrategyUI.tStrategyPrarm[1].domesticAdd     = pDTS->getStrategyB()->getLegA().getOffSet(); 
					ptStrategyInfo->tStrategyUI.tStrategyPrarm[1].domesticBuySell = pDTS->getStrategyB()->getLegA().getBuySell();
					ptStrategyInfo->tStrategyUI.tStrategyPrarm[1].domesticType    = pDTS->getStrategyB()->getLegA().getKaiPing();
					ptStrategyInfo->tStrategyUI.tStrategyPrarm[1].domesticVolume  = pDTS->getStrategyB()->getLegA().getLot();
					ptStrategyInfo->tStrategyUI.tStrategyPrarm[1].overseaAdd      = pDTS->getStrategyB()->getLegB().getOffSet();
					ptStrategyInfo->tStrategyUI.tStrategyPrarm[1].overseaBuySell  = pDTS->getStrategyB()->getLegB().getBuySell();;
					ptStrategyInfo->tStrategyUI.tStrategyPrarm[1].overseaType     = pDTS->getStrategyB()->getLegB().getKaiPing();
					ptStrategyInfo->tStrategyUI.tStrategyPrarm[1].overseaVolume   = pDTS->getStrategyB()->getLegB().getLot();
				}
			}
			
			ptStrategyInfo->tStrategyUI.sStrategy = pDTS->toDesc();// pDTS->toString();
			ptStrategyInfo->tStrategyUI.sgstate = STOP;
			iter->second->strategyInfoList.push_back(ptStrategyInfo);
			break;
        }
        iter++;
	}
}

void CIntentionPairDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	if (GetSafeHwnd())
	{
		if (m_listIntentionPair.GetSafeHwnd())
		{
			CRect rect(0,0,cx,cy);
			m_listIntentionPair.MoveWindow(&rect);
		}
	}
}

void CIntentionPairDlg::OnAddItem() 
{
	CMainFrame * pMainFrame = NULL;
	pMainFrame = gpMainFrame;//((CMainFrame*)::AfxGetMainWnd());
	CIntentionDlg intentionDlg;
	
	intentionDlg.m_state = 1;
	if(intentionDlg.DoModal()==IDOK)
	{
		switch(intentionDlg.m_state) {
		case 1:
			{//增加了新策略

				string sname;
				string sfirst;
				string ssecond;
				string sdiffexp;
				int lotA;
				int lotB;

				sname   = intentionDlg.m_strName.GetBuffer(intentionDlg.m_strName.GetLength());
				sfirst  = intentionDlg.m_strFirstContract.GetBuffer(intentionDlg.m_strFirstContract.GetLength());
				ssecond = intentionDlg.m_strSecondContract.GetBuffer(intentionDlg.m_strSecondContract.GetLength());
				sdiffexp   = sfirst +".ask - "+ssecond+".bid";

				lotA = intentionDlg.m_firstVol;
				lotB = intentionDlg.m_secondVol;
				
				//IntentionPair::create("ru-JRU", "ru1201", "JRUZ1", "ru1201 . ask - JRUZ1 . bid");
				IntentionPair::create(sname, sfirst, ssecond, sdiffexp,  lotA,  lotB);
				TIPUI ttIPUI;
				ttIPUI.sName = sname;
				ttIPUI.buyMaxdiff = "-999999";
				ttIPUI.buyMindiff = "999999";
				ttIPUI.sellMaxdiff = "-999999";
				ttIPUI.sellMindiff = "999999";
				m_listIPUI.push_back(ttIPUI);
				//int count = m_listIntentionPair.GetItemCount();
				//m_listIntentionPair.InsertItem(count, intentionDlg.m_strName);
//				TIPState *ptIPState = new TIPState;
//				ptIPState->sName = intentionDlg.m_strName;
//				ptIPState->state = 1;
//				m_mapIPState.insert(IPStateMap::value_type(ptIPState->sName, ptIPState));

				TIPInfo *ptIPInfo = new TIPInfo;
				ptIPInfo->sName = intentionDlg.m_strName;
				ptIPInfo->firstname = intentionDlg.m_strFirstContract;
				ptIPInfo->secondname = intentionDlg.m_strSecondContract;
				ptIPInfo->firstVol = intentionDlg.m_firstVol;
				ptIPInfo->secondVol = intentionDlg.m_secondVol;
			
				pMainFrame->m_IPInfoMap.insert(IPInfoMap::value_type(ptIPInfo->sName, ptIPInfo));
				int index = pMainFrame->m_IPInfoMap.size() - 1;
				m_listIntentionPair.InsertItem(index,ttIPUI.sName.c_str());
				
			break;
			}
		default:
			break;
		} 
	}
}

void CIntentionPairDlg::OnAddStrategy()
{
	CMainFrame * pMainFrame = gpMainFrame;
	CStrategyDlg strategyDlg;
	char szname[64];
	memset(szname, 0, sizeof(szname));
	if(m_nRSelItem == -1) return; 
	m_listIntentionPair.GetItemText(m_nRSelItem, 0 ,szname , 64);
	if (strlen(szname) == 0) return;
	//找到Intentionpair结构TIPInfo
	IPInfoMap::iterator iter = pMainFrame->m_IPInfoMap.find(szname);
	assert( iter != pMainFrame->m_IPInfoMap.end());
	TIPInfo *pIPInfo = iter->second;
	strategyDlg.m_strName = szname;
	strategyDlg.m_strContractOversea = pIPInfo->firstname.c_str();
	strategyDlg.m_strContractDomestic = pIPInfo->secondname.c_str();
	strategyDlg.m_nVolumeOversea = strategyDlg.m_nVolumeOversea2 = pIPInfo->firstVol;
	strategyDlg.m_nVolumeDomestic = strategyDlg.m_nVolumeDomestic2 = pIPInfo->secondVol;


	if(IDOK == strategyDlg.DoModal())
	{
		CString strStrategyKeyName;
		char stime[32];
		memset(stime, 0, sizeof(stime));
		pMainFrame->GetCurTime(stime);
		strStrategyKeyName.Format("%s_%s",szname,stime);
		
		strcpy_s(strategyDlg.m_tStrategyUI.sName,32, strStrategyKeyName.GetBuffer(strStrategyKeyName.GetLength()));

		if(strategyDlg.m_nOP == 1)
			strategyDlg.m_tStrategyUI.sgstate = WORK;
		else
			strategyDlg.m_tStrategyUI.sgstate = STOP;

		strategyDlg.m_tStrategyUI.firstLeg = strategyDlg.m_bRadio;

		DayTradingStrategy * dts = DayTradingStrategy::create(strStrategyKeyName.GetBuffer(strStrategyKeyName.GetLength()));
		
        if(strategyDlg.m_tStrategyUI.type == 1 || strategyDlg.m_tStrategyUI.type == 3)
		{
			
			if (strategyDlg.m_bRadio == 1)
			{
				TwoLegStrategy * ts1 = dts->createStrategyA();
				ts1->setLegA(new Leg("default", strategyDlg.m_tStrategyUI.tStrategyPrarm[0].overseaBuySell, 
					                 strategyDlg.m_tStrategyUI.tStrategyPrarm[0].overseaType, 
									 strategyDlg.m_tStrategyUI.fistname, 
									 strategyDlg.m_tStrategyUI.tStrategyPrarm[0].overseaVolume, 
									 strategyDlg.m_tStrategyUI.tStrategyPrarm[0].overseaAdd));
				
				ts1->setLegB(new Leg("default", strategyDlg.m_tStrategyUI.tStrategyPrarm[0].domesticBuySell,
					                 strategyDlg.m_tStrategyUI.tStrategyPrarm[0].domesticType, 
									 strategyDlg.m_tStrategyUI.secondname, 
									 strategyDlg.m_tStrategyUI.tStrategyPrarm[0].domesticVolume, 
									 strategyDlg.m_tStrategyUI.tStrategyPrarm[0].domesticAdd));
				string sdiffExp = strategyDlg.m_tStrategyUI.fistname + " . ask - " + strategyDlg.m_tStrategyUI.secondname + " . bid";
				ts1->setCondition(new Condition(sdiffExp, strategyDlg.m_tStrategyUI.tStrategyPrarm[0].compare, strategyDlg.m_tStrategyUI.tStrategyPrarm[0].diff));
				ts1->setHoldingLimit(0, strategyDlg.m_tStrategyUI.maxPostions);
	   			ts1->setHoldingOperation(INCREASE);
				if(strategyDlg.m_tStrategyUI.tStrategyPrarm[0].fishflag == 1)
					ts1->setFish(true);
				else
					ts1->setFish(false);
				ASSERT(ts1 != NULL) ;
				if (strategyDlg.m_tStrategyUI.type == 3)
				{
					TwoLegStrategy * ts2 = dts->createStrategyB();
					ts2->setLegA(new Leg("default", strategyDlg.m_tStrategyUI.tStrategyPrarm[1].overseaBuySell, 
						strategyDlg.m_tStrategyUI.tStrategyPrarm[1].overseaType, 
						strategyDlg.m_tStrategyUI.fistname, 
						strategyDlg.m_tStrategyUI.tStrategyPrarm[1].overseaVolume, 
						strategyDlg.m_tStrategyUI.tStrategyPrarm[1].overseaAdd));
					
					ts2->setLegB(new Leg("default", strategyDlg.m_tStrategyUI.tStrategyPrarm[1].domesticBuySell,
						strategyDlg.m_tStrategyUI.tStrategyPrarm[1].domesticType, 
						strategyDlg.m_tStrategyUI.secondname, 
						strategyDlg.m_tStrategyUI.tStrategyPrarm[1].domesticVolume, 
						strategyDlg.m_tStrategyUI.tStrategyPrarm[1].domesticAdd));
					string sdiffExp = strategyDlg.m_tStrategyUI.fistname + " . ask - " + strategyDlg.m_tStrategyUI.secondname + " . bid";
					ts2->setCondition(new Condition(sdiffExp, strategyDlg.m_tStrategyUI.tStrategyPrarm[1].compare, strategyDlg.m_tStrategyUI.tStrategyPrarm[1].diff));
					ts2->setHoldingLimit(0, strategyDlg.m_tStrategyUI.maxPostions);
					ts2->setHoldingOperation(DECREASE);
					if(strategyDlg.m_tStrategyUI.tStrategyPrarm[1].fishflag == 1)
						ts2->setFish(true);
					else
						ts2->setFish(false);
				}
			}
			if (strategyDlg.m_bRadio == 2)
			{
				TwoLegStrategy * ts1 = dts->createStrategyA();
				ts1->setLegB(new Leg("default", strategyDlg.m_tStrategyUI.tStrategyPrarm[0].overseaBuySell, 
					                 strategyDlg.m_tStrategyUI.tStrategyPrarm[0].overseaType, 
									 strategyDlg.m_tStrategyUI.fistname, 
									 strategyDlg.m_tStrategyUI.tStrategyPrarm[0].overseaVolume, 
									 strategyDlg.m_tStrategyUI.tStrategyPrarm[0].overseaAdd));
				
				ts1->setLegA(new Leg("default", strategyDlg.m_tStrategyUI.tStrategyPrarm[0].domesticBuySell,
					                 strategyDlg.m_tStrategyUI.tStrategyPrarm[0].domesticType, 
									 strategyDlg.m_tStrategyUI.secondname, 
									 strategyDlg.m_tStrategyUI.tStrategyPrarm[0].domesticVolume, 
									 strategyDlg.m_tStrategyUI.tStrategyPrarm[0].domesticAdd));
				string sdiffExp = strategyDlg.m_tStrategyUI.fistname + " . ask - " + strategyDlg.m_tStrategyUI.secondname + " . bid";
				ts1->setCondition(new Condition(sdiffExp, strategyDlg.m_tStrategyUI.tStrategyPrarm[0].compare, strategyDlg.m_tStrategyUI.tStrategyPrarm[0].diff));
				ts1->setHoldingLimit(0, strategyDlg.m_tStrategyUI.maxPostions);
				ts1->setHoldingOperation(INCREASE);
				if(strategyDlg.m_tStrategyUI.tStrategyPrarm[0].fishflag == 1)
					ts1->setFish(true);
				else
					ts1->setFish(false);
				if (strategyDlg.m_tStrategyUI.type == 3)
				{
					TwoLegStrategy * ts2 = dts->createStrategyB();
					ts2->setLegB(new Leg("default", strategyDlg.m_tStrategyUI.tStrategyPrarm[1].overseaBuySell, 
						strategyDlg.m_tStrategyUI.tStrategyPrarm[1].overseaType, 
						strategyDlg.m_tStrategyUI.fistname, 
						strategyDlg.m_tStrategyUI.tStrategyPrarm[1].overseaVolume, 
						strategyDlg.m_tStrategyUI.tStrategyPrarm[1].overseaAdd));
					
					ts2->setLegA(new Leg("default", strategyDlg.m_tStrategyUI.tStrategyPrarm[1].domesticBuySell,
						strategyDlg.m_tStrategyUI.tStrategyPrarm[1].domesticType, 
						strategyDlg.m_tStrategyUI.secondname, 
						strategyDlg.m_tStrategyUI.tStrategyPrarm[1].domesticVolume, 
						strategyDlg.m_tStrategyUI.tStrategyPrarm[1].domesticAdd));
					string sdiffExp = strategyDlg.m_tStrategyUI.fistname + " . ask - " + strategyDlg.m_tStrategyUI.secondname + " . bid";
					ts2->setCondition(new Condition(sdiffExp, strategyDlg.m_tStrategyUI.tStrategyPrarm[1].compare, strategyDlg.m_tStrategyUI.tStrategyPrarm[1].diff));
					ts2->setHoldingLimit(0, strategyDlg.m_tStrategyUI.maxPostions);
					ts2->setHoldingOperation(DECREASE);
					if(strategyDlg.m_tStrategyUI.tStrategyPrarm[1].fishflag == 1)
						ts2->setFish(true);
					else
						ts2->setFish(false);
				}
			}
		
		}
		if(strategyDlg.m_tStrategyUI.type == 2)
		{
			TwoLegStrategy * ts1 = dts->createStrategyA();
			if (strategyDlg.m_bRadio == 1)
			{
				ts1->setLegA(new Leg("default", strategyDlg.m_tStrategyUI.tStrategyPrarm[0].overseaBuySell, 
					                 strategyDlg.m_tStrategyUI.tStrategyPrarm[0].overseaType, 
									 strategyDlg.m_tStrategyUI.fistname, 
									 strategyDlg.m_tStrategyUI.tStrategyPrarm[0].overseaVolume, 
									 strategyDlg.m_tStrategyUI.tStrategyPrarm[0].overseaAdd));
				
				ts1->setLegB(new Leg("default", strategyDlg.m_tStrategyUI.tStrategyPrarm[0].domesticBuySell,
					                 strategyDlg.m_tStrategyUI.tStrategyPrarm[0].domesticType, 
									 strategyDlg.m_tStrategyUI.secondname, 
									 strategyDlg.m_tStrategyUI.tStrategyPrarm[0].domesticVolume, 
									 strategyDlg.m_tStrategyUI.tStrategyPrarm[0].domesticAdd));
				

			}
			if (strategyDlg.m_bRadio == 2)
			{
				ts1->setLegB(new Leg("default", strategyDlg.m_tStrategyUI.tStrategyPrarm[0].overseaBuySell, 
					                 strategyDlg.m_tStrategyUI.tStrategyPrarm[0].overseaType, 
									 strategyDlg.m_tStrategyUI.fistname, 
									 strategyDlg.m_tStrategyUI.tStrategyPrarm[0].overseaVolume, 
									 strategyDlg.m_tStrategyUI.tStrategyPrarm[0].overseaAdd));
				
				ts1->setLegA(new Leg("default", strategyDlg.m_tStrategyUI.tStrategyPrarm[0].domesticBuySell,
					                 strategyDlg.m_tStrategyUI.tStrategyPrarm[0].domesticType, 
									 strategyDlg.m_tStrategyUI.secondname, 
									 strategyDlg.m_tStrategyUI.tStrategyPrarm[0].domesticVolume, 
									 strategyDlg.m_tStrategyUI.tStrategyPrarm[0].domesticAdd));
			}
			string sdiffExp = strategyDlg.m_tStrategyUI.fistname + " . ask - " + strategyDlg.m_tStrategyUI.secondname + " . bid";
			ts1->setCondition(new Condition(sdiffExp, strategyDlg.m_tStrategyUI.tStrategyPrarm[0].compare, strategyDlg.m_tStrategyUI.tStrategyPrarm[0].diff));
			ts1->setHoldingLimit(0, strategyDlg.m_tStrategyUI.maxPostions);
			ts1->setHoldingOperation(INCREASE);
			if(strategyDlg.m_tStrategyUI.tStrategyPrarm[0].fishflag == 1)
				ts1->setFish(true);
			else
				ts1->setFish(false);
		}
		dts->save();

		TStrategyInfo *ptStrategyInfo = new TStrategyInfo;
		ptStrategyInfo->dts = NULL;

		ptStrategyInfo->dts = dts;

		strategyDlg.m_tStrategyUI.sStrategy = dts->toDesc();//toString(); 

		CopyStructStrategyUI(&(ptStrategyInfo->tStrategyUI), (strategyDlg.m_tStrategyUI));

		pIPInfo->strategyInfoList.push_back(ptStrategyInfo);

		
		if(strategyDlg.m_nOP == 1)
		{
			if (dts != NULL)
				dts->start();
		}else{
			if (dts != NULL)
				dts->stop();
			
		}

		string strategyName(szname);
		TIPInfo* pIPInfo = FindIPInfomap(strategyName);
		if (pIPInfo == NULL)	return;
		list<TStrategyInfo*> ::iterator it ;
		
		pMainFrame->m_pStrategyInfoDlg->m_listStrategyInfo.DeleteAllItems();
		
		for(it = pIPInfo->strategyInfoList.begin(); it != pIPInfo->strategyInfoList.end(); ++it)// list
		{
			int count = pMainFrame->m_pStrategyInfoDlg->m_listStrategyInfo.GetItemCount();
			int index = pMainFrame->m_pStrategyInfoDlg->m_listStrategyInfo.InsertItem(count, (*it)->tStrategyUI.sName );
			pMainFrame->m_pStrategyInfoDlg->m_listStrategyInfo.SetItem(index,2,LVIF_TEXT,(*it)->tStrategyUI.sStrategy.c_str(), 0, 0, 0,0);
			if((*it)->tStrategyUI.sgstate == WORK)
				pMainFrame->m_pStrategyInfoDlg->m_listStrategyInfo.SetItem(index,1,LVIF_TEXT,"已启动", 0, 0, 0,0);
				//pMainFrame->m_pStrategyInfoDlg->m_listStrategyInfo.SetItemTextColor(1,index,RGB(255,0,0));
			else if((*it)->tStrategyUI.sgstate == STOP)
				pMainFrame->m_pStrategyInfoDlg->m_listStrategyInfo.SetItem(index,1,LVIF_TEXT,"已停止", 0, 0, 0,0);
				//pMainFrame->m_pStrategyInfoDlg->m_listStrategyInfo.SetItemTextColor(1,index,RGB(255,255,255));
		}
	}
 
}

void CIntentionPairDlg::OnRclickListIntentionPair(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	DWORD   dwPos   =   GetMessagePos(); 
	CPoint   point1(   LOWORD(dwPos),   HIWORD(dwPos)   ); 
	CPoint   point2(   LOWORD(dwPos),   HIWORD(dwPos)   ); 
	
	m_listIntentionPair.ScreenToClient(&point2); 
	m_nRSelItem   =   m_listIntentionPair.HitTest(point2); 

//	NMLISTVIEW*   pNmlist=   (NMLISTVIEW*)pNMHDR; 
//  pNmlist-> iItem;           //行号   
//  pNmlist-> iSubItem;     //列号
	
	if   (point1.x   ==   -1   &&   point1.y   ==   -1) 
		point1.Offset(5,   5); 
	
	CMenu   menu; 
	VERIFY(menu.LoadMenu(IDR_POP_INTENTION_MENU)); 
	CMenu*   pPopup   =   menu.GetSubMenu(0); 
	ASSERT(pPopup   !=   NULL); 
	SetMenuState(m_nRSelItem, pPopup);
	
	pPopup-> TrackPopupMenu(TPM_LEFTALIGN   |   TPM_RIGHTBUTTON,   point1.x,   point1.y,   this); 

	*pResult = 0;
}

void CIntentionPairDlg::SetMenuState(int nItem, CMenu*   pPopup)
{
	char szStrategyName[32];
	memset(szStrategyName, 0, sizeof(szStrategyName));
	m_listIntentionPair.GetItemText(nItem, 0, szStrategyName, sizeof(szStrategyName));
	string strategyName(szStrategyName);
	TIPInfo* ret = FindIPstatemap(strategyName);
	if (ret == NULL)
	{
		pPopup->EnableMenuItem(0,MF_BYPOSITION|MF_ENABLED);
		pPopup->EnableMenuItem(1,MF_BYPOSITION|MF_GRAYED);
		pPopup->EnableMenuItem(2,MF_BYPOSITION|MF_GRAYED);
		return;
	}
	else{
	
	pPopup->EnableMenuItem(1,MF_BYPOSITION|MF_ENABLED);
	pPopup->EnableMenuItem(0,MF_BYPOSITION|MF_GRAYED);
	pPopup->EnableMenuItem(2,MF_BYPOSITION|MF_ENABLED);
	}

}

TIPInfo* CIntentionPairDlg::FindIPstatemap(string strategyName)
{
	
	IPInfoMap::iterator iter = gpMainFrame->m_IPInfoMap.find(strategyName);
	if( iter != gpMainFrame->m_IPInfoMap.end())	return iter->second;
	else 	return NULL;
}


void CIntentionPairDlg::OnClickListIntentionPair(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	LPNMITEMACTIVATE temp = (LPNMITEMACTIVATE) pNMHDR;

	CMainFrame * pMainFrame = NULL;
	pMainFrame = ((CMainFrame*)::AfxGetMainWnd());

    int nItem=temp->iItem;
	char szStrategyName[32];
	memset(szStrategyName, 0, sizeof(szStrategyName));
	m_listIntentionPair.GetItemText(nItem, 0, szStrategyName, sizeof(szStrategyName));
	string strategyName(szStrategyName);
	TIPInfo* pIPInfo = FindIPInfomap(strategyName);
	if (pIPInfo == NULL)	return;
	list<TStrategyInfo*> ::iterator it ;

	pMainFrame->m_pStrategyInfoDlg->m_listStrategyInfo.DeleteAllItems();
	
	for(it = pIPInfo->strategyInfoList.begin(); it != pIPInfo->strategyInfoList.end(); ++it)// list
	{
		int count = pMainFrame->m_pStrategyInfoDlg->m_listStrategyInfo.GetItemCount();
		int index = pMainFrame->m_pStrategyInfoDlg->m_listStrategyInfo.InsertItem(count, (*it)->tStrategyUI.sName );
		pMainFrame->m_pStrategyInfoDlg->m_listStrategyInfo.SetItem(index,2,LVIF_TEXT,(*it)->tStrategyUI.sStrategy.c_str(), 0, 0, 0,0);
		if((*it)->tStrategyUI.sgstate == WORK)
			pMainFrame->m_pStrategyInfoDlg->m_listStrategyInfo.SetItem(index,1,LVIF_TEXT,"已启动", 0, 0, 0,0);
			//pMainFrame->m_pStrategyInfoDlg->m_listStrategyInfo.SetItemTextColor(1,index,RGB(255,0,0));
		else if((*it)->tStrategyUI.sgstate == STOP)
			pMainFrame->m_pStrategyInfoDlg->m_listStrategyInfo.SetItem(index,1,LVIF_TEXT,"已停止", 0, 0, 0,0);
			//pMainFrame->m_pStrategyInfoDlg->m_listStrategyInfo.SetItemTextColor(1,index,RGB(255,255,255));
	}

	*pResult = 0;
}

TIPInfo* CIntentionPairDlg::FindIPInfomap(string szname)
{
	TIPInfo* ret = NULL;

	IPInfoMap::iterator iter = gpMainFrame->m_IPInfoMap.find(szname);
	if( iter != gpMainFrame->m_IPInfoMap.end())
		ret = iter->second;

	return ret;
}

BOOL CIntentionPairDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message==WM_KEYDOWN && (pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE)) 
		return TRUE;
	else 
		return CDialog::PreTranslateMessage(pMsg); 
}

void CIntentionPairDlg::OnGetdispinfoListIntentionPair(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	LV_ITEM* pItem= &(pDispInfo)->item;
	
	int iItemIndx= pItem->iItem;
	if( pItem->mask&LVIF_TEXT ) {//字符串缓冲区有效
		switch( pItem->iSubItem ) {
		case 0:
			lstrcpy( pItem->pszText, m_listIPUI[iItemIndx].sName.c_str() );
			break;
		case 2:
			lstrcpy( pItem->pszText, m_listIPUI[iItemIndx].fistbuy.c_str() );
			break;
		case 3:
			lstrcpy( pItem->pszText, m_listIPUI[iItemIndx].fistsell.c_str() );
			break;
		case 4:
			lstrcpy( pItem->pszText, m_listIPUI[iItemIndx].buydiff.c_str() );
			break;
		case 5:
			lstrcpy( pItem->pszText, m_listIPUI[iItemIndx].kc.c_str() );
			break;
		case 6:
			lstrcpy( pItem->pszText, m_listIPUI[iItemIndx].selldiff.c_str() );
			break;
		case 7:
			lstrcpy( pItem->pszText, m_listIPUI[iItemIndx].pc.c_str() );
			break;
		case 8:
			lstrcpy( pItem->pszText, m_listIPUI[iItemIndx].secondbuy.c_str() );
			break;
		case 9:
			lstrcpy( pItem->pszText, m_listIPUI[iItemIndx].secondsell.c_str() );
			break;
		case 10:
			lstrcpy( pItem->pszText, m_listIPUI[iItemIndx].buyMaxdiff.c_str() );
			break;
		case 11:
			lstrcpy( pItem->pszText, m_listIPUI[iItemIndx].buyMindiff.c_str() );
			break;
		case 12:
			lstrcpy( pItem->pszText, m_listIPUI[iItemIndx].sellMaxdiff.c_str() );
			break;
		case 13:
			lstrcpy( pItem->pszText, m_listIPUI[iItemIndx].sellMindiff.c_str() );
			break;
		}
	}
	*pResult = 0;
}
void CIntentionPairDlg::OnColumnclickListSp(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if( pNMListView->iSubItem == 1 )//进口盈亏
	{
		if(m_sortflag == 0)//0 为不排序
		{
//			TRACE_LOG("不排序变降序");
			KillTimer(2);
			m_sortflag = 1;
			SetTimer(3,500,NULL);
		}
		else if(m_sortflag == 1)//1 为降序
		{
//			TRACE_LOG("降序变升序");
			KillTimer(3);
			m_sortflag = 2;
			SetTimer(4,500,NULL);
		} 
		else if(m_sortflag == 2)//2 为升序
		{
//			TRACE_LOG("升序变不排序");
			KillTimer(4);
			m_sortflag = 0;
			SetTimer(2,500,NULL);
		}
	}
}
void CIntentionPairDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1)
	{
		KillTimer(1);
		TwoLegStrategy *ts2;
		TwoLegStrategy *ts1;
		IPInfoMap::iterator iter = gpMainFrame->m_IPInfoMap.begin();
		while( iter != gpMainFrame->m_IPInfoMap.end()){
			list<TStrategyInfo*> ::iterator it ;
			for(it = iter->second->strategyInfoList.begin(); it != iter->second->strategyInfoList.end(); ++it)// list
			{
				ts2 = NULL;
				ts1 = NULL;
				ts1 = (*it)->dts->getStrategyA();
				if((*it)->dts->hasStrategyB())
				{
					ts2 = (*it)->dts->getStrategyB();
				}

				if(ts1 != NULL){
					if(ts1->isSecondLegMissed()) 
					{
						TRACE_LOG("second leg missed");
						if(AfxMessageBox( "第二腿打空,请处理!", MB_OK ) == IDOK)	
						ts1->missedLegHandled();
					}
				}
				if(ts2 != NULL){
					if(ts2->isSecondLegMissed())
					{
						TRACE_LOG("second leg missed");
						if(AfxMessageBox( "第二腿打空,请处理!", MB_OK ) == IDOK)
					    ts2->missedLegHandled();
					}
				}
			}
			iter++;
		}
		SetTimer(1,500,NULL);
	}else if (nIDEvent == 2)//不排序
	{
		KillTimer(2);
		UpdateInitIntentionPair(IntentionPair::listAll());
	}else if (nIDEvent == 3)//1 为降序
	{
		KillTimer(3);
		UpdateInitIntentionPair(IntentionPair::sortListAll(DOWN));
		SetTimer(3,1000 * 60,NULL);
	}else if (nIDEvent == 4)//2 为升序
	{
		KillTimer(4);
		UpdateInitIntentionPair(IntentionPair::sortListAll(UP));
		SetTimer(4,1000 * 60,NULL);
	}
	CDialog::OnTimer(nIDEvent);
}


void CIntentionPairDlg::OnDataCurve()
{
	// TODO: 在此添加命令处理程序代码
	char szname[64];
	memset(szname, 0, sizeof(szname));
	if(m_nRSelItem == -1) return; 
	m_listIntentionPair.GetItemText(m_nRSelItem, 0 ,szname , 64);
	IntentionPair *  pIP = IntentionPair::get(szname);
	string sfilename = pIP->getDateFileName();
	string spath = "testmschart.exe";
	ShellExecute(this->m_hWnd,"open",spath.c_str(),sfilename.c_str(), "",SW_SHOW);
}


void CIntentionPairDlg::OnDblclkListIntentionPair(NMHDR *pNMHDR, LRESULT *pResult)
{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
}


void CIntentionPairDlg::OnLvnItemchangedListIntentionPair(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CIntentionPairDlg::OnAddMoreStrategy()
{
	CMainFrame * pMainFrame = gpMainFrame;
	CStrategyMoreDlg strategyDlg;
	char szname[64];
	memset(szname, 0, sizeof(szname));
	if(m_nRSelItem == -1) return; 
	m_listIntentionPair.GetItemText(m_nRSelItem, 0 ,szname , 64);
	if (strlen(szname) == 0) return;
	//找到Intentionpair结构TIPInfo
	IPInfoMap::iterator iter = pMainFrame->m_IPInfoMap.find(szname);
	assert( iter != pMainFrame->m_IPInfoMap.end());
	TIPInfo *pIPInfo = iter->second;
	strategyDlg.m_strName = szname;
	strategyDlg.m_strContractOversea = pIPInfo->firstname.c_str();
	strategyDlg.m_strContractDomestic = pIPInfo->secondname.c_str();
	strategyDlg.m_nVolumeOversea   = pIPInfo->firstVol;
	strategyDlg.m_nVolumeDomestic  = pIPInfo->secondVol;


	if(IDOK == strategyDlg.DoModal())
	{
		int count = strategyDlg.m_count;
		for(int i = 0; i < count; i++){
		CString strStrategyKeyName;
		char stime[32];
		memset(stime, 0, sizeof(stime));
		pMainFrame->GetCurTime(stime);
		Sleep(50);
		strStrategyKeyName.Format("%s_%s",szname,stime);
		
		strcpy_s(strategyDlg.m_tStrategyUI[i].sName,32, strStrategyKeyName.GetBuffer(strStrategyKeyName.GetLength()));

		if(strategyDlg.m_nOP == 1)
			strategyDlg.m_tStrategyUI[i].sgstate = WORK;
		else
			strategyDlg.m_tStrategyUI[i].sgstate = STOP;

		strategyDlg.m_tStrategyUI[i].firstLeg = strategyDlg.m_bRadio;

		DayTradingStrategy * dts = DayTradingStrategy::create(strStrategyKeyName.GetBuffer(strStrategyKeyName.GetLength()));
		
        if(strategyDlg.m_tStrategyUI[i].type == 1 || strategyDlg.m_tStrategyUI[i].type == 3)
		{
			
			if (strategyDlg.m_bRadio == 1)
			{
				TwoLegStrategy * ts1 = dts->createStrategyA();
				ts1->setLegA(new Leg("default", strategyDlg.m_tStrategyUI[i].tStrategyPrarm[0].overseaBuySell, 
					                 strategyDlg.m_tStrategyUI[i].tStrategyPrarm[0].overseaType, 
									 strategyDlg.m_tStrategyUI[i].fistname, 
									 strategyDlg.m_tStrategyUI[i].tStrategyPrarm[0].overseaVolume, 
									 strategyDlg.m_tStrategyUI[i].tStrategyPrarm[0].overseaAdd));
				
				ts1->setLegB(new Leg("default", strategyDlg.m_tStrategyUI[i].tStrategyPrarm[0].domesticBuySell,
					                 strategyDlg.m_tStrategyUI[i].tStrategyPrarm[0].domesticType, 
									 strategyDlg.m_tStrategyUI[i].secondname, 
									 strategyDlg.m_tStrategyUI[i].tStrategyPrarm[0].domesticVolume, 
									 strategyDlg.m_tStrategyUI[i].tStrategyPrarm[0].domesticAdd));
				string sdiffExp = strategyDlg.m_tStrategyUI[i].fistname + " . ask - " + strategyDlg.m_tStrategyUI[i].secondname + " . bid";
				ts1->setCondition(new Condition(sdiffExp, strategyDlg.m_tStrategyUI[i].tStrategyPrarm[0].compare, strategyDlg.m_tStrategyUI[i].tStrategyPrarm[0].diff));
				ts1->setHoldingLimit(0, strategyDlg.m_tStrategyUI[i].maxPostions);
	   			ts1->setHoldingOperation(INCREASE);
				if(strategyDlg.m_tStrategyUI[i].tStrategyPrarm[0].fishflag == 1)
					ts1->setFish(true);
				else
					ts1->setFish(false);
				ASSERT(ts1 != NULL) ;
				if (strategyDlg.m_tStrategyUI[i].type == 3)
				{
					TwoLegStrategy * ts2 = dts->createStrategyB();
					ts2->setLegA(new Leg("default", strategyDlg.m_tStrategyUI[i].tStrategyPrarm[1].overseaBuySell, 
						strategyDlg.m_tStrategyUI[i].tStrategyPrarm[1].overseaType, 
						strategyDlg.m_tStrategyUI[i].fistname, 
						strategyDlg.m_tStrategyUI[i].tStrategyPrarm[1].overseaVolume, 
						strategyDlg.m_tStrategyUI[i].tStrategyPrarm[1].overseaAdd));
					
					ts2->setLegB(new Leg("default", strategyDlg.m_tStrategyUI[i].tStrategyPrarm[1].domesticBuySell,
						strategyDlg.m_tStrategyUI[i].tStrategyPrarm[1].domesticType, 
						strategyDlg.m_tStrategyUI[i].secondname, 
						strategyDlg.m_tStrategyUI[i].tStrategyPrarm[1].domesticVolume, 
						strategyDlg.m_tStrategyUI[i].tStrategyPrarm[1].domesticAdd));
					string sdiffExp = strategyDlg.m_tStrategyUI[i].fistname + " . ask - " + strategyDlg.m_tStrategyUI[i].secondname + " . bid";
					ts2->setCondition(new Condition(sdiffExp, strategyDlg.m_tStrategyUI[i].tStrategyPrarm[1].compare, strategyDlg.m_tStrategyUI[i].tStrategyPrarm[1].diff));
					ts2->setHoldingLimit(0, strategyDlg.m_tStrategyUI[i].maxPostions);
					ts2->setHoldingOperation(DECREASE);
					if(strategyDlg.m_tStrategyUI[i].tStrategyPrarm[1].fishflag == 1)
						ts2->setFish(true);
					else
						ts2->setFish(false);
				}
			}
			if (strategyDlg.m_bRadio == 2)
			{
				TwoLegStrategy * ts1 = dts->createStrategyA();
				ts1->setLegB(new Leg("default", strategyDlg.m_tStrategyUI[i].tStrategyPrarm[0].overseaBuySell, 
					                 strategyDlg.m_tStrategyUI[i].tStrategyPrarm[0].overseaType, 
									 strategyDlg.m_tStrategyUI[i].fistname, 
									 strategyDlg.m_tStrategyUI[i].tStrategyPrarm[0].overseaVolume, 
									 strategyDlg.m_tStrategyUI[i].tStrategyPrarm[0].overseaAdd));
				
				ts1->setLegA(new Leg("default", strategyDlg.m_tStrategyUI[i].tStrategyPrarm[0].domesticBuySell,
					                 strategyDlg.m_tStrategyUI[i].tStrategyPrarm[0].domesticType, 
									 strategyDlg.m_tStrategyUI[i].secondname, 
									 strategyDlg.m_tStrategyUI[i].tStrategyPrarm[0].domesticVolume, 
									 strategyDlg.m_tStrategyUI[i].tStrategyPrarm[0].domesticAdd));
				string sdiffExp = strategyDlg.m_tStrategyUI[i].fistname + " . ask - " + strategyDlg.m_tStrategyUI[i].secondname + " . bid";
				ts1->setCondition(new Condition(sdiffExp, strategyDlg.m_tStrategyUI[i].tStrategyPrarm[0].compare, strategyDlg.m_tStrategyUI[i].tStrategyPrarm[0].diff));
				ts1->setHoldingLimit(0, strategyDlg.m_tStrategyUI[i].maxPostions);
				ts1->setHoldingOperation(INCREASE);
				if(strategyDlg.m_tStrategyUI[i].tStrategyPrarm[0].fishflag == 1)
					ts1->setFish(true);
				else
					ts1->setFish(false);
				if (strategyDlg.m_tStrategyUI[i].type == 3)
				{
					TwoLegStrategy * ts2 = dts->createStrategyB();
					ts2->setLegB(new Leg("default", strategyDlg.m_tStrategyUI[i].tStrategyPrarm[1].overseaBuySell, 
						strategyDlg.m_tStrategyUI[i].tStrategyPrarm[1].overseaType, 
						strategyDlg.m_tStrategyUI[i].fistname, 
						strategyDlg.m_tStrategyUI[i].tStrategyPrarm[1].overseaVolume, 
						strategyDlg.m_tStrategyUI[i].tStrategyPrarm[1].overseaAdd));
					
					ts2->setLegA(new Leg("default", strategyDlg.m_tStrategyUI[i].tStrategyPrarm[1].domesticBuySell,
						strategyDlg.m_tStrategyUI[i].tStrategyPrarm[1].domesticType, 
						strategyDlg.m_tStrategyUI[i].secondname, 
						strategyDlg.m_tStrategyUI[i].tStrategyPrarm[1].domesticVolume, 
						strategyDlg.m_tStrategyUI[i].tStrategyPrarm[1].domesticAdd));
					string sdiffExp = strategyDlg.m_tStrategyUI[i].fistname + " . ask - " + strategyDlg.m_tStrategyUI[i].secondname + " . bid";
					ts2->setCondition(new Condition(sdiffExp, strategyDlg.m_tStrategyUI[i].tStrategyPrarm[1].compare, strategyDlg.m_tStrategyUI[i].tStrategyPrarm[1].diff));
					ts2->setHoldingLimit(0, strategyDlg.m_tStrategyUI[i].maxPostions);
					ts2->setHoldingOperation(DECREASE);
					if(strategyDlg.m_tStrategyUI[i].tStrategyPrarm[1].fishflag == 1)
						ts2->setFish(true);
					else
						ts2->setFish(false);
				}
			}
		
		}
		
		dts->save();

		TStrategyInfo *ptStrategyInfo = new TStrategyInfo;
		ptStrategyInfo->dts = NULL;

		ptStrategyInfo->dts = dts;

		strategyDlg.m_tStrategyUI[i].sStrategy = dts->toDesc();//toString(); 

		CopyStructStrategyUI(&(ptStrategyInfo->tStrategyUI), (strategyDlg.m_tStrategyUI[i]));

		pIPInfo->strategyInfoList.push_back(ptStrategyInfo);

		
		if(strategyDlg.m_nOP == 1)
		{
			if (dts != NULL)
				dts->start();
		}else{
			if (dts != NULL)
				dts->stop();
			
		}

		string strategyName(szname);
		TIPInfo* pIPInfo = FindIPInfomap(strategyName);
		if (pIPInfo == NULL)	return;
		list<TStrategyInfo*> ::iterator it ;
		
		pMainFrame->m_pStrategyInfoDlg->m_listStrategyInfo.DeleteAllItems();
		
		for(it = pIPInfo->strategyInfoList.begin(); it != pIPInfo->strategyInfoList.end(); ++it)// list
		{
			int count = pMainFrame->m_pStrategyInfoDlg->m_listStrategyInfo.GetItemCount();
			int index = pMainFrame->m_pStrategyInfoDlg->m_listStrategyInfo.InsertItem(count, (*it)->tStrategyUI.sName );
			pMainFrame->m_pStrategyInfoDlg->m_listStrategyInfo.SetItem(index,2,LVIF_TEXT,(*it)->tStrategyUI.sStrategy.c_str(), 0, 0, 0,0);
			if((*it)->tStrategyUI.sgstate == WORK){
				pMainFrame->m_pStrategyInfoDlg->m_listStrategyInfo.SetItem(index,1,LVIF_TEXT,"已启动", 0, 0, 0,0);
				pMainFrame->m_pStrategyInfoDlg->m_listStrategyInfo.SetItemTextColor(1,index,RGB(255,0,0));
			}
			else if((*it)->tStrategyUI.sgstate == STOP){
				pMainFrame->m_pStrategyInfoDlg->m_listStrategyInfo.SetItem(index,1,LVIF_TEXT,"已停止", 0, 0, 0,0);
				pMainFrame->m_pStrategyInfoDlg->m_listStrategyInfo.SetItemTextColor(1,index,RGB(255,255,255));
			}
		}
	}
	}
 
}
