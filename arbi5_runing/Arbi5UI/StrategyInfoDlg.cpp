// StrategyInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include <assert.h>
#include "Arbi5UI.h"
#include "StrategyInfoDlg.h"
#include "StrategyDlg.h"
#include "..\LogStore.h"
#include "MainFrm.h"
#include "..\Strategy.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStrategyInfoDlg dialog
extern CMainFrame * gpMainFrame;
IMPLEMENT_DYNCREATE(CStrategyInfoDlg, CDialog)
CStrategyInfoDlg::CStrategyInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStrategyInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStrategyInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CStrategyInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStrategyInfoDlg)
	DDX_Control(pDX, IDC_LIST_STRATEGY_INFO, m_listStrategyInfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStrategyInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CStrategyInfoDlg)
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_STRATEGY_INFO, OnRclickListStrategyInfo)
	ON_COMMAND(ID_MODIFY, OnEditStrategy)
	ON_COMMAND(ID_STOP, OnStopStrategy)
	ON_COMMAND(ID_START, OnStartStrategy)
	ON_COMMAND(ID_DEL, OnDelStrategy)
	 
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_STRATEGY_INFO, &CStrategyInfoDlg::OnDblclkListStrategyInfo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStrategyInfoDlg message handlers
void CStrategyInfoDlg::OnStartStrategy()
{
	CMainFrame * pMainFrame = gpMainFrame;

	char szname[64];
	POSITION pos = m_listStrategyInfo.GetFirstSelectedItemPosition();
	m_listSeletcItem.clear();
	while(pos)
	{
		int CirItem = m_listStrategyInfo.GetNextSelectedItem(pos);
		m_listSeletcItem.push_back(CirItem);
	}
	if(m_nRSelItem == -1) return; 
	if(m_listSeletcItem.size() >=1 ){
		list<int> ::iterator it ;
		for(it = m_listSeletcItem.begin(); it != m_listSeletcItem.end(); ++it)// list
		{
			memset(szname, 0, sizeof(szname));
			m_listStrategyInfo.GetItemText((*it), 0 ,szname , 64);

			CString strName = szname;
			int pos = strName.Find("_");
			if (pos <= 0)
				return;
			strName = strName.Mid(0, pos);

			string strategyName(szname);
			TStrategyInfo* ret = FindSGstatemap(strategyName);
			if (ret == NULL)
				return;

			if ( ret->dts != NULL) ret->dts->start();
			ret->tStrategyUI.sgstate = WORK;

			m_listStrategyInfo.SetItem((*it),1,LVIF_TEXT,"已启动", 0, 0, 0,0);
		
		}
	}
	m_listSeletcItem.clear();
}

void CStrategyInfoDlg::OnStopStrategy()
{
	CMainFrame * pMainFrame = gpMainFrame;
	char szname[64];
	
	POSITION pos = m_listStrategyInfo.GetFirstSelectedItemPosition();
	m_listSeletcItem.clear();
	while(pos)
	{
		int CirItem = m_listStrategyInfo.GetNextSelectedItem(pos);
		m_listSeletcItem.push_back(CirItem);
	}
	if(m_nRSelItem == -1) return; 
	if(m_listSeletcItem.size() >=1 ){
		list<int> ::iterator it ;
		for(it = m_listSeletcItem.begin(); it != m_listSeletcItem.end(); ++it)// list
		{
			memset(szname, 0, sizeof(szname));
			m_listStrategyInfo.GetItemText((*it), 0 ,szname , 64);

			CString strName = szname;
			int pos = strName.Find("_");
			if (pos <= 0)
				return;
			strName = strName.Mid(0, pos);

			string strategyName(szname);
			TStrategyInfo* ret = FindSGstatemap(strategyName);
			if (ret == NULL)
				return;

			//stop this strategy
			if ( ret->dts != NULL) ret->dts->stop();
			ret->tStrategyUI.sgstate = STOP;

			m_listStrategyInfo.SetItem((*it),1,LVIF_TEXT,"已停止", 0, 0, 0,0);
			
		}
	}
	m_listSeletcItem.clear();
}

