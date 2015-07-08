// SfitDealer.cpp: implementation of the SfitDealer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SfitDealer.h"
#include "LogStore.h"
#include <process.h>
#include "Contract.h"

// 配置参数,将从配置文件中读出
static char FRONT_ADDR[128];
static TThostFtdcBrokerIDType	BROKER_ID;			// 经纪公司代码
static TThostFtdcInvestorIDType INVESTOR_ID;		// 投资者代码
static TThostFtdcPasswordType  PASSWORD;			// 用户密码

//******************class SfitDealerInventory**********************************
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
SfitDealerInventory::SfitDealerInventory(Order *pOrder)
{
	order = pOrder;
	localRef = 0;
	remoteRef = "";
	placeStatus = 0;
	dealedLot = 0;
}

SfitDealerInventory::~SfitDealerInventory()
{
}
//******************class SfitDealer**********************************
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SfitDealer::SfitDealer()
{
    started = false;
	hLogin = ::CreateEvent(NULL,TRUE,FALSE,NULL);
	ReadIniConfig();
	InitializeCriticalSection(&cs);
}

SfitDealer::~SfitDealer()
{
	if(started)
	{
		stop();
	}
	DeleteCriticalSection(&cs);
	::CloseHandle(hLogin);
}

SfitDealerInventory* SfitDealer::addInventory(Order *order)
{
	SfitDealerInventory* ret = NULL;
	string key = order->getId();
	//看是否有重复的key
	if(dealerInventoryMap.count(key)!=0)
	{
		return ret;
	}
	ret = new SfitDealerInventory(order);
	dealerInventoryMap.insert(make_pair(key,ret));
	return ret;
}

bool SfitDealer::delInventory(SfitDealerInventory* pDealerInventory)
{
	dealerInventoryMap.erase(pDealerInventory->order->getId());
	delete pDealerInventory;
	return TRUE;
}

SfitDealerInventory* SfitDealer::getInventory(string key)
{
	SfitDealerInventory* ret = NULL;
	map<string, SfitDealerInventory*>::iterator iter = dealerInventoryMap.find(key);
	if(iter == dealerInventoryMap.end()) return ret;
	return iter->second;
}

SfitDealerInventory* SfitDealer::findInventorybyLocal(int localRef)
{
	SfitDealerInventory* ret = NULL;
	map<string, SfitDealerInventory*>::iterator iter = dealerInventoryMap.begin();
	while(iter != dealerInventoryMap.end())
	{
		if(iter->second->localRef == localRef)
		{
			ret = iter->second;
			break;
		}
		iter ++;
	}
	return ret;
}

SfitDealerInventory* SfitDealer::findInventorybyRemote(string remoteRef)
{
	SfitDealerInventory* ret = NULL;
	map<string, SfitDealerInventory*>::iterator iter = dealerInventoryMap.begin();
	while(iter != dealerInventoryMap.end())
	{
		if(iter->second->remoteRef == remoteRef)
		{
			ret = iter->second;
			break;
		}
		iter ++;
	}
	return ret;
}

void SfitDealer::ReadIniConfig()
{
	char szFrontAddr[128];
	char szBrokerId[16];
	char szInvestorId[16];
	char szPassword[16];
	memset(szFrontAddr,  0, sizeof(szFrontAddr));
	memset(szBrokerId,   0, sizeof(szBrokerId));
	memset(szInvestorId, 0, sizeof(szInvestorId));
	memset(szPassword,   0, sizeof(szPassword));

	if(GetPrivateProfileString("TRADE", "FRONT_ADDR", "tcp://116.228.223.29:26205", szFrontAddr, 128, ".\\AIB.ini"))
	{
		strcpy_s(FRONT_ADDR, 128, szFrontAddr);
	}
	if(GetPrivateProfileString("TRADE", "BROKER_ID", "4010", szBrokerId, 16, ".\\AIB.ini"))
	{
        strcpy_s(BROKER_ID, 11, szBrokerId);
	}
	if(GetPrivateProfileString("TRADE", "INVESTOR_ID",  "80000003", szInvestorId, 16, ".\\AIB.ini"))
	{
        strcpy_s(INVESTOR_ID, 13, szInvestorId);
	}
	if(GetPrivateProfileString("TRADE", "PASSWORD", "999999", szPassword, 16, ".\\AIB.ini"))
	{
        strcpy_s(PASSWORD, 41, szPassword);
	}
}

