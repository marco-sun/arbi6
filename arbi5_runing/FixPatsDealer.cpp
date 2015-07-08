//#include "StdAfx.h"
#include "FixPatsDealer.h"
#include "logstore.h"
#include "Contract.h"
#include "SimFixEngine.h"
#include "FixEntry.h"
#include <time.h>

FixPatsDealerInventory::FixPatsDealerInventory(Order *pOrder)
{
	order = pOrder;
	tag11 = "";
	tag37 = "";
	status = -1;

}

FixPatsDealerInventory::~FixPatsDealerInventory()
{
}

int FixPatsDealer::s_InvokeID = 0;

FixPatsDealer::FixPatsDealer(void)
{
	isStarted = false;
	isThreadStarted = false;
	m_pFixEngine = NULL;
	isLogonOK = false;
	hLogin = ::CreateEvent(NULL,TRUE,FALSE,NULL);
	InitializeCriticalSection(&cs);
	dealerInventoryMap.clear();
}


FixPatsDealer::~FixPatsDealer(void)
{
	DeleteCriticalSection(&cs);
	::CloseHandle(hLogin);
	if(isThreadStarted)
		StopThread();
}

void FixPatsDealer::ReadIniConfig()
{ // 登录需要的参数 HeartBeatInterval,username,password
	m_useSimEngine = 0;
	m_HeartBeatInterval = 30;
	memset(m_szUsername,   0, sizeof(m_szUsername));
	memset(m_szPassword,   0, sizeof(m_szPassword));

	GetPrivateProfileStringA("FIXPATSGW", "LOGON_UID", "patsusername", m_szUsername, 16, ".\\AIB.ini");
	GetPrivateProfileStringA("FIXPATSGW", "LOGON_PWD", "patspassword", m_szPassword, 16, ".\\AIB.ini");
	m_HeartBeatInterval = GetPrivateProfileIntA("FIXPATSGW","LOGON_HBINT",30,".\\AIB.ini");
	m_useSimEngine = GetPrivateProfileIntA("FIXPATSGW","SIMENGINE_DEALER",0,".\\AIB.ini");
	TRACE_LOG("FixPatsDealer ReadIniConfig UID:%s,PWD:%s,HeartBeatInt:%d,UseSimEngine:%d.",m_szUsername,m_szPassword,m_HeartBeatInterval,m_useSimEngine);
}

void FixPatsDealer::start()
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
		m_pFixEngine = FixEntry::getDealerEngineInstance((IMsg*)this);
	}
	else
	{
		m_pFixEngine = SimFixEngine::getDealerEngineInstance((IMsg*)this);
	}

	// 组织登录消息，主动向Fix基础架构发起登录
	TLogonMsg sender;
	sender.type = MSGTYPE_FIXMSG_LOGON;
	sender.len = sizeof(TLogonMsg)-sizeof(TRawMsg);
	sender.EncryptMethod = 0;
	sender.HeartBtInt = m_HeartBeatInterval;
	sprintf(sender.RawData,"%s/%s",m_szUsername,m_szPassword);
	sender.RawDataLength = strlen(sender.RawData);
	sender.ResetSeqNumFlag = 'Y';
	if(m_pFixEngine) 
	{
		::ResetEvent(hLogin);
		BOOL ret = m_pFixEngine->SendMsg(&sender);
		// 吊住caller，等待登录成功消息
		DWORD dwRet = WaitForSingleObject(hLogin,180 * 1000); //wait 180 secs at most.
		if (dwRet == WAIT_TIMEOUT)
		{//Engine响应超时，记告警日志，退出程序
			TRACE_ALARM("FixPatsDealer Starts timeout (180ecs)!");
			exit(1);
		}
		else if (dwRet == WAIT_OBJECT_0)
		{//Engine响应
			if(isLogonOK)
			{
				TRACE_LOG("FixPatsDealer Starts OK (LOGON OK).");	
				isStarted = true;
			}
			else
			{
				TRACE_LOG("FixPatsDealer Starts FAIL (LOGON FAIL).");
				exit(1);
			}
		}
		else //WAIT_ABANDONED
		{
			TRACE_ALARM("FixPatsDealer Starts failed (WAIT_ABANDONED)!");
			exit(1);
		}
	}
	else
	{ // 记错误日志，终止程序
		TRACE_LOG("[ERROR] FixPatsDealer can NOT find the Fix Engine.");
		exit(1);
	}
}

