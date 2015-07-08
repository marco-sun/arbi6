#pragma once
#include "fixthread.h"
#include "patsintf.h"
#include "patsapimsgdef.h"
#include "patsapiinnermsgdef.h"
#include <list>
using namespace std;

// Engine State 0-born(start state), 1-intialised, 2-log in, 3-partial log out, 4-total log out(end state)
#define ES_BORN (0) 
#define ES_INIT (1) 
#define ES_LOGIN (2) 
#define ES_LOGOUT (3) 
#define ES_DEAD (4) 

class SimPatsApiEngine : public FixThread
{
public:
	static IMsg* getPriceEngineInstance(IMsg *priceSource);
	static IMsg* getDealerEngineInstance(IMsg *dealer);
	static IMsg* getEngineInstance();
public:
	virtual ~SimPatsApiEngine(void);

	lp_ptGetLogonStatus ptGetLogonStatus;
	lp_ptGetPriceForContract ptGetPriceForContract;
	lp_ptGetOrderByID ptGetOrderByID;
	lp_ptGetFillByID ptGetFillByID;

protected:
	bool OnMsg(TRawMsg *msg, IMsg *remote_src);
	BOOL Prepare(long *pnResult);
	BOOL Cleanup(long *pnResult);
private:
	SimPatsApiEngine(void);
	void ReadIniConfig();
	int Initialise(void);
	int SetupDll(void);
	int SetupCallbacks(void);
	int SetupConnection(void);
	void Login(void);
	void LogOut(TPatsApiLogoutMsg* pMsg);
	void NotifyEngineStatus();
	void GetPrice(TPatsApiCPReqMsg* pCPReqMsg);
	void PrintPriceDetail(const char * tag, PriceDetailStruct& priceDetail);
	void SubPrice(TPatsApiMDReqMsg* pMDReqMsg);
	void SubPrice2PriceServer();
	void AddOrder(TPatsApiAddOrderMsg* pAOMsg);
	void CancelOrder(TPatsApiCancelOrderMsg* pCOMsg);
	void AddOrder_Reject(TPatsApiAddOrderMsg* pAOMsg);
	void GetAllContractInfo(void);

	// inner handler
	void OnInnerOR(TPatsApiInnerORMsg* pMsg);
	void OnInnerFR(TPatsApiInnerFRMsg* pMsg);
	void OnInnerMsg(TPatsApiInnerMsgMsg* pMsg);

	// Status of the Engine
	// 0-born(start state), 1-intialised, 2-log in, 3-partial log out, 4-total log out(end state)
	int m_nEngineStatus;

	// config
	char m_szUsername[64];
	char m_szPassword[64];
	char m_szHostServerIP[16];
	char m_szPriceServerIP[16];
	int	m_nHostServerPort;
	int m_nPriceServerPort;

	U8 m_HostLink_Status;
	U8 m_PriceLink_Status;
	FixThreadTimer  m_local_tm;
	FixThreadTimer  local_tm;
	HINSTANCE hInstance; //handle to the PATS DLL
	IMsg* pDealer;
	IMsg* pPriceSource;
	static SimPatsApiEngine* pStaticEngine;

	map<string,int> subPriceMap;
	CRITICAL_SECTION cs;
	map<string,string>	user2PatsMap;
	map<string,string>	pats2UserMap;



	//DLL 中的方法映射
	lp_ptInitialise ptInitialise;
	lp_ptSetHostAddress ptSetHostAddress;
	lp_ptSetPriceAddress ptSetPriceAddress;
	lp_ptSetHostReconnect ptSetHostReconnect;
	lp_ptSetPriceReconnect ptSetPriceReconnect;
	lp_ptReady ptReady;
	lp_ptEnable ptEnable;
	lp_ptDisable ptDisable;
	lp_ptSubscribePrice ptSubscribePrice;
	lp_ptUnsubscribePrice ptUnsubscribePrice;
	lp_ptNotifyAllMessages ptNotifyAllMessages;
	lp_ptSetPriceAgeCounter ptSetPriceAgeCounter;

