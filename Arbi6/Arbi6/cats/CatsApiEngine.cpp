#include "CatsApiEngine.h"
#include "../arbi/PriceSource.h"
#include "../arbi/Contract.h"
#include "../LogStore.h"
#include <time.h>

CatsOrderInventory::CatsOrderInventory(Order *pOrder)
{
	order = pOrder;
	orderNo = "";
	status = -1;

}

CatsOrderInventory::~CatsOrderInventory()
{
}

CatsOrderInventory* CatsApiEngine::addInventory(string orderNo, Order *order)
{
	CatsOrderInventory* ret = NULL;
	string key = order->getId();
	//看是否有重复的key
	if(user2CatsMap.count(key)!=0)
	{
		return ret;
	}
	user2CatsMap[key] = order->getId();
	ret = new CatsOrderInventory(order);
	orderInventoryMap.insert(make_pair(orderNo,ret));
	return ret;
}

bool CatsApiEngine::delInventory(CatsOrderInventory* pDealerInventory)
{
	user2CatsMap.erase(pDealerInventory->order->getId());
	orderInventoryMap.erase(pDealerInventory->orderNo);
	delete pDealerInventory;
	return TRUE;
}

CatsOrderInventory* CatsApiEngine::getInventory(string orderNo)
{
	CatsOrderInventory* ret = NULL;
	map<string, CatsOrderInventory*>::iterator iter = orderInventoryMap.find(orderNo);
	if(iter != orderInventoryMap.end()) 
		ret = iter->second;
	return ret;
}

CatsOrderInventory* CatsApiEngine::findInventorybyOrderId(string orderId)
{
	CatsOrderInventory* ret = NULL;
	map<string, string>::iterator iter1 = user2CatsMap.find(orderId);
	if(iter1 != user2CatsMap.end()) 
	{
		string orderNo = iter1->second;
		map<string, CatsOrderInventory*>::iterator iter2 = orderInventoryMap.find(orderNo);
		if(iter2 != orderInventoryMap.end())
		{
			ret = iter2->second;
		}
	}
	return ret;
}


CatsApiEngine* CatsApiEngine::pStaticEngine = NULL;

CatsApiEngine* CatsApiEngine::getEngineInstance()
{
	if(pStaticEngine==NULL)
	{
		pStaticEngine = new CatsApiEngine();
		pStaticEngine->ReadIniConfig();
		if(pStaticEngine->init())
		{
			TRACE_LOG("[ENGINE]CatsApiEngine Created OK.");
		}
		else
		{
			TRACE_LOG("[ENGINE]CatsApiEngine Create FAIL!");
			delete pStaticEngine;
			pStaticEngine = NULL;
		}
	}
	return pStaticEngine; 
}

CatsApiEngine::CatsApiEngine(void)
{
	m_initStep = "";
	m_nEngineStatus = CATS_ES_BORN;
	m_contracts.clear();
	//cats2UserMap.clear();
	user2CatsMap.clear();
	InitializeCriticalSection(&cs);
}

CatsApiEngine::~CatsApiEngine(void)
{
	DeleteCriticalSection(&cs);
}

void CatsApiEngine::add(string stock)
{
	if(m_nEngineStatus==CATS_ES_BORN || m_nEngineStatus==CATS_ES_INIT)
	{
		m_contracts.insert(stock);
	}
	else if(m_nEngineStatus==CATS_ES_LOGIN)
	{
		listenStock(stock);
	}
}