// ret=1 means BZ
int SfitDealer::readCommodityProp(const char* szCommodityName)
{
	int ret = 0;
	char szProp[32];
	memset(szProp,  0, sizeof(szProp));

	GetPrivateProfileString("TRADE", szCommodityName, "N/A", szProp, 32, ".\\AIB.ini");
	if(strcmp("BZ",szProp)==0)
	{
		ret = 1;
	}
	return ret;
}

//同步的start,等登录完结才返回
void SfitDealer::start()
{
    if(started) {
	    TRACE_LOG("SfitDealer already started.");
		return;
	}
	pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi(".\\dealstream\\",false);			// 创建UserApi
	pUserSpi = new CSFITTraderSpi();
	pUserSpi->m_pDealer = this;
	pUserApi->RegisterSpi((CThostFtdcTraderSpi*)pUserSpi);			// 注册事件类
	pUserApi->SubscribePublicTopic(THOST_TERT_QUICK);				// 注册公有流
	pUserApi->SubscribePrivateTopic(THOST_TERT_QUICK);			// 注册私有流
	//pUserApi->SubscribePublicTopic(THOST_TERT_RESTART);				// 注册公有流
	//pUserApi->SubscribePrivateTopic(THOST_TERT_RESTART);			// 注册私有流
	
	pUserApi->RegisterFront(FRONT_ADDR);
	::ResetEvent(hLogin);
	pUserApi->Init();
	//wait for login event signaled. ::SetEvent(hLogin)
	DWORD dwRet = WaitForSingleObject(hLogin,10*1000); //wait 10 secs at most.
	if (dwRet == WAIT_TIMEOUT)
	{//启动超时，记告警日志，退出程序
		TRACE_ALARM("SFITDealer Starts timeout!");
		exit(1);
	}
	else if (dwRet == WAIT_OBJECT_0)
	{//正常启动
		//功 每次成下单将开启专门线程，跟踪所下单子的后继情况
		// _beginthread( workDealReportFunction, 0, (void *)this);
		TRACE_LOG("SfitDealer Starts OK.");
	    started = true;		
		return;
	}
	else //WAIT_ABANDONED
	{
		TRACE_ALARM("SfitDealer Starts failed!");
		exit(1);
	}
}

void SfitDealer::stop()
{
	if(pUserApi)
	{
		pUserApi->RegisterSpi(NULL);
		pUserApi->Release();
		pUserApi=NULL;
	}
	if(pUserSpi)
	{
		delete pUserSpi;
		pUserSpi = NULL;
	}
	started = false;
}

void SfitDealer::placeOrder(Order *order)
{
	int ref;
	//放入map中,执行相应动作,开启等待线程
	TRACE_LOG("SfitDealer::placeOrder, orderid:%s.",order->getId().c_str());
	if(!started)
	{
		TRACE_LOG("SfitDealer::placeOrder failed. sfit not started.");
		order->setRejected();
		return;
	}
	//放入map中
	EnterCriticalSection(&cs);
	SfitDealerInventory* pInventory = addInventory(order);
	if(pInventory == NULL)
	{//orderID与正在处理中的重复，要拒绝这次下单请求
		LeaveCriticalSection(&cs);
		TRACE_LOG("SfitDealer::placeOrder failed. orderID is duplicated.");
		//order->setRejected();
		return;
	}
	//分配localRef
	ref = pUserSpi->getCurOrderRef();
	pInventory->localRef = ref;
	LeaveCriticalSection(&cs);
	//执行下单动作
	// commodityProp==0 means 投机; commodityProp==1 means 套保
	int commodityProp = this->readCommodityProp(Contract::get(order->getContract())->getComodityName().c_str());
	if(0 != pUserSpi->InsertOrder(ref,order,commodityProp))
	{//下单直接失败
		EnterCriticalSection(&cs);
		delInventory(pInventory);
		LeaveCriticalSection(&cs);
		TRACE_LOG("SfitDealer::placeOrder failed. 下单直接失败");
		order->setRejected();
		return;
	}
	return;
}

