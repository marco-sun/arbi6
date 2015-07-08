#include "PatsApiDealer.h"
#include "logstore.h"
#include "Contract.h"
#include "SimPatsApiEngine.h"
#include <time.h>

PatsApiDealer::PatsApiDealer(void)
{
	isStarted = false;
	isThreadStarted = false;
	m_pEngine = NULL;
	isLogonOK = false;
	hLogin = ::CreateEvent(NULL,TRUE,FALSE,NULL);
	InitializeCriticalSection(&cs);
}

PatsApiDealer::~PatsApiDealer(void)
{
	::CloseHandle(hLogin);
	DeleteCriticalSection(&cs);
	if(isThreadStarted)
		StopThread();
}

void PatsApiDealer::ReadIniConfig()
{ // 登录需要的参数
	m_useSimEngine = 0;
	memset(m_szTraderAccount,   0, sizeof(m_szTraderAccount));
	m_useSimEngine = GetPrivateProfileIntA("PATSAPIDEALER","SIMENGINE_DEALER",0,".\\AIB.ini");
	GetPrivateProfileStringA("PATSAPIDEALER", "TRADER_ACCOUNT", "", m_szTraderAccount, 21, ".\\AIB.ini");
	TRACE_LOG("PatsApiDealer ReadIniConfig UseSimEngine:%d, TraderAccount:%s.",m_useSimEngine,m_szTraderAccount);
}

void PatsApiDealer::start()
{
	// 不可重入保护
	if(isStarted) return;
	// 读取登录配置
	ReadIniConfig();
	// 开启后台消息线程
	if(!isThreadStarted)
	{
		StartThread();
		isThreadStarted = true;
	}
	if(this->m_useSimEngine == 0)
	{
//		m_pFixEngine = FixEntry::getPriceEngineInstance((IMsg*)this);
	}
	else
	{
		m_pEngine = SimPatsApiEngine::getDealerEngineInstance((IMsg*)this);
	}

	// 组织登录消息，主动向PatsApi基础架构发起登录
	TPatsApiLogonMsg sender;
	memset(&sender,0,sizeof(TPatsApiLogonMsg));
	sender.type = MSGTYPE_PATSAPI_LOGON;
	sender.len = sizeof(TPatsApiLogonMsg)-sizeof(TRawMsg);

	if(m_pEngine) 
	{
		::ResetEvent(hLogin);
		BOOL ret = m_pEngine->SendMsg(&sender);
		// 吊住caller，等待登录成功消息
		DWORD dwRet = WaitForSingleObject(hLogin,120 * 1000); //wait 120 secs at most.
		if (dwRet == WAIT_TIMEOUT)
		{//Engine响应超时，记告警日志，退出程序
			TRACE_ALARM("PatsApiDealer Starts timeout (120ecs)!");
			exit(1);
		}
		else if (dwRet == WAIT_OBJECT_0)
		{//Engine响应
			if(isLogonOK)
			{
				TRACE_LOG("PatsApiDealer Starts OK (LOGON OK).");	
				isStarted = true;
			}
			else
			{
				TRACE_LOG("PatsApiDealer Starts FAIL (LOGON FAIL).");
				exit(1);
			}
		}
		else //WAIT_ABANDONED
		{
			TRACE_ALARM("PatsApiDealer Starts failed (WAIT_ABANDONED)!");
			exit(1);
		}
	}
	else
	{ // 记错误日志，终止程序
		TRACE_LOG("[ERROR] PatsApiDealer can NOT find the Engine.");
		exit(1);
	}
}