bool CatsApiEngine::placeOrder(Order* order)
{
	string stock = order->getContract() + ".SH";
	char szQuant[16];
	sprintf(szQuant,"%d",order->getLot());
	string orderQty = szQuant;
	sprintf(szQuant,"%.3f",order->getPrice());
	string orderPrice = szQuant;
	string tradeSide = (order->getBuySell()==BUY)?"1":"2";

	CatsOrderRef* pRef = new CatsOrderRef();
	pRef->pEngine = this;
	pRef->pOrder = order;

	//下单,下单后有资金、委托流水更新推送,如果有成交，将有持仓更新推送
    CATSAPI_Prepare_OrderSingle(m_hHandle);
	CATSAPI_SetParam(m_hHandle, "catsAcct", m_sCatsAcct.c_str());
    CATSAPI_SetParam(m_hHandle, "catsToken", m_sCatsToken.c_str());
    CATSAPI_SetParam(m_hHandle, "acctType", m_sAcctType.c_str());
    CATSAPI_SetParam(m_hHandle, "acct", m_sAcctId.c_str());
    CATSAPI_SetParam(m_hHandle, "symbol", stock.c_str());
    CATSAPI_SetParam(m_hHandle, "orderQty", orderQty.c_str());
    CATSAPI_SetParam(m_hHandle, "orderPrice", orderPrice.c_str());
	// 1-买入 或 担保品买入 2-卖出 或 担保品卖出 F-ETF申购 G-ETF赎回
    CATSAPI_SetParam(m_hHandle, "tradeSide", tradeSide.c_str());
	// 0限价单 Q市价单（对手方最优价格） R市价单(最优五档即时成交剩余转限价) 
	// S市价单(本方最优价格) T市价单(即时成交剩余撤销) U市价单(最优五档即时成交剩余撤销) V市价单(全额成交或撤单)
    CATSAPI_SetParam(m_hHandle, "orderType", "0"); 
    CATSAPI_SetParam(m_hHandle, "orderParam", "");
    CATSAPI_SetParam(m_hHandle, "corrType", "");
    CATSAPI_SetParam(m_hHandle, "corrId", "");
    if(0!=CATSAPI_Execute_OrderSingle(m_hHandle, OnSingleOrder, pRef))
    {
		char errMsg[1024]={0};
        CATSAPI_GetLastError(m_hHandle, errMsg, sizeof(errMsg));
		logerror("下单失败![%s]",errMsg);
		delete pRef;
        return false;
    }
	return true;
}

bool CatsApiEngine::cancelOrder(Order* order)
{
	CatsOrderInventory* catsOrder = NULL;
	EnterCriticalSection(&cs);
	catsOrder = findInventorybyOrderId(order->getId());
	LeaveCriticalSection(&cs);
	if (catsOrder==NULL)
	{
		return false;
	}
	string orderNo = catsOrder->orderNo;

	//撤单,撤单后有资金、委托流水更新推送
    CATSAPI_Prepare_WithDrawSingle(m_hHandle);
	CATSAPI_SetParam(m_hHandle, "catsAcct", m_sCatsAcct.c_str());
    CATSAPI_SetParam(m_hHandle, "catsToken", m_sCatsToken.c_str());
    CATSAPI_SetParam(m_hHandle, "acctType", m_sAcctType.c_str());
    CATSAPI_SetParam(m_hHandle, "acct", m_sAcctId.c_str());
    CATSAPI_SetParam(m_hHandle, "orderNo", orderNo.c_str());
    CATSAPI_SetParam(m_hHandle, "corrType", "");
    CATSAPI_SetParam(m_hHandle, "corrId", "");
    if(0!=CATSAPI_Execute_WithDrawSingle(m_hHandle, OnCancelOrder, this))
    {
		char errMsg[1024]={0};
        CATSAPI_GetLastError(m_hHandle, errMsg, sizeof(errMsg));
		logerror("撤单失败![%s]",errMsg);
        return false;
    }
	return true;
}

void CatsApiEngine::ReadIniConfig()
{
	char sTradingServerAddr[64];
	char sTradingServerPort[64];
	char sHqServerAddr[64];
	char sHqServerPort[64];
	char sCatsAcct[64];
	char sCatsPwd[64];
	char sAcctId[64];
	char sAcctPwd[64];

	GetPrivateProfileStringA("CATSAPIENGINE", "TradingServerAddr", "0", sTradingServerAddr, 64, ".\\AIB.ini");
	GetPrivateProfileStringA("CATSAPIENGINE", "TradingServerPort", "0", sTradingServerPort, 64, ".\\AIB.ini");
	GetPrivateProfileStringA("CATSAPIENGINE", "HqServerAddr", "0", sHqServerAddr, 64, ".\\AIB.ini");
	GetPrivateProfileStringA("CATSAPIENGINE", "HqServerPort", "0", sHqServerPort, 64, ".\\AIB.ini");
	GetPrivateProfileStringA("CATSAPIENGINE", "CatsAcct", "100010", sCatsAcct, 64, ".\\AIB.ini");
	GetPrivateProfileStringA("CATSAPIENGINE", "CatsPwd", "111111", sCatsPwd, 64, ".\\AIB.ini");
	GetPrivateProfileStringA("CATSAPIENGINE", "AcctId", "100999988", sAcctId, 64, ".\\AIB.ini");
	GetPrivateProfileStringA("CATSAPIENGINE", "AcctPwd", "111111", sAcctPwd, 64, ".\\AIB.ini");
	m_sTradingServerAddr = sTradingServerAddr;
	m_sTradingServerPort = sTradingServerPort;
	m_sHqServerAddr = sHqServerAddr;
	m_sHqServerPort = sHqServerPort;
	m_sCatsAcct = sCatsAcct;
	m_sCatsPwd = sCatsPwd;
	m_sAcctType = "S0";
	m_sAcctId = sAcctId;
	m_sAcctPwd = sAcctPwd;

	TRACE_LOG("[CATSAPIENGINE]ReadIniConfig TradeServer:[%s:%s],MDServer[%s:%s],CatsUser[%s:%s],StockUser[%s:%s].",
		sTradingServerAddr,sTradingServerPort,sHqServerAddr,sHqServerPort,sCatsAcct,sCatsPwd,sAcctId,sAcctPwd);
}