void SfitDealer::cancelOrder(Order *order)
{
	//修改map中相应单子的状态，执行相应动作
	TRACE_LOG("SfitDealer::cancelOrder, orderid:%s.",order->getId().c_str());
	if(!started)
	{
		TRACE_LOG("SfitDealer::cancelOrder failed. sfit not started.");
		return;
	}
	//查看order状态，是否需要撤单
	if(order->isCanceled()||order->isDealed()||order->isRejected())
	{
		TRACE_LOG("SfitDealer::cancelOrder failed. orderid:%s is not in right status.",order->getId().c_str());
		return;
	}
	//从仓库中拿出ref
	int ref;
	EnterCriticalSection(&cs);
	SfitDealerInventory* pInventory = getInventory(order->getId());
	if(pInventory==NULL)
	{
		LeaveCriticalSection(&cs);
		TRACE_LOG("SfitDealer::cancelOrder failed. orderid:%s is not in this dealer.",order->getId().c_str());
		return;
	}
	ref = pInventory->localRef;
	LeaveCriticalSection(&cs);
	//执行相应动作
	if(0 != pUserSpi->CancelOrder(ref,order))
	{
		TRACE_LOG("SfitDealer::cancelOrder failed. orderid:%s 撤单直接失败",order->getId().c_str());
		return;
	}
	return;
}

void SfitDealer::OnReturnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder)
{
	int ref = atoi(pInputOrder->OrderRef);
	//在并行交易不多的前提下,通过扫描inventoryMap完成ref映射
	EnterCriticalSection(&cs);
	SfitDealerInventory* pInventory = findInventorybyLocal(ref);
	handleOrderInsertReport(pInventory,pInputOrder);
	LeaveCriticalSection(&cs);
}

void SfitDealer::OnReturnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction)
{
	int ref = atoi(pInputOrderAction->OrderRef);
	//在并行交易不多的前提下,通过扫描inventoryMap完成ref映射
	EnterCriticalSection(&cs);
	SfitDealerInventory* pInventory = findInventorybyLocal(ref);
	handleOrderActionReport(pInventory,pInputOrderAction);
	LeaveCriticalSection(&cs);
}

void SfitDealer::handleOrderInsertReport(SfitDealerInventory* pInventory, CThostFtdcInputOrderField *pInputOrder)
{
	if(pInventory == NULL) return;
	pInventory->placeStatus = 5;
	TRACE_DEBUG("[网关回调]报单插入有误,判断为报单被拒.将汇报. ref=%d.",pInventory->localRef);
	pInventory->order->setRejected();
	delInventory(pInventory);
}

void SfitDealer::handleOrderActionReport(SfitDealerInventory* pInventory, CThostFtdcInputOrderActionField *pInputOrder)
{
	if(pInventory == NULL) return;
	if(pInventory->placeStatus == 0) // new order without any response from remote server.
	{
		TRACE_DEBUG("[网关回调]撤单有误,判断为原报单未被接受.将汇报撤单成功，并删除该单记录. ref=%d.",pInventory->localRef);
		pInventory->order->setCancel();
		delInventory(pInventory);
	}
}

//网关回调过来的,ref内容大于0的报单状态通知
//通过ref映射回inventory,再根据pOrder内容修改inventory
void SfitDealer::OnReturnOrder(CThostFtdcOrderField *pOrder)
{
	int ref = atoi(pOrder->OrderRef);
	//在并行交易不多的前提下,通过扫描inventoryMap完成ref映射
	EnterCriticalSection(&cs);
	SfitDealerInventory* pInventory = findInventorybyLocal(ref);
	handleOrderStatusReport(pInventory,pOrder);
	LeaveCriticalSection(&cs);
}