void CStrategyInfoDlg::OnEditStrategy() 
{
	CMainFrame * pMainFrame = gpMainFrame;
	char szname[64];
	memset(szname, 0, sizeof(szname));
	if(m_nRSelItem == -1) return; 
	m_listStrategyInfo.GetItemText(m_nRSelItem, 0 ,szname , 64);

	CString strName = szname;
	int pos = strName.Find("_");
	if (pos <= 0)
		return;
    strName = strName.Mid(0, pos);

	IPInfoMap::iterator iter = pMainFrame->m_IPInfoMap.find(strName.GetBuffer(strName.GetLength()));
	assert( iter != pMainFrame->m_IPInfoMap.end());
	TIPInfo *pIPInfo = iter->second;

	
	list<TStrategyInfo*> ::iterator it ;
	CStrategyDlg strategyDlg;
	
	DayTradingStrategy * dts = NULL;
	for(it = pIPInfo->strategyInfoList.begin(); it != pIPInfo->strategyInfoList.end(); ++it)// list
	{
		if(strcmp((*it)->tStrategyUI.sName ,szname) == 0)
		{
			CopyStructStrategyUI(&(strategyDlg.m_tStrategyUI), (*it)->tStrategyUI);
			dts = (*it)->dts;
			break;
		}
	}

	if (dts == NULL)
	{
		TRACE_LOG("找不到%s对应的TwoLegStrategy指针",szname);
		return;
	}
	TwoLegStrategy *ts1 = dts->getStrategyA();
	TwoLegStrategy *ts2 = dts->getStrategyB();

	strategyDlg.m_buttonstate = 2; //modify
	strategyDlg.m_strName = strName;
	strategyDlg.m_strContractOversea  = pIPInfo->firstname.c_str();
	strategyDlg.m_strContractDomestic = pIPInfo->secondname.c_str();
	strategyDlg.m_nVolumeOversea  = strategyDlg.m_nVolumeOversea2 = pIPInfo->firstVol;
	strategyDlg.m_nVolumeDomestic = strategyDlg.m_nVolumeDomestic2 = pIPInfo->secondVol;
	
	if(IDOK == strategyDlg.DoModal())
	{
		//TODO add for modify strategy
		strcpy_s(strategyDlg.m_tStrategyUI.sName, 32, szname);

		if(strategyDlg.m_nOP == 1)
			strategyDlg.m_tStrategyUI.sgstate = WORK;
		else
			strategyDlg.m_tStrategyUI.sgstate = STOP;

		strategyDlg.m_tStrategyUI.firstLeg = strategyDlg.m_bRadio;
        if(strategyDlg.m_tStrategyUI.type == 1 || strategyDlg.m_tStrategyUI.type == 3)
		{
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
					if(ts2 == NULL)
						ts2 = dts->createStrategyB();
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
				}
				else
					dts->setStrategyBNull();
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
					if(ts2 == NULL)
						ts2 = dts->createStrategyB();
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
				else{
					
					dts->setStrategyBNull();
				}
			}

		}
		if(strategyDlg.m_tStrategyUI.type == 2)
		{
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

		TStrategyInfo *ptStrategyInfo = FindSGstatemap(szname);
		ptStrategyInfo->dts = dts;

		strategyDlg.m_tStrategyUI.sStrategy = dts->toDesc();//dts->toString();

		CopyStructStrategyUI(&(ptStrategyInfo->tStrategyUI), (strategyDlg.m_tStrategyUI));
		
		if(strategyDlg.m_nOP == 1)
		{
			if (dts != NULL)
				dts->start();
		}else{
			if (dts != NULL)
				dts->stop();
		
		}

		list<TStrategyInfo*> ::iterator it ;
		
		m_listStrategyInfo.DeleteAllItems();
		
		for(it = pIPInfo->strategyInfoList.begin(); it != pIPInfo->strategyInfoList.end(); ++it)// list
		{
			int count = m_listStrategyInfo.GetItemCount();
			int index = m_listStrategyInfo.InsertItem(count, (*it)->tStrategyUI.sName );
			m_listStrategyInfo.SetItem(index,2,LVIF_TEXT,(*it)->tStrategyUI.sStrategy.c_str(), 0, 0, 0,0);
			if((*it)->tStrategyUI.sgstate == WORK)
				m_listStrategyInfo.SetItem(index,1,LVIF_TEXT,"已启动", 0, 0, 0,0);
				//pMainFrame->m_pStrategyInfoDlg->m_listStrategyInfo.SetItemTextColor(1,index,RGB(255,0,0));
			else
				m_listStrategyInfo.SetItem(index,1,LVIF_TEXT,"已停止", 0, 0, 0,0);
				//pMainFrame->m_pStrategyInfoDlg->m_listStrategyInfo.SetItemTextColor(1,index,RGB(255,255,255));
		}
		m_listStrategyInfo.SetSelectionMark(m_nRSelItem);
		m_listStrategyInfo.SetItemState(m_nRSelItem,   LVIS_SELECTED,   LVIS_SELECTED);
	}
	return;
}