bool CatsApiEngine::init()
{
	//初始化CATS API库
	int nRet = CATSAPI_Init(0);
	if(nRet!=0)
	{
		logdebug("Init catsapi lib error!");
		return false;
	}
	nRet = CATSAPI_InitSession(&m_hHandle, 0, 1, CATSAPIOnTrdReconnected, this,CATSAPIOnTrdDisconnected,this,CATSAPIOnHqReconnected,this,CATSAPIOnHqDisconnected,this);
	if(nRet!=0)
	{
		logdebug("Init catsapi session error!");
		return false;
	}
	//连接CATS 服务器
	CATSAPI_Prepare_CatsConnect(m_hHandle);
	//设置参数
	CATSAPI_SetParam(m_hHandle, "serverName", "TAS");
	CATSAPI_SetParam(m_hHandle, "user","catsuser"); 
	CATSAPI_SetParam(m_hHandle, "password", "GV2gODkBbGg=");
	CATSAPI_SetParam(m_hHandle, "tradingServerAddr", m_sTradingServerAddr.c_str());
	CATSAPI_SetParam(m_hHandle, "tradingServerPort", m_sTradingServerPort.c_str());
	CATSAPI_SetParam(m_hHandle, "hqServerAddr", m_sHqServerAddr.c_str());
	CATSAPI_SetParam(m_hHandle, "hqServerPort", m_sHqServerPort.c_str());
	CATSAPI_SetParam(m_hHandle, "srcPhyAddr", "");
	CATSAPI_SetParam(m_hHandle, "srcIpAddr", "");
	if(0!=CATSAPI_Execute_CatsConnect(m_hHandle, OnCATSConnected, this))
	{
		char errMsg[1024]={0};
		CATSAPI_GetLastError(m_hHandle, errMsg, sizeof(errMsg));
		logdebug("Connect to catsserver error![%s]",errMsg);
		return false;
	}
	return true;
}

bool CatsApiEngine::logonCATS()
{
	//CATS 账户登录
    CATSAPI_Prepare_CatsLogin(m_hHandle);

    CATSAPI_SetParam(m_hHandle, "catsAcct", m_sCatsAcct.c_str());
    CATSAPI_SetParam(m_hHandle, "catsPassword", m_sCatsPwd.c_str());
	//logdebug("Cats login catsAcct[%s] catsPassword[%s]",m_sCatsAcct.c_str(), m_sCatsPwd.c_str());
    if(0!=CATSAPI_Execute_CatsLogin(m_hHandle, OnCATSUserLogin, this))
    {
		char errMsg[1024]={0};
        CATSAPI_GetLastError(m_hHandle, errMsg, sizeof(errMsg));
		logerror("Cats login error![%s]",errMsg);
        return false;
    }
	return true;
}

bool CatsApiEngine::logonAccount()
{
	//交易账户登录
    CATSAPI_Prepare_TradeAccLogin(m_hHandle);
	CATSAPI_SetParam(m_hHandle, "catsAcct", m_sCatsAcct.c_str());
    CATSAPI_SetParam(m_hHandle, "catsToken", m_sCatsToken.c_str());
    CATSAPI_SetParam(m_hHandle, "acctType", m_sAcctType.c_str());
    CATSAPI_SetParam(m_hHandle, "acct",m_sAcctId.c_str() );
    CATSAPI_SetParam(m_hHandle, "password",m_sAcctPwd.c_str() );
    CATSAPI_SetParam(m_hHandle, "loginParam","");
    CATSAPI_SetParam(m_hHandle, "srcIpAddr","");
    CATSAPI_SetParam(m_hHandle, "srcPhyAddr","");
    if(0!=CATSAPI_Execute_TradeAccLogin(m_hHandle, OnTradeAcctLogin, this))
    {
		char errMsg[1024]={0};
        CATSAPI_GetLastError(m_hHandle, errMsg, sizeof(errMsg));
		logerror("Trade acct login error![%s]",errMsg);
        return false;
    }
	return true;
}