//要分辨以下信息:下单成功(记录orderSysID),下单失败,撤单成功,撤单失败.
/////////////////////////////////////////////////////////////////////////
///typedef char TThostFtdcOrderSubmitStatusType;是一个报单提交状态类型
/////////////////////////////////////////////////////////////////////////
///已经提交 #define THOST_FTDC_OSS_InsertSubmitted '0'
///撤单已经提交 #define THOST_FTDC_OSS_CancelSubmitted '1'
///修改已经提交 #define THOST_FTDC_OSS_ModifySubmitted '2'
///已经接受 #define THOST_FTDC_OSS_Accepted '3'
///报单已经被拒绝 #define THOST_FTDC_OSS_InsertRejected '4'
///撤单已经被拒绝 #define THOST_FTDC_OSS_CancelRejected '5'
///改单已经被拒绝 #define THOST_FTDC_OSS_ModifyRejected '6'
/////////////////////////////////////////////////////////////////////////
///typedef char TThostFtdcOrderStatusType;是一个报单状态类型
/////////////////////////////////////////////////////////////////////////
///全部成交 #define THOST_FTDC_OST_AllTraded '0'
///部分成交还在队列中 #define THOST_FTDC_OST_PartTradedQueueing '1'
///部分成交不在队列中 #define THOST_FTDC_OST_PartTradedNotQueueing '2'
///未成交还在队列中 #define THOST_FTDC_OST_NoTradeQueueing '3'
///未成交不在队列中 #define THOST_FTDC_OST_NoTradeNotQueueing '4'
///撤单 #define THOST_FTDC_OST_Canceled '5'
///未知 #define THOST_FTDC_OST_Unknown 'a'
///尚未触发 #define THOST_FTDC_OST_NotTouched 'b'
///已触发 #define THOST_FTDC_OST_Touched 'c'

void SfitDealer::handleOrderStatusReport(SfitDealerInventory* pInventory, CThostFtdcOrderField *pOrder)
{
	if(pInventory == NULL) return;
	TRACE_DEBUG("[网关回调]报单状态通知。Ref:%s,OrderSysID:%s,SubmitStatus:%c,OrderStatus:%c.",
		pOrder->OrderRef,pOrder->OrderSysID,pOrder->OrderSubmitStatus,pOrder->OrderStatus);		

	if (pOrder->OrderSubmitStatus == THOST_FTDC_OSS_CancelRejected)
	{//撤单被拒
		TRACE_DEBUG("[网关回调]报单状态通知,判断为撤单被拒.");
	}
	else if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled)
	{//撤单成功
		pInventory->placeStatus = 4;
		TRACE_DEBUG("[网关回调]报单状态通知,判断为撤单成功.将汇报.");
		pInventory->order->setCancel();
		delInventory(pInventory);
	}
	else if(pOrder->OrderSubmitStatus == THOST_FTDC_OSS_InsertRejected)
	{//报单被拒
		pInventory->placeStatus = 5;
		TRACE_DEBUG("[网关回调]报单状态通知,判断为报单被拒.将汇报.");
		pInventory->order->setRejected();
		delInventory(pInventory);
	}
	else
	{//这里是报单的情况
		pInventory->remoteRef = pOrder->OrderSysID;
		if(pInventory->remoteRef != "" && pInventory->placeStatus == 0) //新报单已经被接受
			pInventory->placeStatus = 1;
		if (pOrder->OrderSubmitStatus == THOST_FTDC_OSS_Accepted && pOrder->OrderStatus == THOST_FTDC_OST_NoTradeQueueing)
		{//挂单的情况
			TRACE_DEBUG("[网关回调]报单状态通知,判断为挂单.");
		}
		else if (pOrder->OrderSubmitStatus == THOST_FTDC_OSS_InsertSubmitted && pOrder->OrderStatus == THOST_FTDC_OST_AllTraded)
		{//立刻成交的情况
			TRACE_DEBUG("[网关回调]报单状态通知,判断为立刻成交.");
		}
		else if (pOrder->OrderSubmitStatus == THOST_FTDC_OSS_InsertSubmitted && pOrder->OrderStatus == THOST_FTDC_OST_PartTradedQueueing)
		{//部分成交部分挂单的情况
			TRACE_DEBUG("[网关回调]报单状态通知,判断为部分成交部分挂单.");
		}
		else
		{//其他情况
			TRACE_DEBUG("[网关回调]报单状态通知,判断为其他情况.");
		}
	}
}


