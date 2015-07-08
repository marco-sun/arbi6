// RohonDealer.cpp: implementation of the RohonDealer class.
//
//////////////////////////////////////////////////////////////////////

#include "RohonDealer.h"
#include "NameConverter.h"
#include "LogStore.h"
#include <process.h>

// 配置参数,将从配置文件中读出
static char FRONT_ADDR[128];		    // 前置地址
static char INVESTOR_ID[16];			// 投资者代码
static char PASSWORD[41] ;				// 用户密码

//******************class RohonDealerInventory**********************************
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
RohonDealerInventory::RohonDealerInventory(Order *pOrder)
{
	order = pOrder;
	localRef = 0;
	remoteRef = "";
	placeStatus = 0;
	dealedLot = 0;
}

RohonDealerInventory::~RohonDealerInventory()
{
}

//******************class RohonDealer**********************************
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RohonDealer::RohonDealer()
{
    started = false;
	hLogin = ::CreateEvent(NULL,TRUE,FALSE,NULL);
	ReadIniConfig();
	InitializeCriticalSection(&cs);
}

RohonDealer::~RohonDealer()
{
	if(started)
	{
		stop();
	}
	DeleteCriticalSection(&cs);
	::CloseHandle(hLogin);
}

RohonDealerInventory* RohonDealer::addInventory(Order *order)
{
	RohonDealerInventory* ret = NULL;
	string key = order->getId();
	//看是否有重复的key
	if(dealerInventoryMap.count(key)!=0)
	{
		return ret;
	}
	ret = new RohonDealerInventory(order);
	dealerInventoryMap.insert(make_pair(key,ret));
	return ret;
}

bool RohonDealer::delInventory(RohonDealerInventory* pDealerInventory)
{
	dealerInventoryMap.erase(pDealerInventory->order->getId());
	delete pDealerInventory;
	return TRUE;
}

RohonDealerInventory* RohonDealer::getInventory(string key)
{
	RohonDealerInventory* ret = NULL;
	map<string, RohonDealerInventory*>::iterator iter = dealerInventoryMap.find(key);
	if(iter == dealerInventoryMap.end()) return ret;
	return iter->second;
}