bool CatsApiEngine::listenStock(string stock)
{
	//订阅行情更新，订阅成功行情变化将有行情更新数据推送
	//行情增量传输，不变的传负值或空
    CATSAPI_PreSub_MarketData(m_hHandle);
    CATSAPI_SetParam(m_hHandle, "catsAcct", m_sCatsAcct.c_str());
    CATSAPI_SetParam(m_hHandle, "catsToken", m_sCatsToken.c_str());
	string sSymbol = stock + ".SH";
    CATSAPI_SetParam(m_hHandle, "symbol", sSymbol.c_str());
    if(0 != CATSAPI_Subscribe_MarketData(m_hHandle, PubMarketDataCallback,this , BizSubMDCallback, this))
    {
        char errMsg[1024] = {0};
        CATSAPI_GetLastError(m_hHandle, errMsg, sizeof(errMsg));
		logerror("订阅%s的实时行情失败(%s)", stock.c_str(), errMsg);
		return false;
    }
	return true;
}

bool CatsApiEngine::subOrderUpdate()
{
	//订阅委托推送
    CATSAPI_PreSub_OrderUpdate(m_hHandle);
    CATSAPI_SetParam(m_hHandle, "catsAcct", m_sCatsAcct.c_str());
    CATSAPI_SetParam(m_hHandle, "catsToken", m_sCatsToken.c_str());
    CATSAPI_SetParam(m_hHandle, "acctType", m_sAcctType.c_str());
    CATSAPI_SetParam(m_hHandle, "acct", m_sAcctId.c_str());
    if(0 != CATSAPI_Subscribe_OrderUpdate(m_hHandle, PubOrderUpdateCallback, this, BizSubCommonCallback, this))
    {
        char errMsg[1024] = {0};
        CATSAPI_GetLastError(m_hHandle, errMsg, sizeof(errMsg));
		logerror("订阅%s的委托流水失败(%s)", m_sAcctId.c_str(), errMsg);
		return false;
    }
	m_initStep = "SubOrder";
	return true;
}

bool CatsApiEngine::subTrade()
{
	//订阅成交推送
    CATSAPI_PreSub_Trade(m_hHandle);
    CATSAPI_SetParam(m_hHandle, "catsAcct", m_sCatsAcct.c_str());
    CATSAPI_SetParam(m_hHandle, "catsToken", m_sCatsToken.c_str());
    CATSAPI_SetParam(m_hHandle, "acctType", m_sAcctType.c_str());
    CATSAPI_SetParam(m_hHandle, "acct", m_sAcctId.c_str());
    CATSAPI_SetParam(m_hHandle, "notPubHist", "false");
    if(0 != CATSAPI_Subscribe_Trade(m_hHandle, PubTradeUpdateCallback, this, BizSubCommonCallback, this))
    {
        char errMsg[1024] = {0};
        CATSAPI_GetLastError(m_hHandle, errMsg, sizeof(errMsg));
		logerror("订阅%s的成交流水失败(%s)", m_sAcctId.c_str(), errMsg);
		return false;
    }
	m_initStep = "SubTrade";
	return true;
}

bool CatsApiEngine::insertOrder(string stock, string orderQty, string orderPrice, string tradeSide)
{
	return true;
}

bool CatsApiEngine::cancelOrder(string orderNo)
{
	return true;
}

void CatsApiEngine::subscribeBatch()
{
	TRACE_LOG("CATS网关异步订阅价格, 待订阅合约共%d个.",m_contracts.size());
	set<string>::iterator iter = m_contracts.begin();
	while(iter!=m_contracts.end())
	{
		string contract = *iter;
		bool bResult = listenStock(contract);
		TRACE_LOG("[CatsApiEngine]Send subscribe for contract %s, and result %s.", contract.c_str(), (bResult ? "成功" : "失败") ) ;
		iter++;
	}
}

bool CatsApiEngine::uninit()
{
	CATSAPI_ExitSession(m_hHandle);
	CATSAPI_Fini();
	return true;
}