	lp_ptCountExchanges ptCountExchanges;
	lp_ptGetExchange ptGetExchange;
	lp_ptGetExchangeByName ptGetExchangeByName;
	lp_ptExchangeExists ptExchangeExists;
	lp_ptGetExchangeRate ptGetExchangeRate;
	lp_ptCountTraders ptCountTraders;
	lp_ptGetTrader ptGetTrader;
	lp_ptTraderExists ptTraderExists;
	lp_ptCountOrderTypes ptCountOrderTypes;
	lp_ptGetOrderType ptGetOrderType;
	lp_ptCountReportTypes ptCountReportTypes;
	lp_ptGetReportType ptGetReportType;
	lp_ptReportTypeExists ptReportTypeExists;
	lp_ptGetReportSize ptGetReportSize;
	lp_ptGetReport ptGetReport;
	lp_ptCountCommodities ptCountCommodities;
	lp_ptGetCommodity ptGetCommodity;
	lp_ptCommodityExists ptCommodityExists;
	lp_ptGetCommodityByName ptGetCommodityByName;
	lp_ptCountContracts ptCountContracts;
	lp_ptGetContract ptGetContract;
	lp_ptGetExtendedContract ptGetExtendedContract;
	lp_ptContractExists ptContractExists;
	lp_ptGetContractByName ptGetContractByName;
	lp_ptGetExtendedContractByName ptGetExtendedContractByName;
	lp_ptCountOrderHistory ptCountOrderHistory;
	lp_ptGetOrderHistory ptGetOrderHistory;

	lp_ptLogOn ptLogon;
	lp_ptLogOff ptLogoff;
	lp_ptDisconnect ptDisconnect;
	lp_ptDOMEnabled	ptDOMEnabled;
	lp_ptPostTradeAmendEnabled	ptPostTradeAmendEnabled;
	lp_ptCountUsrMsgs ptCountUsrMsgs;
	lp_ptGetUsrMsg ptGetUsrMsg;
	lp_ptGetUsrMsgByID ptGetUsrMsgByID;					
	lp_ptAcknowledgeUsrMsg ptAcknowledgeUsrMsg;

	lp_ptRegisterCallback ptRegisterCallback;
	lp_ptRegisterLinkStateCallback ptRegisterLinkStateCallback;
	lp_ptRegisterMsgCallback ptRegisterMsgCallback;
	lp_ptRegisterPriceCallback ptRegisterPriceCallback;
	lp_ptRegisterOrderCallback ptRegisterOrderCallback;
	lp_ptRegisterFillCallback ptRegisterFillCallback;
	lp_ptRegisterContractCallback ptRegisterContractCallback;
	lp_ptRegisterStatusCallback ptRegisterStatusCallback;

	lp_ptCountFills ptCountFills;
	lp_ptGetFill ptGetFill;
	lp_ptGetContractPosition ptGetContractPosition;
	lp_ptGetOpenPosition ptGetOpenPosition;

	lp_ptGetTotalPosition ptGetTotalPosition;
	lp_ptGetAveragePrice ptGetAveragePrice;
	lp_ptBlankPrices ptBlankPrices;
	lp_ptGetPrice ptGetPrice;
	
	lp_ptAddOrder ptAddOrder;
	lp_ptAmendOrder ptAmendOrder;
	lp_ptCancelOrder ptCancelOrder;
	lp_ptCancelBuys ptCancelBuys;
	lp_ptCancelSells ptCancelSells;
	
	lp_ptCancelAll ptCancelAll;
	lp_ptQueryOrderStatus ptQueryOrderStatus;
	lp_ptOrderChecked ptOrderChecked;
	lp_ptCountOrders ptCountOrders;
	lp_ptGetOrder ptGetOrder;
	
	
	lp_ptSetEncryptionCode ptSetEncryptionCode;
	lp_ptSetHandShakePeriod ptSetHandShakePeriod;
	lp_ptSetInternetUser ptSetInternetUser;
	

};