//网关回调过来的,所有成交通知
//通过orderSysID映射回inventory,再根据pTrade内容修改inventory
void SfitDealer::OnTrade(CThostFtdcTradeField *pTrade)
{
	//在并行交易不多的前提下,通过扫描inventoryMap完成orderSysID映射
	EnterCriticalSection(&cs);
	SfitDealerInventory* pInventory = findInventorybyRemote(string(pTrade->OrderSysID));
	handleTradeReport(pInventory,pTrade);
	LeaveCriticalSection(&cs);
}


void SfitDealer::handleTradeReport(SfitDealerInventory* pInventory, CThostFtdcTradeField *pTrade)
{
	if(pInventory == NULL) return;
	TRACE_LOG("[网关回调]关注的成交。内容：%s,ordersysid:%s,[%c,%c],%.3f,%d hand at %s %s.",
			pTrade->InstrumentID,pTrade->OrderSysID,
			pTrade->OffsetFlag,pTrade->Direction,
			pTrade->Price,pTrade->Volume,
			pTrade->TradeDate,pTrade->TradeTime);
	pInventory->dealedLot += pTrade->Volume;
	if(pInventory->dealedLot < pInventory->order->getLot())
	{//partial dealed.
		pInventory->placeStatus = 2;
		pInventory->order->addDetail(new OrderDetail(pInventory->order->getId(),pTrade->Volume,pTrade->Price));
	}
	else
	{//all dealed.
		pInventory->placeStatus = 3;
		pInventory->order->addDetail(new OrderDetail(pInventory->order->getId(),pTrade->Volume,pTrade->Price));
		delInventory(pInventory);
	}
}



//**********************class CSFITTraderSpi**************************
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSFITTraderSpi::CSFITTraderSpi()
{
	SESSION_ID=0;
	FRONT_ID=0;
	iRequestID=0;
	iNextOrderRef=0;
	memset(m_OrderSysID,0,sizeof(m_OrderSysID));
}
CSFITTraderSpi::~CSFITTraderSpi()
{
}
//////////////////////////////////////////////////////////////////////
// CALLBACKS
//////////////////////////////////////////////////////////////////////
void CSFITTraderSpi::OnFrontConnected()
{
	TRACE_LOG("[网关回调]IP连接已建立.");
	/*
	//request login.
	CThostFtdcReqAuthenticateField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.UserID, INVESTOR_ID);
	strcpy(req.AuthCode,"C6LKF3H2FJLSCDFN");  //xujun
	//strcpy(req.AuthCode,"MPZ7SSQIO5C8PCU6"); //xiaoyuetest
    strcpy(req.UserProductInfo, "xujun");
	//strcpy(req.UserProductInfo, "abc"); 
	int iResult = m_pDealer->pUserApi->ReqAuthenticate(&req, ++iRequestID);
	TRACE_LOG("[网关]发送认证请求: %s,%s, %s.",
		BROKER_ID,INVESTOR_ID,((iResult == 0) ? "发送成功" : "发送失败"));
	*/
	
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy_s(req.BrokerID, 11, BROKER_ID);
	strcpy_s(req.UserID, 13, INVESTOR_ID);
	strcpy_s(req.Password, 41, PASSWORD);
	int iResult = m_pDealer->pUserApi->ReqUserLogin(&req, ++iRequestID);
	TRACE_LOG("[网关]发送用户登录请求: %s,%s, %s.",
		BROKER_ID,INVESTOR_ID,((iResult == 0) ? "发送成功" : "发送失败"));
	
	
}