void CatsApiEngine::updateOrderInfo(CatsOrderInfo& orderInfo)
{
	CatsOrderInventory* catsOrder = NULL;
	EnterCriticalSection(&cs);
	catsOrder = getInventory(orderInfo.orderNo);
	if(catsOrder!=NULL)
	{
		//更新报单状态
		if(orderInfo.status == 5)
		{
			TRACE_DEBUG("[Cats网关回调]报单状态通知,判断为报单被拒.将汇报.");
			catsOrder->order->setRejected();
			delInventory(catsOrder);
		}
		else if(orderInfo.status == 4)//orderInfo.cancelQty == orderInfo.orderQty)
		{
			TRACE_DEBUG("[Cats网关回调]报单状态通知,判断为撤单成功.将汇报.");
			catsOrder->order->setCancel();
			delInventory(catsOrder);
		}
		else if(orderInfo.status == 2)//orderInfo.filledQty == orderInfo.orderQty)
		{
			TRACE_DEBUG("[Cats网关回调]报单状态通知,判断为全部成交.将汇报.");
			catsOrder->order->addDetail(new OrderDetail(catsOrder->order->getId(),orderInfo.orderQty,orderInfo.avgPrice));
			delInventory(catsOrder);
		}
		else
		{// 其他状态，仅打印
			TRACE_DEBUG("[Cats网关回调]报单状态通知, 状态%d.", orderInfo.status);
		}
	}
	LeaveCriticalSection(&cs);
}

void CatsApiEngine::updateTradeInfo(CatsTradeInfo& tradeInfo)
{ // cats的成交通知还未正式开放
	//EnterCriticalSection(&cs);
	//LeaveCriticalSection(&cs);
}

void CatsApiEngine::cancelOrderResp(Order* pOrder)
{
}

void CatsApiEngine::orderResp(Order* pOrder, string orderNo)
{
	EnterCriticalSection(&cs);
	this->addInventory(orderNo, pOrder);
	LeaveCriticalSection(&cs);
}


//**********************************************************************************************
//***********************************回调函数***************************************************
//**********************************************************************************************
void CatsApiEngine::CATSAPIReadString( CATSHANDLE hCats, const char* key, string& outStr )
{
	char buff[1024]={0};
    CATSAPI_GetCStrField(hCats, key, buff, sizeof(buff)-1);
    outStr = string(buff);
}

// 用于订阅的异步应答，仅需判断是否失败
void CatsApiEngine::BizSubCommonCallback(void* pArg)
{
	CatsApiEngine* pCB = (CatsApiEngine*)pArg;
	int nErrorCode = 0;
	char errMsg[1024]={0};
	nErrorCode = CATSAPI_GetLastError(pCB->m_hHandle, errMsg, sizeof(errMsg));
	if(nErrorCode!=0)
	{
		logdebug("Biz Sub failed! ErrorMsg:%s",errMsg);
	}
	else
	{ // Next step due to m_initStep
		if(pCB->m_initStep == "SubOrder")
		{
			logdebug("订阅报单状态成功");
			pCB->subTrade();
		}
		else if(pCB->m_initStep == "SubTrade")
		{ // 初始化流程结束, 通知上层
			logdebug("订阅成交通知成功");
			// 设置状态，订阅set中的行情
			pCB->m_nEngineStatus = CATS_ES_LOGIN;
			pCB->subscribeBatch();
		}
		else
		{
			logdebug("其他订阅成功");
		}
	}
}

// 用于订阅的异步应答，仅需判断是否失败
void CatsApiEngine::BizSubMDCallback(void* pArg)
{
	CatsApiEngine* pCB = (CatsApiEngine*)pArg;
	int nErrorCode = 0;
	char errMsg[1024]={0};
	nErrorCode = CATSAPI_GetLastError(pCB->m_hHandle, errMsg, sizeof(errMsg));
	if(nErrorCode!=0)
	{
		logdebug("Biz Sub failed! ErrorMsg:%s",errMsg);
	}
	else
	{
		logdebug("订阅价格成功");
	}
}

//交易服务器断后重连成功回调函数
void CatsApiEngine::CATSAPIOnTrdReconnected(void *)
{
	logdebug("Trade server reconnect success!");
}
//交易服务器断开连接回调函数
void CatsApiEngine::CATSAPIOnTrdDisconnected(void *)
{
	logdebug("Trade server disconnected!");
}
//行情服务器断后重连成功回调函数
void CatsApiEngine::CATSAPIOnHqReconnected(void *)
{
	logdebug("Hq server reconnect success!");
}
//行情服务器断开连接回调函数
void CatsApiEngine::CATSAPIOnHqDisconnected(void *)
{
	logdebug("Hq server disconnected!");
}
//CATS服务器连接回调
void CatsApiEngine::OnCATSConnected(void* pArg)
{
	CatsApiEngine* pCB = (CatsApiEngine*)pArg;
	int nErrorCode = 0;
	char errMsg[1024]={0};
	nErrorCode = CATSAPI_GetLastError(pCB->m_hHandle, errMsg, sizeof(errMsg));
	if(nErrorCode==0)
	{
		logdebug("Connect to catsserver success!");
		CATSAPI_Fetch_CatsConnectResult(pCB->m_hHandle);
		if(CATSAPI_GetNext(pCB->m_hHandle) == 0)
		{
			//获取返回内容 latestClientVersion,updateServerUsername,updateServerPassword,updateServerIP,updateServerPort,updateFileUrl
			
		}
		if(!pCB->logonCATS())
		{ // TODO: 登陆失败，通知上层
			;
		}
	}
	else
	{
		logdebug("Connect to catsserver failed! ErrorMsg:%s",errMsg);
	}
}