void CStrategyInfoDlg::OnDelStrategy() 
{
	char szname[64];
	memset(szname, 0, sizeof(szname));
	if(m_nRSelItem == -1) return; 
	m_listStrategyInfo.GetItemText(m_nRSelItem, 0 ,szname , 64);

	CString strName = szname;
	int pos = strName.Find("_");
	if (pos <= 0)
		return;
    CString strIPName = strName.Mid(0, pos);

	IPInfoMap::iterator iter = gpMainFrame->m_IPInfoMap.find(strIPName.GetBuffer(strIPName.GetLength()));
	assert( iter != gpMainFrame->m_IPInfoMap.end());
	TIPInfo *pIPInfo = iter->second;
	if (pIPInfo == NULL)	return;

	list<TStrategyInfo*> ::iterator it ;
	m_listStrategyInfo.DeleteAllItems();
	for(it = pIPInfo->strategyInfoList.begin(); it != pIPInfo->strategyInfoList.end();)
	{
		if (strcmp((*it)->tStrategyUI.sName, szname) == 0)
		{
			(*it)->dts->remove();
			it = pIPInfo->strategyInfoList.erase(it);
			continue;
		}
		int count = m_listStrategyInfo.GetItemCount();
		int index = m_listStrategyInfo.InsertItem(count, (*it)->tStrategyUI.sName );
		m_listStrategyInfo.SetItem(index,2,LVIF_TEXT,(*it)->tStrategyUI.sStrategy.c_str(), 0, 0, 0,0);
		if((*it)->tStrategyUI.sgstate == WORK)
			m_listStrategyInfo.SetItem(index,1,LVIF_TEXT,"已启动", 0, 0, 0,0);
		else
			m_listStrategyInfo.SetItem(index,1,LVIF_TEXT,"已停止", 0, 0, 0,0);

		it++;
	}


}

BOOL CStrategyInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CMainFrame * pMainFrame = NULL;
	pMainFrame = ((CMainFrame*)::AfxGetMainWnd());
	pMainFrame->m_pStrategyInfoDlg = this;
	
	// TODO: Add extra initialization here
	m_listStrategyInfo.SetBkColor(RGB(0,0,0));        //设置背景色
	m_listStrategyInfo.SetRowHeigt(25);               //设置行高度
	m_listStrategyInfo.SetHeaderHeight(1.5);          //设置头部高度
	m_listStrategyInfo.SetHeaderFontHW(22,0);         //设置头部字体高度,和宽度,0表示缺省，自适应 
	m_listStrategyInfo.SetHeaderTextColor(RGB(255,200,100)); //设置头部字体颜色
	m_listStrategyInfo.SetTextColor(RGB(255,0,0));  //设置文本颜色
	m_listStrategyInfo.SetFontHW(18,0);               //设置字体高度，和宽度,0表示缺省宽度

   //设置字体高度，和宽度,0表示缺省宽度

	int   nFullWidth=GetSystemMetrics(SM_CXSCREEN); 
	int base = (nFullWidth - 25) /7;
	int nlen = base*5;

	m_listStrategyInfo.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listStrategyInfo.InsertColumn(0, "策略编号", LVCFMT_LEFT, base);
	m_listStrategyInfo.InsertColumn(1, "状态", LVCFMT_LEFT, base/2);
	m_listStrategyInfo.InsertColumn(2, "策略对", LVCFMT_LEFT, nlen + base/2);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
				  
}

void CStrategyInfoDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	if (GetSafeHwnd())
	{
		if (m_listStrategyInfo.GetSafeHwnd())
		{
			CRect rect(0,0,cx,cy);
			m_listStrategyInfo.MoveWindow(&rect);
		}
	}
	// TODO: Add your message handler code here
	
}

void CStrategyInfoDlg::OnRclickListStrategyInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	DWORD   dwPos   =   GetMessagePos(); 
	CPoint   point1(   LOWORD(dwPos),   HIWORD(dwPos)   ); 
	CPoint   point2(   LOWORD(dwPos),   HIWORD(dwPos)   ); 
	
	m_listStrategyInfo.ScreenToClient(&point2); 
	m_nRSelItem   =   m_listStrategyInfo.HitTest(point2); 
	
	if   (point1.x   ==   -1   &&   point1.y   ==   -1) 
		point1.Offset(5,   5); 
	
	CMenu   menu; 
	VERIFY(menu.LoadMenu(IDR_POP_MENU)); 
	CMenu*   pPopup   =   menu.GetSubMenu(0); 
	ASSERT(pPopup   !=   NULL); 
	SetMenuState(m_nRSelItem, pPopup);
	
	pPopup-> TrackPopupMenu(TPM_LEFTALIGN   |   TPM_RIGHTBUTTON,   point1.x,   point1.y,   this); 

	*pResult = 0;

}