RohonDealerInventory* RohonDealer::findInventorybyLocal(int localRef)
{
	RohonDealerInventory* ret = NULL;
	map<string, RohonDealerInventory*>::iterator iter = dealerInventoryMap.begin();
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

RohonDealerInventory* RohonDealer::findInventorybyRemote(string remoteRef)
{
	RohonDealerInventory* ret = NULL;
	map<string, RohonDealerInventory*>::iterator iter = dealerInventoryMap.begin();
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

void RohonDealer::ReadIniConfig()
{
	char szFrontAddr[128];
	char szInvestorId[16];
	char szPassword[41];
	memset(szFrontAddr,  0, sizeof(szFrontAddr));
	memset(szInvestorId, 0, sizeof(szInvestorId));
	memset(szPassword,   0, sizeof(szPassword));

	if(GetPrivateProfileString("ROHONTRADE", "FRONT_ADDR", "tcp://61.152.108.102:5101", szFrontAddr, 128, ".\\AIB.ini"))
	{
		strcpy_s(FRONT_ADDR, 128, szFrontAddr);
	}
	if(GetPrivateProfileString("ROHONTRADE", "INVESTOR_ID",  "demo000", szInvestorId, 16, ".\\AIB.ini"))
	{
        strcpy_s(INVESTOR_ID, 16, szInvestorId);
	}
	if(GetPrivateProfileString("ROHONTRADE", "PASSWORD", "888888", szPassword, 41, ".\\AIB.ini"))
	{
        strcpy_s(PASSWORD, 41, szPassword);
	}
}

//同步的start,等登录完结才返回
void RohonDealer::start()
{
    if(started) {
	    TRACE_LOG("RohonDealer already started.");
		return;
	}
	//pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi(".\\dealstream\\",false);			// 创建UserApi

	pUserApi = CRohonTraderClientApi::CreateRHTraderApi();
	pUserSpi = new CROHONTraderSpi();
	pUserSpi->m_pDealer = this;
	pUserApi->RegisterSpi(pUserSpi);			// 注册事件类
	///订阅公共流。
	///@param nResumeType 公共流重传方式  
	///        ROHON_TERT_RESTART:从本交易日开始重传
	///        ROHON_TERT_RESUME:从上次收到的续传
	///        ROHON_TERT_QUICK:只传送登录后公共流的内容
	///@remark 该方法要在Init方法前调用。若不调用则不会收到公共流的数据。
	pUserApi->SubscribePublicTopic(ROHON_TERT_QUICK);				// 注册公有流
	///订阅私有流。
	///@param nResumeType 私有流重传方式  
	///        ROHON_TERT_RESTART:从本交易日开始重传
	///        ROHON_TERT_RESUME:从上次收到的续传
	///        ROHON_TERT_QUICK:只传送登录后私有流的内容
	///@remark 该方法要在Init方法前调用。若不调用则不会收到私有流的数据。
	pUserApi->SubscribePrivateTopic(ROHON_TERT_QUICK);			// 注册私有流

	pUserApi->RegisterFront(FRONT_ADDR);
	::ResetEvent(hLogin);
	pUserApi->Init();

	//pUserApi->SubscribePublicTopic(THOST_TERT_RESTART);				// 注册公有流
	//pUserApi->SubscribePrivateTopic(THOST_TERT_RESTART);			// 注册私有流
	
	//wait for login event signaled. ::SetEvent(hLogin)
	DWORD dwRet = WaitForSingleObject(hLogin,10*1000); //wait 10 secs at most.
	if (dwRet == WAIT_TIMEOUT)
	{//启动超时，记告警日志，退出程序
		TRACE_ALARM("RohonDealer Starts timeout!");
		printf("RohonDealer Starts timeout!\n");
		exit(1);
	}
	else if (dwRet == WAIT_OBJECT_0)
	{//正常启动
		//功 每次成下单将开启专门线程，跟踪所下单子的后继情况
		// _beginthread( workDealReportFunction, 0, (void *)this);
		TRACE_LOG("RohonDealer Starts OK.");
		printf("RohonDealer Starts OK.\n");
	    started = true;		
		return;
	}
	else //WAIT_ABANDONED
	{
		TRACE_ALARM("RohonDealer Starts failed!");
		printf("RohonDealer Starts failed!\n");
		exit(1);
	}
}

void RohonDealer::stop()
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

void RohonDealer::queryAllInstrument()
{
	if(started)
	{
		pUserSpi->QueryAllInstrument();
	}
}

void RohonDealer::placeOrder(Order *order)
{
	int ref;
	//放入map中,执行相应动作,开启等待线程
	TRACE_LOG("RohonDealer::placeOrder, orderid:%s.",order->getId().c_str());
	if(!started)
	{
		TRACE_LOG("RohonDealer::placeOrder failed. RohonDealer not started.");
		order->setRejected();
		return;
	}
	//放入map中
	EnterCriticalSection(&cs);
	RohonDealerInventory* pInventory = addInventory(order);
	if(pInventory == NULL)
	{//orderID与正在处理中的重复，要拒绝这次下单请求
		LeaveCriticalSection(&cs);
		TRACE_LOG("RohonDealer::placeOrder failed. orderID is duplicated.");
		//order->setRejected();
		return;
	}
	//分配localRef
	ref = pUserSpi->getCurOrderRef();
	pInventory->localRef = ref;
	LeaveCriticalSection(&cs);
	//执行下单动作
	if(0 != pUserSpi->InsertOrder(ref,order))
	{//下单直接失败
		EnterCriticalSection(&cs);
		delInventory(pInventory);
		LeaveCriticalSection(&cs);
		TRACE_LOG("RohonDealer::placeOrder failed. 下单直接失败");
		order->setRejected();
		return;
	}
	return;
}

void RohonDealer::cancelOrder(Order *order)
{
	//修改map中相应单子的状态，执行相应动作
	TRACE_LOG("RohonDealer::cancelOrder, orderid:%s.",order->getId().c_str());
	if(!started)
	{
		TRACE_LOG("RohonDealer::cancelOrder failed. sfit not started.");
		return;
	}
	//查看order状态，是否需要撤单
	if(order->isCanceled()||order->isDealed()||order->isRejected())
	{
		TRACE_LOG("RohonDealer::cancelOrder failed. orderid:%s is not in right status.",order->getId().c_str());
		return;
	}
	//从仓库中拿出ref
	int ref;
	EnterCriticalSection(&cs);
	RohonDealerInventory* pInventory = getInventory(order->getId());
	if(pInventory==NULL)
	{
		LeaveCriticalSection(&cs);
		TRACE_LOG("RohonDealer::cancelOrder failed. orderid:%s is not in this dealer.",order->getId().c_str());
		return;
	}
	ref = pInventory->localRef;
	LeaveCriticalSection(&cs);
	//执行相应动作
	if(0 != pUserSpi->CancelOrder(ref,order))
	{
		TRACE_LOG("RohonDealer::cancelOrder failed. orderid:%s 撤单直接失败",order->getId().c_str());
		return;
	}
	return;
}

void RohonDealer::OnReturnRspOrderInsert(CRohonInputOrderField *pInputOrder)
{
	int ref = atoi(pInputOrder->OrderRef);
	//在并行交易不多的前提下,通过扫描inventoryMap完成ref映射
	EnterCriticalSection(&cs);
	RohonDealerInventory* pInventory = findInventorybyLocal(ref);
	handleOrderInsertReport(pInventory,pInputOrder);
	LeaveCriticalSection(&cs);
}

void RohonDealer::OnReturnRspOrderAction(CRohonInputOrderActionField *pInputOrderAction)
{
	int ref = atoi(pInputOrderAction->OrderRef);
	//在并行交易不多的前提下,通过扫描inventoryMap完成ref映射
	EnterCriticalSection(&cs);
	RohonDealerInventory* pInventory = findInventorybyLocal(ref);
	handleOrderActionReport(pInventory,pInputOrderAction);
	LeaveCriticalSection(&cs);
}

void RohonDealer::handleOrderInsertReport(RohonDealerInventory* pInventory, CRohonInputOrderField *pInputOrder)
{
	if(pInventory == NULL) return;
	pInventory->placeStatus = 5;
	TRACE_DEBUG("[RohonDealer网关回调]报单插入有误,判断为报单被拒.将汇报. ref=%d.",pInventory->localRef);
	pInventory->order->setRejected();
	delInventory(pInventory);
}

void RohonDealer::handleOrderActionReport(RohonDealerInventory* pInventory, CRohonInputOrderActionField *pInputOrder)
{
	if(pInventory == NULL) return;
	if(pInventory->placeStatus == 0) // new order without any response from remote server.
	{
		TRACE_DEBUG("[RohonDealer网关回调]撤单有误,判断为原报单未被接受.将汇报撤单成功，并删除该单记录. ref=%d.",pInventory->localRef);
		pInventory->order->setCancel();
		delInventory(pInventory);
	}
}

//网关回调过来的,ref内容大于0的报单状态通知
//通过ref映射回inventory,再根据pOrder内容修改inventory
void RohonDealer::OnReturnOrder(CRohonOrderField *pOrder)
{
	int ref = atoi(pOrder->OrderRef);
	//在并行交易不多的前提下,通过扫描inventoryMap完成ref映射
	EnterCriticalSection(&cs);
	RohonDealerInventory* pInventory = findInventorybyLocal(ref);
	handleOrderStatusReport(pInventory,pOrder);
	LeaveCriticalSection(&cs);
}

//要分辨以下信息:下单成功(记录orderSysID),下单失败,撤单成功,撤单失败.
/////////////////////////////////////////////////////////////////////////
///TFtdcOrderSubmitStatusType是一个报单提交状态类型
/////////////////////////////////////////////////////////////////////////
///已经提交			#define ROHON_TRADE_OSS_InsertSubmitted '0'
///撤单已经提交		#define ROHON_TRADE_OSS_CancelSubmitted '1'
///修改已经提交		#define ROHON_TRADE_OSS_ModifySubmitted '2'
///已经接受			#define ROHON_TRADE_OSS_Accepted '3'
///报单已经被拒绝	#define ROHON_TRADE_OSS_InsertRejected '4'
///撤单已经被拒绝	#define ROHON_TRADE_OSS_CancelRejected '5'
///改单已经被拒绝	#define ROHON_TRADE_OSS_ModifyRejected '6'
//
/////////////////////////////////////////////////////////////////////////
///TFtdcOrderStatusType是一个报单状态类型
/////////////////////////////////////////////////////////////////////////
///全部成交				#define ROHON_TRADE_OST_AllTraded '0'
///部分成交还在队列中	#define ROHON_TRADE_OST_PartTradedQueueing '1'
///部分成交不在队列中	#define ROHON_TRADE_OST_PartTradedNotQueueing '2'
///未成交还在队列中		#define ROHON_TRADE_OST_NoTradeQueueing '3'
///未成交不在队列中		#define ROHON_TRADE_OST_NoTradeNotQueueing '4'
///撤单					#define ROHON_TRADE_OST_Canceled '5'
///未知					#define ROHON_TRADE_OST_Unknown 'a'
///尚未触发				#define ROHON_TRADE_OST_NotTouched 'b'
///已触发				#define ROHON_TRADE_OST_Touched 'c'
///已提交				#define ROHON_TRADE_OST_Submitted 'd'
///正在修改				#define ROHON_TRADE_OST_Amending 'm'
//
void RohonDealer::handleOrderStatusReport(RohonDealerInventory* pInventory, CRohonOrderField *pOrder)
{
	if(pInventory == NULL) return;
	TRACE_DEBUG("[RohonDealer网关回调]报单状态通知。Ref:%s,OrderSysID:%s,SubmitStatus:%c,OrderStatus:%c.",
		pOrder->OrderRef,pOrder->OrderSysID,pOrder->OrderSubmitStatus,pOrder->OrderStatus);	
	printf("[RohonDealer网关回调]报单状态通知。Ref:%s,OrderSysID:%s,SubmitStatus:%c,OrderStatus:%c.\n",
		pOrder->OrderRef,pOrder->OrderSysID,pOrder->OrderSubmitStatus,pOrder->OrderStatus);	

	if (pOrder->OrderSubmitStatus == ROHON_TRADE_OSS_CancelRejected)
	{//撤单被拒
		TRACE_DEBUG("[RohonDealer网关回调]报单状态通知,判断为撤单被拒.");
	}
	else if (pOrder->OrderStatus == ROHON_TRADE_OST_Canceled)
	{//撤单成功
		pInventory->placeStatus = 4;
		TRACE_DEBUG("[RohonDealer网关回调]报单状态通知,判断为撤单成功.将汇报.");
		pInventory->order->setCancel();
		delInventory(pInventory);
	}
	else if(pOrder->OrderSubmitStatus == ROHON_TRADE_OSS_InsertRejected)
	{//报单被拒
		pInventory->placeStatus = 5;
		TRACE_DEBUG("[RohonDealer网关回调]报单状态通知,判断为报单被拒.将汇报.");
		pInventory->order->setRejected();
		delInventory(pInventory);
	}
	else
	{//这里是报单的情况
		pInventory->remoteRef = pOrder->OrderSysID;
		if(pInventory->remoteRef != "" && pInventory->placeStatus == 0) //新报单已经被接受
			pInventory->placeStatus = 1;
		if (pOrder->OrderSubmitStatus == ROHON_TRADE_OSS_Accepted && pOrder->OrderStatus == ROHON_TRADE_OST_NoTradeQueueing)
		{//挂单的情况
			TRACE_DEBUG("[RohonDealer网关回调]报单状态通知,判断为挂单.");
		}
		else if (pOrder->OrderSubmitStatus == ROHON_TRADE_OSS_InsertSubmitted && pOrder->OrderStatus == ROHON_TRADE_OST_AllTraded)
		{//立刻成交的情况
			TRACE_DEBUG("[RohonDealer网关回调]报单状态通知,判断为全部成交.");
		}
		else if (pOrder->OrderSubmitStatus == ROHON_TRADE_OSS_InsertSubmitted && pOrder->OrderStatus == ROHON_TRADE_OST_PartTradedQueueing)
		{//部分成交部分挂单的情况
			TRACE_DEBUG("[RohonDealer网关回调]报单状态通知,判断为部分成交部分挂单.");
		}
		else
		{//其他情况
			TRACE_DEBUG("[RohonDealer网关回调]报单状态通知,判断为其他情况.");
		}
	}
}


//网关回调过来的,所有成交通知
//通过orderSysID映射回inventory,再根据pTrade内容修改inventory
void RohonDealer::OnTrade(CRohonTradeField *pTrade)
{
	//在并行交易不多的前提下,通过扫描inventoryMap完成orderSysID映射
	EnterCriticalSection(&cs);
	RohonDealerInventory* pInventory = findInventorybyRemote(string(pTrade->OrderSysID));
	handleTradeReport(pInventory,pTrade);
	LeaveCriticalSection(&cs);
}


void RohonDealer::handleTradeReport(RohonDealerInventory* pInventory, CRohonTradeField *pTrade)
{
	if(pInventory == NULL) return;
	TRACE_LOG("[RohonDealer网关回调]关注的成交。内容：%s,ordersysid:%s,[%c,%c],%.3f,%d hand at %s %s.",
			pTrade->InstrumentID,pTrade->OrderSysID,
			pTrade->OffsetFlag,pTrade->Direction,
			pTrade->Price,pTrade->Volume,
			pTrade->TradeDate,pTrade->TradeTime);
	printf("[RohonDealer网关回调]关注的成交。内容：%s,ordersysid:%s,[%c,%c],%.3f,%d hand at %s %s.\n",
			pTrade->InstrumentID,pTrade->OrderSysID,
			pTrade->OffsetFlag,pTrade->Direction,
			pTrade->Price,pTrade->Volume,
			pTrade->TradeDate,pTrade->TradeTime);
	pInventory->dealedLot += pTrade->Volume;
	if(pInventory->dealedLot < pInventory->order->getLot())
	{//partial dealed.
		pInventory->placeStatus = 2;
		pInventory->order->addDetail(new OrderDetail(pInventory->order->getId(),pTrade->Volume,pTrade->Price));
		TRACE_LOG("[RohonDealer网关回调]partial dealed,baseName:%s.",pInventory->order->getContract().c_str());
		printf("[RohonDealer网关回调]partial dealed,baseName:%s.\n",pInventory->order->getContract().c_str());
	}
	else
	{//all dealed.
		pInventory->placeStatus = 3;
		pInventory->order->addDetail(new OrderDetail(pInventory->order->getId(),pTrade->Volume,pTrade->Price));
		TRACE_LOG("[RohonDealer网关回调]all dealed,baseName:%s.",pInventory->order->getContract().c_str());
		printf("[RohonDealer网关回调]all dealed,baseName:%s.\n",pInventory->order->getContract().c_str());
		delInventory(pInventory);
	}
}


//**********************class CROHONTraderSpi**************************
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CROHONTraderSpi::CROHONTraderSpi()
{
	SESSION_ID=0;
	FRONT_ID=0;
	iRequestID=0;
	iNextOrderRef=0;
	memset(m_OrderSysID,0,sizeof(m_OrderSysID));
}
CROHONTraderSpi::~CROHONTraderSpi()
{
}
//////////////////////////////////////////////////////////////////////
// CALLBACKS
//////////////////////////////////////////////////////////////////////
void CROHONTraderSpi::OnFrontConnected()
{
	TRACE_LOG("[RohonDealer网关回调]IP连接已建立.");
	//request login.
	CRohonReqUserLoginField req;
	memset(&req, 0, sizeof(req));

	strcpy_s(req.UserID, 16, INVESTOR_ID);
	strcpy_s(req.Password, 41, PASSWORD);
	int iResult = m_pDealer->pUserApi->ReqUserLogin(&req, ++iRequestID);
	TRACE_LOG("[RohonDealer网关]发送用户登录请求: %s, %s.",INVESTOR_ID,((iResult == 0) ? "发送成功" : "发送失败"));
}

void CROHONTraderSpi::OnRspUserLogin(CRohonRspUserLoginField *pRspUserLogin, CRohonRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		// 保存会话参数
		FRONT_ID = pRspUserLogin->FrontID;
		SESSION_ID = pRspUserLogin->SessionID;
		iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
		iNextOrderRef++;
		TRACE_LOG("[RohonDealer网关回调]用户登录成功: 交易日%s,前置编号%d,会话编号%d,报单引用%d.",
		m_pDealer->pUserApi->GetTradingDay(),FRONT_ID,SESSION_ID,iNextOrderRef);
		///投资者结算结果确认
		CRohonSettlementInfoConfirmField req;
		memset(&req, 0, sizeof(req));
		strcpy_s(req.InvestorID, 16, INVESTOR_ID);
		int iResult = m_pDealer->pUserApi->ReqSettlementInfoConfirm(&req, ++iRequestID);
		TRACE_LOG("[RohonDealer网关]发送结算请求: %s, %s.",INVESTOR_ID,((iResult == 0) ? "发送成功" : "发送失败"));
	}
	else
	{
		TRACE_LOG("[RohonDealer网关回调]用户登录失败.");
	}
}

void CROHONTraderSpi::OnRspSettlementInfoConfirm(CRohonSettlementInfoConfirmField *pSettlementInfoConfirm, CRohonRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		TRACE_LOG("[RohonDealer网关回调]结算成功.");
		::SetEvent(m_pDealer->hLogin);
	}
	else
	{
		TRACE_LOG("[RohonDealer网关回调]结算失败.");
	}
}