//cats账户登录回调
void CatsApiEngine::OnCATSUserLogin(void* pArg)
{
	CatsApiEngine* pCB = (CatsApiEngine*)pArg;
    //判断响应
	int nErrorCode = 0;
    char errMsg[1024]={0};
	nErrorCode = CATSAPI_GetLastError(pCB->m_hHandle, errMsg, sizeof(errMsg));
	if(nErrorCode==0)
	{
		CATSAPI_Fetch_CatsLoginResult(pCB->m_hHandle);
	    if(CATSAPI_GetNext(pCB->m_hHandle) == 0)
		{
			string sCatsAcct,sClientName,sClientType,sCertType,sCertNo,sCatsToken;
	        CATSAPIReadString(pCB->m_hHandle, "catsAcct", sCatsAcct);
	        CATSAPIReadString(pCB->m_hHandle, "clientName", sClientName);
	        CATSAPIReadString(pCB->m_hHandle, "clientType", sClientType);
	        CATSAPIReadString(pCB->m_hHandle, "certType", sCertType);
	        CATSAPIReadString(pCB->m_hHandle, "certNo", sCertNo);
	        CATSAPIReadString(pCB->m_hHandle, "catsToken", sCatsToken);

			logdebug("cats account:%s\nclientName:%s\nclientType:%s\ncertType:%s\n"
				"certNo:%s\ncatsToken:%s", sCatsAcct.c_str(),
				sClientName.c_str(), sClientType.c_str(),
				sCertType.c_str(), sCertNo.c_str(),
				sCatsToken.c_str());

			pCB->m_sCatsAcct = sCatsAcct;
			pCB->m_sCatsToken = sCatsToken;

			CATSAPI_Fetch_CatsSubAccResult(pCB->m_hHandle);
			while(CATSAPI_GetNext(pCB->m_hHandle) == 0)
			{
				string sAcctType,sAcct,sBranchName,sName,sMainFlag;
	            CATSAPIReadString(pCB->m_hHandle, "acctType", sAcctType);
	            CATSAPIReadString(pCB->m_hHandle, "acct", sAcct);
	            CATSAPIReadString(pCB->m_hHandle, "branchName", sBranchName);
	            CATSAPIReadString(pCB->m_hHandle, "name", sName);
	            CATSAPIReadString(pCB->m_hHandle, "mainFlag", sMainFlag); // 0-辅账户 1-主账户


				logdebug("subAcct:acctType:%s, acct:%s", sAcctType.c_str(), sAcct.c_str());
			}
			// 交易账户登陆
			if(!pCB->logonAccount())
			{ // TODO: 登陆失败，通知上层
				;
			}
		}
	}
	else
	{
		logerror("CATS login failed![%s]",errMsg);
	}
}

void CatsApiEngine::OnTradeAcctLogin(void* pArg)
{
	CatsApiEngine* pCB = (CatsApiEngine*)pArg;
    //判断响应
	int nErrorCode = 0;
    char errMsg[1024]={0};
	nErrorCode = CATSAPI_GetLastError(pCB->m_hHandle, errMsg, sizeof(errMsg));
	if(nErrorCode==0)
	{
		CATSAPI_Fetch_TradeAccLoginResult(pCB->m_hHandle);
	    if(CATSAPI_GetNext(pCB->m_hHandle) == 0)
		{ // acctType acct userToken branchName name
			string sAcctType,sAcctId;
	        CATSAPIReadString(pCB->m_hHandle, "acctType",sAcctType);
	        CATSAPIReadString(pCB->m_hHandle, "acct",sAcctId);
			logdebug("Trade acct login success![%s] [%s]",sAcctType.c_str(),sAcctId.c_str());
		}
		// 订阅报单回报
		if(!pCB->subOrderUpdate())
		{ // TODO: 订阅失败，通知上层
			;
		}
	}
	else
	{
		logerror("Trade acct login failed![%s]",errMsg);

	}
}