FixPatsDealerInventory* FixPatsDealer::addInventory(Order *order)
{
	FixPatsDealerInventory* ret = NULL;
	string key = order->getId();
	//看是否有重复的key
	if(dealerInventoryMap.count(key)!=0)
	{
		return ret;
	}
	ret = new FixPatsDealerInventory(order);
	dealerInventoryMap.insert(make_pair(key,ret));
	return ret;
}

bool FixPatsDealer::delInventory(FixPatsDealerInventory* pDealerInventory)
{
	dealerInventoryMap.erase(pDealerInventory->order->getId());
	delete pDealerInventory;
	return TRUE;
}

FixPatsDealerInventory* FixPatsDealer::getInventory(string key)
{
	FixPatsDealerInventory* ret = NULL;
	map<string, FixPatsDealerInventory*>::iterator iter = dealerInventoryMap.find(key);
	if(iter == dealerInventoryMap.end()) return ret;
	return iter->second;
}

FixPatsDealerInventory* FixPatsDealer::findInventorybyTag11(string tag11)
{
	FixPatsDealerInventory* ret = NULL;
	map<string, FixPatsDealerInventory*>::iterator iter = dealerInventoryMap.begin();
	while(iter != dealerInventoryMap.end())
	{
		if(iter->second->tag11 == tag11)
		{
			ret = iter->second;
			break;
		}
		iter ++;
	}
	return ret;
}

void FixPatsDealer::placeOrder(Order *order)
{
	string tag11;
	
	TRACE_LOG("FixPatsDealer::placeOrder, orderid:%s.",order->getId().c_str());
	if(!isStarted)
	{
		TRACE_LOG("FixPatsDealer::placeOrder failed. FixPatsDealer not started.");
		order->setRejected();
		return;
	}
	//放入map中
	EnterCriticalSection(&cs);
	FixPatsDealerInventory* pInventory = addInventory(order);
	if(pInventory == NULL)
	{//orderID与正在处理中的重复，要拒绝这次下单请求
		LeaveCriticalSection(&cs);
		TRACE_LOG("FixPatsDealer::placeOrder failed. orderID is duplicated.");
		return;
	}
	//组装tag11
	tag11 = assembleID(order->getId());
	pInventory->tag11 = tag11;
	LeaveCriticalSection(&cs);
	//发送下单消息
	TNewOrderSingleMsg sender;
	assembleNOSMsg(tag11,order,sender);
	if(m_pFixEngine) 
	{
		m_pFixEngine->SendMsg(&sender);
	}
	else
	{//下单直接失败
		EnterCriticalSection(&cs);
		delInventory(pInventory);
		LeaveCriticalSection(&cs);
		TRACE_LOG("FixPatsDealer::placeOrder failed. 下单直接失败 can NOT find the Fix Engine");
		order->setRejected();
		return;
	}
	return;
}

void FixPatsDealer::cancelOrder(Order *order)
{
	//查看order状态，是否需要撤单
	if(order->isCanceled()||order->isDealed()||order->isRejected())
	{
		TRACE_LOG("FixPatsDealer::cancelOrder failed. orderid:%s is not in right status.",order->getId().c_str());
		return;
	}
	TRACE_LOG("FixPatsDealer::cancelOrder. orderid:%s.",order->getId().c_str());
	//从仓库中拿出ref
	string tag11;
	string tag37;
	EnterCriticalSection(&cs);
	FixPatsDealerInventory* pInventory = getInventory(order->getId());
	if(pInventory==NULL)
	{
		LeaveCriticalSection(&cs);
		TRACE_LOG("FixPatsDealer::cancelOrder failed. orderid:%s is not in this dealer.",order->getId().c_str());
		return;
	}
	tag11 = pInventory->tag11;
	tag37 = pInventory->tag37;
	LeaveCriticalSection(&cs);

	//发送撤单消息
	TOrderCancelMsg sender;
	assembleOrderCancelMsg(tag11,tag37,order,sender);
	if(m_pFixEngine) 
	{
		m_pFixEngine->SendMsg(&sender);
	}
	else
	{//撤单直接失败
		EnterCriticalSection(&cs);
		delInventory(pInventory);
		LeaveCriticalSection(&cs);
		TRACE_LOG("FixPatsDealer::cancelOrder failed. 撤单直接失败 can NOT find the Fix Engine");
		order->setRejected();
		return;
	}
}