void CSFITTraderSpi::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateFiels, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    TRACE_LOG("OnRspAuthenticate");
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy_s(req.BrokerID, 11, BROKER_ID);
	strcpy_s(req.UserID, 13, INVESTOR_ID);
	strcpy_s(req.Password, 41, PASSWORD);
	int iResult = m_pDealer->pUserApi->ReqUserLogin(&req, ++iRequestID);
	TRACE_LOG("[网关]发送用户登录请求: %s,%s, %s.",
		BROKER_ID,INVESTOR_ID,((iResult == 0) ? "发送成功" : "发送失败"));
}

void CSFITTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		// 保存会话参数
		FRONT_ID = pRspUserLogin->FrontID;
		SESSION_ID = pRspUserLogin->SessionID;
		iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
		iNextOrderRef++;
		TRACE_LOG("[网关回调]用户登录成功: 交易日%s,前置编号%d,会话编号%d,报单引用%d.",
		m_pDealer->pUserApi->GetTradingDay(),FRONT_ID,SESSION_ID,iNextOrderRef);
		///投资者结算结果确认
		CThostFtdcSettlementInfoConfirmField req;
		memset(&req, 0, sizeof(req));
		strcpy_s(req.BrokerID, 11, BROKER_ID);
		strcpy_s(req.InvestorID, 13, INVESTOR_ID);
		int iResult = m_pDealer->pUserApi->ReqSettlementInfoConfirm(&req, ++iRequestID);
		TRACE_LOG("[网关]发送结算请求: %s,%s, %s.",
		BROKER_ID,INVESTOR_ID,((iResult == 0) ? "发送成功" : "发送失败"));
	}
	else
	{
		TRACE_LOG("[网关回调]用户登录失败.");
	}
}

void CSFITTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		TRACE_LOG("[网关回调]结算成功.");
		::SetEvent(m_pDealer->hLogin);
	}
	else
	{
		TRACE_LOG("[网关回调]结算失败.");
	}
}

void CSFITTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	TRACE_DEBUG("[网关回调]报单应答.");
	if(IsErrorRspInfo(pRspInfo))
	{
		TRACE_LOG("[网关回调]报单应答表明报单有错.");
		// 根据12月29日周杰测试中发现的问题（平今仓位不足），此处增加处理逻辑
		// 先判断报单是否本网关送出
		int ref = 0;
		ref = atoi(pInputOrder->OrderRef);
		if(ref > 0)
		{// 判断为自己网关送出的报单
			m_pDealer->OnReturnRspOrderInsert(pInputOrder);
		}
	}
}

void CSFITTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	TRACE_DEBUG("[网关回调]撤单应答.");
	if(IsErrorRspInfo(pRspInfo))
	{
		TRACE_LOG("[网关回调]撤单应答表明相应报单已撤或已成交.");
		//根据3月2日周杰使用中发现的问题（报单无回复情况下的撤单失败应该设置为撤单成功），此处增加处理逻辑
		// 先判断报单是否本网关送出
		int ref = 0;
		ref = atoi(pInputOrderAction->OrderRef);
		if(ref > 0)
		{// 判断为自己网关送出的报单
			m_pDealer->OnReturnRspOrderAction(pInputOrderAction);
		}
	}
}

///报单通知,私有流
void CSFITTraderSpi::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	if (!IsMyOrder(pOrder)) return;

	m_pDealer->OnReturnOrder(pOrder);
}

///成交通知,私有流
void CSFITTraderSpi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	m_pDealer->OnTrade(pTrade);
}

void CSFITTraderSpi:: OnFrontDisconnected(int nReason)
{
	TRACE_DEBUG("OnFrontDisconnected. Reason=%d.",nReason);
}
		
void CSFITTraderSpi::OnHeartBeatWarning(int nTimeLapse)
{
	TRACE_DEBUG("OnHeartBeatWarning. nTimerLapse=%d.",nTimeLapse);
}

void CSFITTraderSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	TRACE_ALARM("[网关回调]错误的请求.");
	IsErrorRspInfo(pRspInfo);
}