//委托回调
void CatsApiEngine::OnSingleOrder(void* pArg)
{
	CatsOrderRef* pRef = (CatsOrderRef*)pArg;
	CatsApiEngine* pCB = pRef->pEngine;
	Order* pOrder = pRef->pOrder;
	delete pRef;
    //判断响应
	string sOrderNo = "";
	int nErrorCode = 0;
    char errMsg[1024]={0};
	nErrorCode = CATSAPI_GetLastError(pCB->m_hHandle, errMsg, sizeof(errMsg));
	if(nErrorCode==0)
	{
		CATSAPI_Fetch_OrderSingleResult(pCB->m_hHandle);
	    if(CATSAPI_GetNext(pCB->m_hHandle) == 0)
		{
	        CATSAPIReadString(pCB->m_hHandle, "orderNo",sOrderNo);
			pCB->orderResp(pOrder,sOrderNo);
			logdebug("Order success!OrderNo:%s",sOrderNo.c_str());
		}
	}
	else
	{
		logerror("Order failed!ErrorMsg:%s",errMsg);
		pOrder->setRejected();
	}
}

//撤单回调
void CatsApiEngine::OnCancelOrder(void* pArg)
{
	CatsOrderRef* pRef = (CatsOrderRef*)pArg;
	CatsApiEngine* pCB = pRef->pEngine;
	Order* pOrder = pRef->pOrder;
	delete pRef;
    //判断响应
	int nErrorCode = 0;
    char errMsg[1024]={0};
	nErrorCode = CATSAPI_GetLastError(pCB->m_hHandle, errMsg, sizeof(errMsg));
	if(nErrorCode==0)
	{
		CATSAPI_Fetch_WithDrawSingleResult(pCB->m_hHandle);
	    if(CATSAPI_GetNext(pCB->m_hHandle) == 0)
		{
			string sOrderNo;
			string sOrigOrderNo;
			CATSAPIReadString(pCB->m_hHandle, "originalOrderNo",sOrigOrderNo);
	        CATSAPIReadString(pCB->m_hHandle, "orderNo",sOrderNo);
			logdebug("Cancel Order Accept!CancelOrderNo:%s",sOrderNo.c_str());
		}
	}
	else
	{
		logerror("CancelOrder failed! ErrorMsg:%s",errMsg);
		pOrder->setRejected();
	}
}

//行情推送回调
void CatsApiEngine::PubMarketDataCallback(void* pArg)
{
	CatsApiEngine* pCB = (CatsApiEngine*)pArg;
    CATSAPI_Fetch_MarketDataResult(pCB->m_hHandle);

    while(CATSAPI_GetNext(pCB->m_hHandle) == 0)
    {
		int nBufLen=0;
		char *pBuf = CATSAPI_GetBuffer(pCB->m_hHandle,&nBufLen);
		if(nBufLen<=0) return ;

		Pricing tmpPrice("N/A",0.0,0.0);
		//注意：增量传输，不变的数据传负值或空
		STRealMarketData_Depth *pRealMarketData = (STRealMarketData_Depth *)pBuf;
		string sSymbol = pRealMarketData->symbol;
		int pos = sSymbol.find_first_of(".");
		tmpPrice.contract = sSymbol.substr(0,pos);
//		if(pRealMarketData->stopMark==1)
//		{ // 停牌
//			instPrice.last = (double)pRealMarketData->prevClosePrice/10000;
//		}
//		else
		{
			tmpPrice.lastprice = (int32)pRealMarketData->lastPrice<0 ? -1 : (double)pRealMarketData->lastPrice/10000;
			tmpPrice.bid = (int32)pRealMarketData->bidPrice1<0 ? -1 : (double)pRealMarketData->bidPrice1/10000;
			tmpPrice.ask = (int32)pRealMarketData->askPrice1<0 ? -1 : (double)pRealMarketData->askPrice1/10000;
		}
		float lastPrice = (float)pRealMarketData->lastPrice/10000;
		float turnover = (float)pRealMarketData->turnOver/10000;
		logdebug("[MarketData]:symbol:%s,lastprice:%.3f turnover:%.3f",pRealMarketData->symbol,lastPrice,turnover);
		Contract::get(tmpPrice.contract)->setPrice(0,tmpPrice.ask,tmpPrice.bid, tmpPrice.lastprice);
    }

}