void CROHONTraderSpi::OnRspQryInstrument(CRohonInstrumentField *pInstrument, CRohonRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (!IsErrorRspInfo(pRspInfo) && pInstrument!=NULL)
	{
		TRACE_LOG("[RohonDealer网关回调]查询所有合约结果：%s, 交易所：%s, 合约：%s", pInstrument->InstrumentID, pInstrument->ExchangeID, pInstrument->InstrumentName);
		printf("[RohonDealer网关回调]查询所有合约结果：%s, 交易所：%s, 合约：%s\n", pInstrument->InstrumentID, pInstrument->ExchangeID, pInstrument->InstrumentName);
	}
	if (bIsLast)
	{
		TRACE_LOG("[RohonDealer网关回调]查询所有合约结果 结束");
		printf("[RohonDealer网关回调]查询所有合约结果 结束\n");
	}
}

void CROHONTraderSpi::OnRspOrderInsert(CRohonInputOrderField *pInputOrder, CRohonRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	TRACE_DEBUG("[RohonDealer网关回调]报单应答.");
	if(IsErrorRspInfo(pRspInfo))
	{
		TRACE_LOG("[RohonDealer网关回调]报单应答表明报单有错.");
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

void CROHONTraderSpi::OnRspOrderAction(CRohonInputOrderActionField *pInputOrderAction, CRohonRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	TRACE_DEBUG("[RohonDealer网关回调]撤单应答.");
	if(IsErrorRspInfo(pRspInfo))
	{
		TRACE_LOG("[RohonDealer网关回调]撤单应答表明相应报单已撤或已成交.");
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
void CROHONTraderSpi::OnRtnOrder(CRohonOrderField *pOrder)
{
	if (!IsMyOrder(pOrder)) return;

	m_pDealer->OnReturnOrder(pOrder);
}

///成交通知,私有流
void CROHONTraderSpi::OnRtnTrade(CRohonTradeField *pTrade)
{
	m_pDealer->OnTrade(pTrade);
}

void CROHONTraderSpi:: OnFrontDisconnected(int nReason)
{
	TRACE_DEBUG("OnFrontDisconnected. Reason=%d.",nReason);
}
		
void CROHONTraderSpi::OnHeartBeatWarning(int nTimeLapse)
{
	TRACE_DEBUG("OnHeartBeatWarning. nTimerLapse=%d.",nTimeLapse);
}

bool CROHONTraderSpi::IsErrorRspInfo(CRohonRspInfoField *pRspInfo)
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
bool CROHONTraderSpi::IsMyOrder(CRohonOrderField *pOrder)
{
	int ref = 0;
	ref = atoi(pOrder->OrderRef);
	return ((pOrder->FrontID == FRONT_ID) &&
			(pOrder->SessionID == SESSION_ID) &&
			(ref != 0));
}

//报单的网关接口实现方法
int CROHONTraderSpi::InsertOrder(int orderRef, Order *order)
{
	CRohonInputOrderField req;
	memset(&req, 0, sizeof(req));
	strcpy_s(req.InvestorID, 13, INVESTOR_ID);
	sprintf_s(req.OrderRef, 13, "%d", orderRef);
	req.OrderPriceType = ROHON_TRADE_OPT_LimitPrice;
	req.CombHedgeFlag[0] = ROHON_TRADE_HF_Speculation;
	req.TimeCondition = ROHON_TRADE_TC_GFD;

	// 关于手数和成交量的指示
	req.VolumeTotalOriginal = order->getLot();  //本次报单的目标手数
	req.VolumeCondition = ROHON_TRADE_VC_AV;  //无成交条件挂单
	req.MinVolume = 1;  //最小成交手数，等同于目标手数（在上一项设为ROHON_TRADE_VC_MV才有作用，这里起保护意义）

	req.ContingentCondition = ROHON_TRADE_CC_Immediately;
	req.ForceCloseReason = ROHON_TRADE_FCC_NotForceClose;
	req.IsAutoSuspend = 0;
	req.UserForceClose = 0;

	///合约代码
	NameConverter* pNC = NameConverter::getInstance();
	string rohonName = pNC->base2RohonName(order->getContract());
	if(rohonName=="") 
	{
		TRACE_LOG("[CROHONTraderSpi::InsertOrder]Warning, NO RohonName found according to BaseName:%s.",order->getContract().c_str());
		return 1;
	}
	strcpy_s(req.InstrumentID, 81, rohonName.c_str());

	///买卖方向: 
	if(order->getBuySell()==BUY)
		req.Direction = ROHON_TRADE_D_Buy;
	else
		req.Direction = ROHON_TRADE_D_Sell;
	///组合开平标志: 开仓
	if(order->getKaiPing()==KAI_CANG)
		req.CombOffsetFlag[0] = ROHON_TRADE_OF_Open;
	else if (order->getKaiPing()==PING_CANG)
		req.CombOffsetFlag[0] = ROHON_TRADE_OF_Close;
	else
		req.CombOffsetFlag[0] = ROHON_TRADE_OF_CloseToday;
	///价格
	req.LimitPrice = order->getPrice();

	int iResult = m_pDealer->pUserApi->ReqOrderInsert(&req, ++iRequestID);
	TRACE_LOG("[RohonDealer网关]报单录入请求: %s,[%c,%c],%.3f, lot:%d, %s.",
		req.InstrumentID,req.CombOffsetFlag[0],req.Direction,req.LimitPrice,req.VolumeTotalOriginal,
		((iResult == 0) ? "发送成功" : "发送失败"));
	return iResult;
}

//撤单的网关接口实现方法
int CROHONTraderSpi::CancelOrder(int orderRef, Order *order)
{
	CRohonInputOrderActionField req;
	memset(&req, 0, sizeof(req));
	strcpy_s(req.InvestorID, 13, INVESTOR_ID);

	NameConverter* pNC = NameConverter::getInstance();
	string rohonName = pNC->base2RohonName(order->getContract());
	if(rohonName=="") 
	{
		TRACE_LOG("[CROHONTraderSpi::CancelOrder]Warning, NO RohonName found according to BaseName:%s.",order->getContract().c_str());
		return 1;
	}
	strcpy_s(req.InstrumentID, 81, rohonName.c_str());
	
	sprintf_s(req.OrderRef, 13, "%d", orderRef); 

	req.FrontID = FRONT_ID;
	req.SessionID = SESSION_ID;
	req.ActionFlag = ROHON_TRADE_AF_Delete;

	int iResult = m_pDealer->pUserApi->ReqOrderAction(&req, ++iRequestID);
	TRACE_LOG("[RohonDealer网关]撤单操作请求: %s, %.3f, %s.",
		req.InstrumentID,req.LimitPrice,((iResult == 0) ? "发送成功" : "发送失败"));
	return iResult;
}

int CROHONTraderSpi::QueryAllInstrument()
{
	CRohonQryInstrumentField field;
	memset(&field, 0, sizeof(CRohonQryInstrumentField));
	int iResult = m_pDealer->pUserApi->ReqQryInstrument(&field, ++iRequestID);
	TRACE_LOG("[RohonDealer网关]查询所有合约操作请求: %s.",((iResult == 0) ? "发送成功" : "发送失败"));
	return iResult;

}

int CROHONTraderSpi::getCurOrderRef()
{
	int ret = iNextOrderRef;
	iNextOrderRef++;
	return ret;
}