void PatsApiDealer::placeOrder(Order *order)
{
	TRACE_LOG("PatsApiDealer::placeOrder, orderid:%s.",order->getId().c_str());
	if(!isStarted)
	{
		TRACE_LOG("PatsApiDealer::placeOrder failed. PatsApiDealer not started.");
		order->setRejected();
		return;
	}
	Contract *pContract = Contract::get(order->getContract());
	//TODO: ZHOUJIE
	//string patsapiName = pContract->getFixName();
	string patsapiName = pContract->getPatsApiName();

	// patsapi格式名称转为patsapi合约三要素
	// patsapi格式名称为 cname-cCCYYMM@cexchange
	string cname = patsapiName.substr(0,patsapiName.find_first_of('-'));
	string cexchange = patsapiName.substr(patsapiName.find_first_of('@')+1);
	string cCCYYMM = patsapiName.substr(patsapiName.find_first_of('-')+1,patsapiName.find_first_of('@')-patsapiName.find_first_of('-')-1);
	TRACE_LOG("PatsApiDealer parse patsapiName[%s] to [%s,%s,%s].",patsapiName.c_str(),cname.c_str(),cexchange.c_str(),cCCYYMM.c_str());
	
	//发送下单消息
	TPatsApiAddOrderMsg sender;
	memset(&sender,0,sizeof(TPatsApiAddOrderMsg));
	sender.type = MSGTYPE_PATSAPI_AO;
	sender.len = sizeof(TPatsApiAddOrderMsg)-sizeof(TRawMsg);
	strncpy(sender.traderAccount,this->m_szTraderAccount,20);
	strncpy(sender.userOrderID,order->getId().c_str(),63);
	strncpy(sender.exchangeName,cexchange.c_str(),10);
	strncpy(sender.contractName,cname.c_str(),10);
	strncpy(sender.MaturityMonthYear,cCCYYMM.c_str(),50);
	if(order->getBuySell()==BUY) sender.BuySell = 'B';
	else sender.BuySell = 'S';
	if(order->getKaiPing()==KAI_CANG) sender.OpenClose = 'O';
	else sender.OpenClose = 'C';
	sender.OrderQty = order->getLot();
	sender.Price = order->getPrice();
	if(cname=="CMX COP")
	{
		sender.Price = order->getPrice()/100;
		TRACE_LOG("PatsApiDealer convert CMX COP oder price from %f to %f.",order->getPrice(),sender.Price);
	}
	if(m_pEngine) 
	{
		EnterCriticalSection(&cs);
		dealerInventoryMap[order->getId()]=order;
		LeaveCriticalSection(&cs);
		m_pEngine->SendMsg(&sender);
	}
	else
	{//下单直接失败
		TRACE_LOG("PatsApiDealer::placeOrder failed. 下单直接失败 can NOT find the Engine");
		order->setRejected();
		return;
	}
	return;
}

void PatsApiDealer::cancelOrder(Order *order)
{ // TODO
	// 1.查看order状态，是否需要撤单
	if(order->isCanceled()||order->isDealed()||order->isRejected())
	{
		TRACE_LOG("PatsApiDealer::cancelOrder failed. orderid:%s is not in right status.",order->getId().c_str());
		return;
	}
	TRACE_LOG("PatsApiDealer::cancelOrder. orderid:%s.",order->getId().c_str());
	//发送撤单消息
	TPatsApiCancelOrderMsg sender;
	memset(&sender,0,sizeof(TPatsApiCancelOrderMsg));
	sender.type = MSGTYPE_PATSAPI_CO;
	sender.len = sizeof(TPatsApiCancelOrderMsg)-sizeof(TRawMsg);
	strncpy(sender.userOrderID,order->getId().c_str(),63);
	if(m_pEngine) 
	{
		m_pEngine->SendMsg(&sender);
	}
	else
	{//撤单直接失败
		TRACE_LOG("PatsApiDealer::cancelOrder failed. 撤单直接失败 can NOT find the Engine");
	}

}