//委托推送回调
void CatsApiEngine::PubOrderUpdateCallback(void* pArg)
{
	CatsApiEngine* pCB = (CatsApiEngine*)pArg;
    CATSAPI_Fetch_OrderUpdateResult(pCB->m_hHandle);

    while(CATSAPI_GetNext(pCB->m_hHandle) == 0)
    {
        string strAcct;
        string strAcctType;
        CATSAPIReadString(pCB->m_hHandle, "acctType", strAcctType);
        CATSAPIReadString(pCB->m_hHandle, "acct", strAcct);

        CATSAPI_Fetch_OrderUpdateResult_Orders(pCB->m_hHandle);
        while(CATSAPI_GetNext(pCB->m_hHandle) == 0)
        {
			CatsOrderInfo orderInfo;
			string sOrderParam,sCorrType,sCorrId;
			CATSAPIReadString(pCB->m_hHandle, "symbol",orderInfo.stock);
			CATSAPI_GetIntField(pCB->m_hHandle, "orderQty",orderInfo.orderQty);
			CATSAPI_GetFloatField(pCB->m_hHandle, "orderPrice",orderInfo.orderPrice);
			CATSAPIReadString(pCB->m_hHandle, "tradeSide",orderInfo.tradeSide);
			CATSAPIReadString(pCB->m_hHandle, "orderType",orderInfo.orderType);
			CATSAPIReadString(pCB->m_hHandle, "orderParam",sOrderParam);
		    CATSAPIReadString(pCB->m_hHandle, "orderNo",orderInfo.orderNo);
		    CATSAPI_GetIntField(pCB->m_hHandle, "filledQty",orderInfo.filledQty);
		    CATSAPI_GetFloatField(pCB->m_hHandle, "avgPrice",orderInfo.avgPrice);
		    CATSAPI_GetIntField(pCB->m_hHandle, "cancelQty",orderInfo.cancelQty);
			CATSAPIReadString(pCB->m_hHandle, "orderTime",orderInfo.orderTime);
			// 0-新建 1-部分成交 2-完全成交 3-部分撤单 4-全部撤单 5-订单拒绝
		    CATSAPI_GetIntField(pCB->m_hHandle, "status",orderInfo.status);
			CATSAPIReadString(pCB->m_hHandle, "corrType",sCorrType);
			CATSAPIReadString(pCB->m_hHandle, "corrId",sCorrId);
			//此处省略其他字段
			logdebug("[OrderInfo] acct:%s symbol:%s orderQty:%d orderPrice:%.3f tradeSide:%s orderNo:%s filledQty:%d avgPrice:%.3f cancelQty:%d status:%d",
				strAcct.c_str(),orderInfo.stock.c_str(),orderInfo.orderQty,orderInfo.orderPrice,
				orderInfo.tradeSide.c_str(),orderInfo.orderNo.c_str(),orderInfo.filledQty,
				orderInfo.avgPrice,orderInfo.cancelQty,orderInfo.status);
			pCB->updateOrderInfo(orderInfo);
        }

    }

}

//成交推送回调
void CatsApiEngine::PubTradeUpdateCallback(void* pArg)
{
	CatsApiEngine* pCB = (CatsApiEngine*)pArg;
    CATSAPI_Fetch_TradeResult(pCB->m_hHandle);

    while(CATSAPI_GetNext(pCB->m_hHandle) == 0)
    {
		CatsTradeInfo tradeInfo;
		CATSAPIReadString(pCB->m_hHandle, "symbol", tradeInfo.stock);
        CATSAPIReadString(pCB->m_hHandle, "tradeSide", tradeInfo.tradeSide);
        CATSAPIReadString(pCB->m_hHandle, "orderNo",tradeInfo.orderNo );
        CATSAPIReadString(pCB->m_hHandle, "tradeNo",tradeInfo.tradeNo );
        CATSAPIReadString(pCB->m_hHandle, "quantity",tradeInfo.quantity );
        CATSAPIReadString(pCB->m_hHandle, "price",tradeInfo.price );
        CATSAPIReadString(pCB->m_hHandle, "time", tradeInfo.tradeTime);
        CATSAPIReadString(pCB->m_hHandle, "tradeType",tradeInfo.tradeType );
		loginfo("[Trade]symbol:%s tradeSide:%s orderNo:%s tradeNo:%s quantity:%s price:%s time:%s tradeType:%s",
			tradeInfo.stock.c_str(),tradeInfo.tradeSide.c_str(),tradeInfo.orderNo.c_str(),
			tradeInfo.tradeNo.c_str(),tradeInfo.quantity.c_str(),tradeInfo.price.c_str(),
			tradeInfo.tradeTime.c_str(),tradeInfo.tradeType.c_str());
		pCB->updateTradeInfo(tradeInfo);
    }

}