int FixPatsDealer::getInvokeID()
{
	int ret = 0;
	EnterCriticalSection(&cs);
	ret = s_InvokeID;
	s_InvokeID++;
	LeaveCriticalSection(&cs);
	return ret;
}

string FixPatsDealer::assembleID(string orderguid)
{ // {8F612FF1-5BBD-429C-8645-02AD16B488EB}
	//return "{8F612FF_5";
	char idbuff[8];
	sprintf(idbuff,"_%d",getInvokeID());
	string ret = orderguid.substr(0,8) + idbuff;
	return ret;
}

void FixPatsDealer::assembleOrderCancelMsg(string tag11, string tag37, Order *order, TOrderCancelMsg &msg)
{
	Contract *pContract = Contract::get(order->getContract());
	string fixpatsName = pContract->getFixName();
	string cname = fixpatsName.substr(0,fixpatsName.find_first_of('-'));
	string cexchange = fixpatsName.substr(fixpatsName.find_first_of('@')+1);
	string cCCYYMM = fixpatsName.substr(fixpatsName.find_first_of('-')+1,
		fixpatsName.find_first_of('@')-fixpatsName.find_first_of('-')-1);
	TRACE_LOG("assembleOrderCancelMsg parse [%s] to [%s,%s,%s].",
		order->getContract().c_str(),cname.c_str(),cexchange.c_str(),cCCYYMM.c_str());

	msg.type = MSGTYPE_FIXMSG_CANCEL;
	msg.len = sizeof(TOrderCancelMsg)-sizeof(TRawMsg);
	strcpy(msg.OrigCIOrdID,tag11.c_str());
	msg.OrderID = atoi(tag37.c_str());
	sprintf(msg.CIOrdID,"%s_c_%d",tag11.c_str(),getInvokeID());
	strcpy(msg.Symbol,cname.c_str());
	strcpy(msg.SecurityType,"FUT");
	strcpy(msg.MaturityMonthYear,cCCYYMM.c_str());
	strcpy(msg.SecurityExchange,cexchange.c_str());
	if(order->getBuySell()==BUY) msg.Side = 1;
	else msg.Side = 2;
	msg.OrderQty = order->getLot();
}

void FixPatsDealer::assembleNOSMsg(string tag11, Order *order, TNewOrderSingleMsg &msg)
{
	Contract *pContract = Contract::get(order->getContract());
	string fixpatsName = pContract->getFixName();
	string cname = fixpatsName.substr(0,fixpatsName.find_first_of('-'));
	string cexchange = fixpatsName.substr(fixpatsName.find_first_of('@')+1);
	string cCCYYMM = fixpatsName.substr(fixpatsName.find_first_of('-')+1,
		fixpatsName.find_first_of('@')-fixpatsName.find_first_of('-')-1);
	TRACE_LOG("assembleNOSMsg parse [%s] to [%s,%s,%s].",
		order->getContract().c_str(),cname.c_str(),cexchange.c_str(),cCCYYMM.c_str());

	msg.type = MSGTYPE_FIXMSG_NOS;
	msg.len = sizeof(TNewOrderSingleMsg)-sizeof(TRawMsg);
	strcpy(msg.CIOrdID,tag11.c_str());
	strcpy(msg.Symbol,cname.c_str());
	strcpy(msg.SecurityType,"FUT");
	strcpy(msg.MaturityMonthYear,cCCYYMM.c_str());
	strcpy(msg.SecurityExchange,cexchange.c_str());
	msg.OrdType = 2;
	if(order->getBuySell()==BUY) msg.Side = 1;
	else msg.Side = 2;
	if(order->getKaiPing()==KAI_CANG) strcpy(msg.OpenClose,"O");
	else strcpy(msg.OpenClose,"C");
	msg.OrderQty = order->getLot();
	msg.Price = order->getPrice();
	strcpy(msg.Text,"FixPatsDealer");
}