// ************后台消息线程******************
// Fix网关回来的所有消息在此处处理
bool PatsApiDealer::OnMsg(TRawMsg *msg, IMsg *remote_src)
{
	TRACE_LOG("PatsApiDealer recv msg.");
	if ( msg->type == MSGTYPE_PATSAPI_LOGON_RESP)
	{
		TRACE_LOG("  Is MSGTYPE_PATSAPI_LOGON_RESP.");
		TPatsApiLogonRespMsg* pLogonRespMsg = (TPatsApiLogonRespMsg*)msg;
		if(pLogonRespMsg->FailCode == 0)
		{
			isLogonOK = true;
		}
		else
		{
			isLogonOK = false;
			TRACE_LOG("PatsApiPriceSource Logon fail. code:%d, text:%s.",pLogonRespMsg->FailCode,pLogonRespMsg->Reason);
		}
		::SetEvent(hLogin);
	}
	else if ( msg->type == MSGTYPE_PATSAPI_OR)
	{
		TPatsApiOrderRespMsg* pOrderRespMsg = (TPatsApiOrderRespMsg*)msg;
		int status = pOrderRespMsg->OrdStatus;
		TRACE_LOG("PatsApiDealer[报单状态通知] orderid:%s, status:%d,lots:(%d/%d).",pOrderRespMsg->userOrderID,pOrderRespMsg->OrdStatus,
			pOrderRespMsg->filledLots,pOrderRespMsg->totalLots);
		Order* order = getOrder(pOrderRespMsg->userOrderID);
		if(order!=NULL)
		{
			// if order is cancelled or rejected, should report to user.
			if(0==status||2==status)
			{
				order->setRejected();
			}
			else if(5==status||6==status)
			{
				order->setCancel();
			}
			else
			{ // no need to report in other status.
			}
		}
	}
	else if ( msg->type == MSGTYPE_PATSAPI_FR)
	{ //
		TPatsApiFillRptMsg* pOrderFillMsg = (TPatsApiFillRptMsg*)msg;
		TRACE_LOG("PatsApiDealer[报单成交通知] 报单%s于价格 %.3f 成交 %d 手, 时间是 %s %s.",pOrderFillMsg->userOrderID,
			pOrderFillMsg->filledPrice,pOrderFillMsg->filledLots,pOrderFillMsg->dateFilled,pOrderFillMsg->timeFilled);
		
		
		Order* order = getOrder(pOrderFillMsg->userOrderID);
		if(order!=NULL)
		{
			Contract *pContract = Contract::get(order->getContract());
			string patsapiName = pContract->getPatsApiName();
			// patsapi格式名称转为patsapi合约三要素
			// patsapi格式名称为 cname-cCCYYMM@cexchange
			string cname = patsapiName.substr(0,patsapiName.find_first_of('-'));
			double filledP = pOrderFillMsg->filledPrice;
			if(cname=="CMX COP")
			{
				filledP = pOrderFillMsg->filledPrice * 100;
				TRACE_LOG("PatsApiDealer convert CMX COP oder fill price from %f to %f.", pOrderFillMsg->filledPrice,filledP);
			}
			order->addDetail(new OrderDetail(order->getId(),pOrderFillMsg->filledLots,filledP));
		}
	}
	else if ( msg->type == MSGTYPE_PATSAPI_ENGINE_STATUS)
	{
		TPatsApiEngineStatusMsg* pEngineStatus = (TPatsApiEngineStatusMsg*)msg;
		TRACE_LOG("PatsApiDealer[引擎状态通知] PATSAPI引擎状态为%d.",pEngineStatus->status);
	}
	else
	{
		TRACE_LOG("Unknown message 0x%x received from \'%s\'", 
			msg->type, remote_src ? remote_src->GetName() : "[Local]");
	}
	return true;
}

Order* PatsApiDealer::getOrder(string orderid)
{
	Order* ret = NULL;
	EnterCriticalSection(&cs);
	map<string, Order*>::iterator iter = dealerInventoryMap.find(orderid);
	if(iter != dealerInventoryMap.end()) ret = iter->second;
	LeaveCriticalSection(&cs);
	return ret;
}