void CStrategyInfoDlg::SetMenuState(int nItem, CMenu*   pPopup)
{
	char szStrategyName[32];
	memset(szStrategyName, 0, sizeof(szStrategyName));
	m_listStrategyInfo.GetItemText(nItem, 0, szStrategyName, sizeof(szStrategyName));
	string strategyName(szStrategyName);
	TStrategyInfo* ret = FindSGstatemap(strategyName);
	if (ret == NULL)
	{
		pPopup->EnableMenuItem(0,MF_BYPOSITION|MF_GRAYED);
		pPopup->EnableMenuItem(1,MF_BYPOSITION|MF_GRAYED);
		pPopup->EnableMenuItem(3,MF_BYPOSITION|MF_GRAYED);
		pPopup->EnableMenuItem(4,MF_BYPOSITION|MF_GRAYED);
		return;
	}
	switch(ret->tStrategyUI.sgstate) {
	case WORK://开始
		pPopup->EnableMenuItem(0,MF_BYPOSITION|MF_ENABLED);
		pPopup->EnableMenuItem(1,MF_BYPOSITION|MF_GRAYED);
		pPopup->EnableMenuItem(3,MF_BYPOSITION|MF_GRAYED);
		pPopup->EnableMenuItem(4,MF_BYPOSITION|MF_GRAYED);
		break;
	case STOP://停止
		pPopup->EnableMenuItem(0,MF_BYPOSITION|MF_GRAYED);
		pPopup->EnableMenuItem(1,MF_BYPOSITION|MF_ENABLED);
		pPopup->EnableMenuItem(3,MF_BYPOSITION|MF_ENABLED);
		pPopup->EnableMenuItem(4,MF_BYPOSITION|MF_ENABLED);

		break;
	default:
		pPopup->EnableMenuItem(0,MF_BYPOSITION|MF_GRAYED);
		pPopup->EnableMenuItem(1,MF_BYPOSITION|MF_GRAYED);
		pPopup->EnableMenuItem(3,MF_BYPOSITION|MF_GRAYED);
		pPopup->EnableMenuItem(4,MF_BYPOSITION|MF_GRAYED);
		break;
	}
}

TIPState* CStrategyInfoDlg::FindIPstatemap(string strategyName)
{
	IPStateMap::iterator iter = m_mapIPState.find(strategyName);
	if( iter != m_mapIPState.end())	return iter->second;
	else 	return NULL;
}

TStrategyInfo* CStrategyInfoDlg::FindSGstatemap(string szname)
{
	TStrategyInfo* ret = NULL;
	CMainFrame * pMainFrame = NULL;
	pMainFrame = ((CMainFrame*)::AfxGetMainWnd());

	CString strName = szname.c_str();
	int pos = strName.Find("_");
	if (pos <= 0)
		return ret;
    strName = strName.Mid(0, pos);

	IPInfoMap::iterator iter = pMainFrame->m_IPInfoMap.find(strName.GetBuffer(strName.GetLength()));
	assert( iter != pMainFrame->m_IPInfoMap.end());
	TIPInfo *pIPInfo = iter->second;

	
	list<TStrategyInfo*> ::iterator it ;
	CStrategyDlg strategyDlg;

	for(it = pIPInfo->strategyInfoList.begin(); it != pIPInfo->strategyInfoList.end(); ++it)// list
	{
		if(strcmp((*it)->tStrategyUI.sName ,szname.c_str()) == 0)
		{
			ret = (*it);
			break;
		}
	}
	return ret;
}

BOOL CStrategyInfoDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message==WM_KEYDOWN && (pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE)) 
		return TRUE;
	else 
		return CDialog::PreTranslateMessage(pMsg); 
}



void CStrategyInfoDlg::OnDblclkListStrategyInfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE temp = (LPNMITEMACTIVATE) pNMHDR;
	
	int nItem=temp->iItem;
	char szStrategyName[32];
	memset(szStrategyName, 0, sizeof(szStrategyName));
	m_listStrategyInfo.GetItemText(nItem, 0, szStrategyName, sizeof(szStrategyName));
	DayTradingStrategy * dts = DayTradingStrategy::get(szStrategyName);
	if(dts != NULL)
		dts->shootAnyway();
	*pResult = 0;
}