// ************后台消息线程******************
// Fix网关回来的所有消息在此处处理
bool FixPatsDealer::OnMsg(TRawMsg *msg, IMsg *remote_src)
{
	TRACE_LOG("FixPatsDealer recv msg.");
	if ( msg->type == MSGTYPE_FIXMSG_LOGON_RESP)
	{
		TRACE_LOG("  Is MSGTYPE_FIXMSG_LOGON_RESP.");
		TLogonRespMsg* pLogonRespMsg = (TLogonRespMsg*)msg;
		if(pLogonRespMsg->FailCode == 0)
		{
			isLogonOK = true;
		}
		else
		{
			isLogonOK = false;
			TRACE_LOG("FixPatsDealer Logon fail. code:%d, text:%s.",pLogonRespMsg->FailCode,pLogonRespMsg->Reason);
		}
		::SetEvent(hLogin);
	}
	else if ( msg->type == MSGTYPE_FIXMSG_OR)
	{
		TRACE_LOG("  Is MSGTYPE_FIXMSG_OR.");
		TOrderRespMsg* pOrderRespMsg = (TOrderRespMsg*)msg;
		int status = atoi(pOrderRespMsg->OrdStatus);
		handleStatus(pOrderRespMsg,status);
	}
	else if ( msg->type == MSGTYPE_FIXMSG_CANCELREJECT)
	{// TODO
		TCancelRejectMsg* pCancelRejectMsg = (TCancelRejectMsg*)msg;
		string tag41 = pCancelRejectMsg->OrigCIOrdID;
		int status = atoi(pCancelRejectMsg->OrdStatus);
		EnterCriticalSection(&cs);
		FixPatsDealerInventory* pInventory = findInventorybyTag11(tag41);
		if(pInventory != NULL)
		{
			TRACE_LOG("FixPatsDealer[报单状态通知] 撤单拒绝 reason:%d, text:%s, orderid:%s, tag41:%s, tag37:%s, oldstatus:%d, curstatus:%d.",
				pCancelRejectMsg->reason, pCancelRejectMsg->Text,
				pInventory->order->getId().c_str(),pInventory->tag11.c_str(),pInventory->tag37.c_str(),pInventory->status,status);
		}
		LeaveCriticalSection(&cs);
	}
	else
	{
		TRACE_LOG("Unknown message 0x%x received from \'%s\'", 
			msg->type, remote_src ? remote_src->GetName() : "[Local]");
	}
	return true;
}

void FixPatsDealer::handleStatus(TOrderRespMsg *pResp, int status)
{
	string temp41 = pResp->OrigCIOrdID;
	string tag11;
	if(temp41=="")	tag11 = pResp->CIOrdID;
	else tag11 = temp41;
	EnterCriticalSection(&cs);
	FixPatsDealerInventory* pInventory = findInventorybyTag11(tag11);
	if(pInventory != NULL)
	{
		TRACE_LOG("FixPatsDealer[报单状态通知] orderid:%s, tag11:%s, tag37:%s, oldstatus:%d, curstatus:%d.",
			pInventory->order->getId().c_str(),pInventory->tag11.c_str(),pInventory->tag37.c_str(),pInventory->status,status);
		pInventory->status = status;
		char numbers[24];
		if(status==0)
		{
			TRACE_LOG("FixPatsDealer[报单状态通知] 报单被接受.拿到OrderID(tag37):%d.",pResp->OrderID);
			pInventory->tag37 = itoa(pResp->OrderID,numbers,10);
		}
		else if(status==1||status==2)
		{
			TRACE_LOG("FixPatsDealer[报单状态通知] 报单于价格 %.3f 成交 %d 手, 还剩 %d 手未成交.",pResp->LastPx,pResp->LastShares,pResp->LeavesQty);
			pInventory->order->addDetail(new OrderDetail(pInventory->order->getId(),pResp->LastShares,pResp->LastPx));
		}
		else if(status==4)
		{
			TRACE_LOG("FixPatsDealer[报单状态通知] 撤单成功.将汇报.");
			pInventory->order->setCancel();
		}
		else if(status==8)
		{
			TRACE_LOG("FixPatsDealer[报单状态通知] 报单被拒.将汇报.");
			pInventory->order->setRejected();
		}
		else
		{
			TRACE_LOG("FixPatsDealer[报单状态通知] 暂不关注该状态的报单通知.");
		}
		if (status==2 || status==4 || status==8) 
		{
			TRACE_LOG("FixPatsDealer[报单状态通知] 报单生命周期结束.将删除网关对应条目.");
			delInventory(pInventory);		
		}
	}
	LeaveCriticalSection(&cs);
}