bool CSFITTraderSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	// 如果ErrorID != 0, 说明收到了错误的响应
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
	{
		TRACE_ALARM("ErrorID=%d, ErrorMsg=%s.",pRspInfo->ErrorID,pRspInfo->ErrorMsg);
	}
	return bResult;
}

//通过会话参数判断私有流的报单通知是否自己发出的
bool CSFITTraderSpi::IsMyOrder(CThostFtdcOrderField *pOrder)
{
	int ref = 0;
	ref = atoi(pOrder->OrderRef);
	return ((pOrder->FrontID == FRONT_ID) &&
			(pOrder->SessionID == SESSION_ID) &&
			(ref != 0));
}

//报单的网关接口实现方法
int CSFITTraderSpi::InsertOrder(int orderRef, Order *order, int commodityProp)
{
	CThostFtdcInputOrderField req;
	memset(&req, 0, sizeof(req));
	strcpy_s(req.BrokerID, 11, BROKER_ID);
	strcpy_s(req.InvestorID, 13, INVESTOR_ID);
	sprintf_s(req.OrderRef, 13, "%d", orderRef);
	req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	
	if(commodityProp==1)
	{ // 指定套保
		req.CombHedgeFlag[0] = THOST_FTDC_HF_Hedge;
	}
	else
	{ // 默认投机
		req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	}
	
	req.TimeCondition = THOST_FTDC_TC_GFD;

	// 关于手数和成交量的指示
	req.VolumeTotalOriginal = order->getLot();  //本次报单的目标手数
	req.VolumeCondition = THOST_FTDC_VC_AV;  //无成交条件挂单
	req.MinVolume = 1;  //最小成交手数，等同于目标手数（在上一项设为THOST_FTDC_VC_MV才有作用，这里起保护意义）

	req.ContingentCondition = THOST_FTDC_CC_Immediately;
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	req.IsAutoSuspend = 0;
	req.UserForceClose = 0;

	///合约代码
	strcpy_s(req.InstrumentID, 31, order->getContract().c_str());
	///买卖方向: 
	if(order->getBuySell()==BUY)
		req.Direction = THOST_FTDC_D_Buy;
	else
		req.Direction = THOST_FTDC_D_Sell;
	///组合开平标志: 开仓
	if(order->getKaiPing()==KAI_CANG)
		req.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
	else if (order->getKaiPing()==PING_CANG)
		req.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
	else
		req.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
	///价格
	req.LimitPrice = order->getPrice();

	int iResult = m_pDealer->pUserApi->ReqOrderInsert(&req, ++iRequestID);
	TRACE_LOG("[网关]报单录入请求: %s,[%c,%c],%.3f, lot:%d, %s, %s.",
		req.InstrumentID,req.CombOffsetFlag[0],req.Direction,req.LimitPrice,req.VolumeTotalOriginal,
		((commodityProp==1)?"套保":"投机"), ((iResult == 0) ? "发送成功" : "发送失败"));
	return iResult;
}

//撤单的网关接口实现方法
int CSFITTraderSpi::CancelOrder(int orderRef, Order *order)
{
	CThostFtdcInputOrderActionField req;
	memset(&req, 0, sizeof(req));
	strcpy_s(req.BrokerID, 11, BROKER_ID);
	strcpy_s(req.InvestorID, 13, INVESTOR_ID);
	strcpy_s(req.InstrumentID, 31, order->getContract().c_str());
	sprintf_s(req.OrderRef, 13, "%d", orderRef); 

	req.FrontID = FRONT_ID;
	req.SessionID = SESSION_ID;
	req.ActionFlag = THOST_FTDC_AF_Delete;

	int iResult = m_pDealer->pUserApi->ReqOrderAction(&req, ++iRequestID);
	TRACE_LOG("[网关]撤单操作请求: %s, %.3f, %s.",
		req.InstrumentID,req.LimitPrice,((iResult == 0) ? "发送成功" : "发送失败"));
	return iResult;
}

int CSFITTraderSpi::getCurOrderRef()
{
	int ret = iNextOrderRef;
	iNextOrderRef++;
	return ret;
}

