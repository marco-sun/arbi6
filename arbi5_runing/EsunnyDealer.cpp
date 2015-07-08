#include "stdafx.h"
#include "EsunnyDealer.h"
#include "Contract.h"
#include "LogStore.h"
#include <process.h>

// 配置参数,将从配置文件中读出
static TEsAddressField ADDR;
static TEsLoginReqField LOGIN;

static const char* g_szStateDesc[] = {"无效单", "新单", "已排队", "完全成交", "撤单请求", "已撤单", "埋单", 
            "埋单删除", "已发送", "待撤", "部分成交", "非交易时间", "资金不足", "交易关闭", 
            "指令失败", "撤单失败", "系统撤单", "触发单", "触发单删除", "挂起", "激活", "已受理", 
            "自动单", "自动单删除", "未知"};


static EsunnyDealer* sp_esDealer = NULL;
//******************class EsunnyDealerInventory**********************************
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
EsunnyDealerInventory::EsunnyDealerInventory(Order *pOrder)
{
	order = pOrder;
	localRef = -1;
	remoteRef = "";
	systemNo = "";
	placeStatus = 0;
	dealedLot = 0;
}

EsunnyDealerInventory::~EsunnyDealerInventory()
{
}

//******************class EsunnyDealer**********************************
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
EsunnyDealer::EsunnyDealer(void)
{
	started = false;
	hLogin = ::CreateEvent(NULL,TRUE,FALSE,NULL);
	ReadIniConfig();
	InitializeCriticalSection(&cs);
}


EsunnyDealer::~EsunnyDealer(void)
{
	if(started)	stop();
	DeleteCriticalSection(&cs);
	::CloseHandle(hLogin);
}

////////////////////////////////////////
/// \fn    GetOrderStateDesc
/// \brief 获取状态描述
/// \param int iState
/// \return   int
////////////////////////////////////////
const char* EsunnyDealer::GetOrderStateDesc(unsigned int iState)
{
    if (iState > sizeof(g_szStateDesc)/sizeof(*g_szStateDesc))
    {
        iState = sizeof(g_szStateDesc)/sizeof(*g_szStateDesc) - 1;
    }
    return g_szStateDesc[iState];
}

EsunnyDealer* EsunnyDealer::getInstance()
{
	if(sp_esDealer == NULL)
		sp_esDealer = new EsunnyDealer();
	return sp_esDealer;
}

//////////////////////////////////////////////////////////////////////
// Inventory Operations
//////////////////////////////////////////////////////////////////////
EsunnyDealerInventory* EsunnyDealer::addInventory(Order *order)
{
	EsunnyDealerInventory* ret = NULL;
	string key = order->getId();
	//看是否有重复的key
	if(dealerInventoryMap.count(key)!=0)
	{
		return ret;
	}
	ret = new EsunnyDealerInventory(order);
	dealerInventoryMap.insert(make_pair(key,ret));
	return ret;
}

bool EsunnyDealer::delInventory(EsunnyDealerInventory* pDealerInventory)
{
	dealerInventoryMap.erase(pDealerInventory->order->getId());
	delete pDealerInventory;
	return TRUE;
}

EsunnyDealerInventory* EsunnyDealer::getInventory(string key)
{
	EsunnyDealerInventory* ret = NULL;
	map<string, EsunnyDealerInventory*>::iterator iter = dealerInventoryMap.find(key);
	if(iter == dealerInventoryMap.end()) return ret;
	return iter->second;
}

EsunnyDealerInventory* EsunnyDealer::findInventorybyLocal(int localRef)
{
	EsunnyDealerInventory* ret = NULL;
	map<string, EsunnyDealerInventory*>::iterator iter = dealerInventoryMap.begin();
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

EsunnyDealerInventory* EsunnyDealer::findInventorybyRemote(string remoteRef)
{
	EsunnyDealerInventory* ret = NULL;
	map<string, EsunnyDealerInventory*>::iterator iter = dealerInventoryMap.begin();
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

EsunnyDealerInventory* EsunnyDealer::findInventorybySys(string sysNo)
{
	EsunnyDealerInventory* ret = NULL;
	map<string, EsunnyDealerInventory*>::iterator iter = dealerInventoryMap.begin();
	while(iter != dealerInventoryMap.end())
	{
		if(iter->second->systemNo == sysNo)
		{
			ret = iter->second;
			break;
		}
		iter ++;
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////
// Configuration
//////////////////////////////////////////////////////////////////////
void EsunnyDealer::ReadIniConfig()
{
	char szFrontAddr[16];
	int nPort;
	char szInvestorId[16];
	char szPassword[16];
	memset(szFrontAddr,  0, sizeof(szFrontAddr));
	memset(szInvestorId, 0, sizeof(szInvestorId));
	memset(szPassword,   0, sizeof(szPassword));

	memset(&LOGIN,0,sizeof(LOGIN));

	if(GetPrivateProfileString("ESUNNY", "IP", "222.88.40.170", szFrontAddr, 16, ".\\AIB.ini"))
	{
		strncpy_s(ADDR.Ip, sizeof(ADDR.Ip), szFrontAddr, sizeof(ADDR.Ip) - 1);
	}
	nPort = GetPrivateProfileInt("ESUNNY", "PORT", 6060, ".\\AIB.ini");
	ADDR.Port = nPort;
	
	LOGIN.LoginType = 'C';
	if(GetPrivateProfileString("ESUNNY", "LOGIN_NO",  "SSS", szInvestorId, 16, ".\\AIB.ini"))
	{
		strncpy_s(LOGIN.LoginNo, sizeof(LOGIN.LoginNo), szInvestorId, sizeof(LOGIN.LoginNo) - 1);
	}
	if(GetPrivateProfileString("ESUNNY", "PASSWORD", "sss123456", szPassword, 16, ".\\AIB.ini"))
	{
		strncpy_s(LOGIN.Password, sizeof(LOGIN.Password), szPassword, sizeof(LOGIN.Password) - 1);
	}
}

//////////////////////////////////////////////////////////////////////
// Start & Stop
//////////////////////////////////////////////////////////////////////
//同步的start,等登录完结才返回
void EsunnyDealer::start()
{
	bool bRet;
    if(started) {
	    TRACE_LOG("EsunnyDealer already started.");
		return;
	}
	// 1. 进行认证
	bRet = CertEsunnyApi("4A35B623174363F3A1BDD4D66D1F66AC7A368C423BB68CAFD0C78CDBA812DAAD9AAD1C1052E60447B35F0E2237A83160554F", "");	
    if (!bRet)
    {
       TRACE_ALARM("EsunnyDealer Starts failed! [Cert FAIL!]");
		exit(1);
    }
	// 2. 获取处理类实例
	pUserSpi = new EsunnyDealerSpi();
	if((pUserApi=CreateEsunnyTradeApi()) == NULL)
	{
		TRACE_ALARM("EsunnyDealer Starts failed! [Create API FAIL!]");
		exit(1);
	}
	pUserSpi->m_pDealer = this;
	pUserApi->SetSpi((IEsunnyTradeSpi*)pUserSpi);			// 注册事件类

	// 3. 登录流程
	::ResetEvent(hLogin);
	bRet = pUserApi->Open(ADDR);
	if (!bRet)
    {
       TRACE_ALARM("EsunnyDealer Starts failed! [pUserApi->Open(ADDR) FAIL!]");
		exit(1);
    }
	// 4. wait for login event signaled. ::SetEvent(hLogin)
	DWORD dwRet = WaitForSingleObject(hLogin,10*1000); //wait 10 secs at most.
	if (dwRet == WAIT_TIMEOUT)
	{//启动超时，记告警日志，退出程序
		TRACE_ALARM("EsunnyDealer Starts failed! [WAIT_TIMEOUT]");
		exit(1);
	}
	else if (dwRet == WAIT_OBJECT_0)
	{//正常启动
		TRACE_LOG("EsunnyDealer Starts OK.");
	    started = true;		
		return;
	}
	else //WAIT_ABANDONED
	{
		TRACE_ALARM("EsunnyDealer Starts failed! [WAIT_ABANDONED]");
		exit(1);
	}
}

void EsunnyDealer::stop()
{
	if(pUserApi)
	{
		if(pUserApi->IsOpen()) pUserApi->Close();
		pUserApi->Free();
		pUserApi=NULL;
	}
	if(pUserSpi)
	{
		delete pUserSpi;
		pUserSpi = NULL;
	}
	started = false;
}

//////////////////////////////////////////////////////////////////////
// Place & Cancel Orders
//////////////////////////////////////////////////////////////////////
void EsunnyDealer::placeOrder(Order *order)
{
	int ref;
	//放入map中,执行相应动作,开启等待线程
	TRACE_LOG("EsunnyDealer::placeOrder, orderid:%s.",order->getId().c_str());
	if(!started)
	{
		TRACE_LOG("EsunnyDealer::placeOrder failed. EsunnyDealer not started.");
		order->setRejected();
		return;
	}
	//放入map中
	EnterCriticalSection(&cs);
	EsunnyDealerInventory* pInventory = addInventory(order);
	if(pInventory == NULL)
	{//orderID与正在处理中的重复，要拒绝这次下单请求
		LeaveCriticalSection(&cs);
		TRACE_LOG("EsunnyDealer::placeOrder failed. orderID is duplicated.");
		return;
	}
	//分配localRef
	ref = pUserSpi->GenerateReqID();
	pInventory->localRef = ref;
	LeaveCriticalSection(&cs);
	//执行下单动作
	char exchangeCode = Contract::get(order->getContract())->getExchangeNumberForYiSheng();
	if(0 != pUserSpi->InsertOrder(ref,exchangeCode,order))
	{//下单直接失败
		EnterCriticalSection(&cs);
		delInventory(pInventory);
		LeaveCriticalSection(&cs);
		TRACE_LOG("EsunnyDealer::placeOrder failed. 下单直接失败");
		order->setRejected();
		return;
	}
	return;
}

void EsunnyDealer::cancelOrder(Order *order)
{
	//修改map中相应单子的状态，执行相应动作
	string orderNo="";
	string sysNo="";
	TRACE_LOG("EsunnyDealer::cancelOrder, orderid:%s.",order->getId().c_str());
	if(!started)
	{
		TRACE_LOG("EsunnyDealer::cancelOrder failed. EsunnyDealer not started.");
		return;
	}
	//查看order状态，是否需要撤单
	if(order->isCanceled()||order->isDealed()||order->isRejected())
	{
		TRACE_LOG("EsunnyDealer::cancelOrder failed. orderid:%s is not in right status.",order->getId().c_str());
		return;
	}
	//从仓库中拿出委托号和系统号
	EnterCriticalSection(&cs);
	EsunnyDealerInventory* pInventory = getInventory(order->getId());
	if(pInventory==NULL)
	{
		LeaveCriticalSection(&cs);
		TRACE_LOG("EsunnyDealer::cancelOrder failed. orderid:%s is not in this dealer.",order->getId().c_str());
		return;
	}
	orderNo = pInventory->remoteRef;
	sysNo = pInventory->systemNo;
	LeaveCriticalSection(&cs);
	//执行相应动作
	char exchangeCode = Contract::get(order->getContract())->getExchangeNumberForYiSheng();
	if(0 != pUserSpi->CancelOrder(exchangeCode,orderNo,sysNo,order))
	{
		TRACE_LOG("EsunnyDealer::cancelOrder failed. orderid:%s 撤单直接失败",order->getId().c_str());
		return;
	}
	return;
}


//网关回调过来的报单应答
//通过请求编号ref映射回inventory,
//再根据pOrder内容修改inventory
void EsunnyDealer::OnReturnQryOrder(TEsOrderInsertRspField* pRsp)
{
	int reqId = pRsp->ReqData.RequestID;
	string orderNo = pRsp->OrderNo;
	EnterCriticalSection(&cs);
	EsunnyDealerInventory* pInventory = findInventorybyLocal(reqId);
	if(pInventory!=NULL)
	{
		pInventory->remoteRef = orderNo;
		TRACE_DEBUG("[EsunnyTrade网关回调]报单响应。报单请求号:%d,报单委托号:%s,状态:%s,附加信息:%s.",
			pRsp->ReqData.RequestID,pRsp->OrderNo,GetOrderStateDesc(pRsp->OrderState),pRsp->OrderEcho);	
		if(pRsp->OrderState == ORDER_INVALID || pRsp->OrderState == ORDER_NON_TRADING_HOURS || pRsp->OrderState == ORDER_INSUFFICIENT_FUNDS)
		{//报单被拒
			pInventory->placeStatus = 5;
			TRACE_DEBUG("[EsunnyTrade网关回调]报单响应,判断为报单被拒.将汇报.");
			pInventory->order->setRejected();
			delInventory(pInventory);
		}
	}
	LeaveCriticalSection(&cs);
}

//网关回调过来的报单状态通知
//通过委托号remoteref映射回inventory
//再根据pOrder内容修改inventory
void EsunnyDealer::OnReturnOrder(TEsOrderInsertRspField& rsp)
{
	string orderNo = rsp.OrderNo;;
	
	EnterCriticalSection(&cs);
	EsunnyDealerInventory* pInventory = findInventorybyRemote(orderNo);
	handleOrderStatusReport(pInventory,rsp);
	LeaveCriticalSection(&cs);
}

//网关回调过来的成交通知
//通过系统号映射回inventory,再根据pOrder内容修改inventory
void EsunnyDealer::OnTrade(TEsMatchRspField& rsp)
{
	string sysNo = rsp.SystemNo;
	EnterCriticalSection(&cs);
	EsunnyDealerInventory* pInventory = findInventorybySys(sysNo);
	handleTradeReport(pInventory,rsp);
	LeaveCriticalSection(&cs);
}


//委托状态 0-无效单,1-新单,2-已排队,3-完全成交,4-撤单请求,5-已撤单,6-埋单,7-埋单删除,
//8-已发送, 9-待撤,10-部分成交,11-非交易时间,12-资金不足,13-交易关闭,14-指令失败,
//15-撤单失败,16-系统撤单, 17-触发单,18-触发单删除,19-挂起,20-激活,21-已受理,22-自动单,23-自动单删除 
void EsunnyDealer::handleOrderStatusReport(EsunnyDealerInventory* pInventory, TEsOrderInsertRspField& rsp)
{
	if(pInventory == NULL) return;
	TRACE_DEBUG("[EsunnyTrade网关回调]报单状态通知。报单:%s,状态:%s,附加信息:%s.",
		rsp.OrderNo,GetOrderStateDesc(rsp.OrderState),rsp.OrderEcho);		

	if (rsp.OrderState == ORDER_WITHDRAWALS_FAILED)
	{//撤单被拒
		TRACE_DEBUG("[EsunnyTrade网关回调]报单状态通知,判断为撤单被拒.");
	}
	else if (rsp.OrderState == ORDER_CANCELED)
	{//撤单成功
		pInventory->placeStatus = 4;
		TRACE_DEBUG("[EsunnyTrade网关回调]报单状态通知,判断为撤单成功.将汇报.");
		pInventory->order->setCancel();
		delInventory(pInventory);
	}
	else if(rsp.OrderState == ORDER_INVALID || rsp.OrderState == ORDER_NON_TRADING_HOURS || rsp.OrderState == ORDER_INSUFFICIENT_FUNDS)
	{//报单被拒
		pInventory->placeStatus = 5;
		TRACE_DEBUG("[EsunnyTrade网关回调]报单状态通知,判断为报单被拒.将汇报.");
		pInventory->order->setRejected();
		delInventory(pInventory);
	}
	else
	{//这里是报单的情况
		if(pInventory->systemNo == "") pInventory->systemNo = rsp.SystemNo;

		if(rsp.OrderState == ORDER_NEW) //新报单已经被接受
		{
			TRACE_DEBUG("[EsunnyTrade网关回调]报单状态通知,判断为新报单已经被接受.");
			pInventory->placeStatus = 1;
		}
		else if (rsp.OrderState == ORDER_IN_QUEUE)
		{//挂单的情况
			TRACE_DEBUG("[EsunnyTrade网关回调]报单状态通知,判断为挂单.");
		}
		else if (rsp.OrderState == ORDER_ALL_MATCH)
		{//立刻成交的情况
			TRACE_DEBUG("[EsunnyTrade网关回调]报单状态通知,判断为全部成交.");
		}
		else if (rsp.OrderState == ORDER_PART_MATCH)
		{//部分成交部分挂单的情况
			TRACE_DEBUG("[EsunnyTrade网关回调]报单状态通知,判断为部分成交部分挂单.");
		}
		else
		{//其他情况
			TRACE_DEBUG("[EsunnyTrade网关回调]报单状态通知,判断为其他情况.");
		}
	}
}

void EsunnyDealer::handleTradeReport(EsunnyDealerInventory* pInventory, TEsMatchRspField& rsp)
{
	if(pInventory == NULL) return;
	TRACE_LOG("[EsunnyTrade网关回调]关注的成交。orderID:%s,sysNo:%s,于 %.3f 成交 %d 手, 时间 %s.",
		pInventory->order->getId().c_str(),rsp.SystemNo,rsp.MatchPrice,rsp.MatchVol,rsp.MatchTime);

	pInventory->dealedLot += rsp.MatchVol;
	if(pInventory->dealedLot < pInventory->order->getLot())
	{//partial dealed.
		pInventory->placeStatus = 2;
		pInventory->order->addDetail(new OrderDetail(pInventory->order->getId(),rsp.MatchVol,rsp.MatchPrice));
	}
	else
	{//all dealed.
		pInventory->placeStatus = 3;
		pInventory->order->addDetail(new OrderDetail(pInventory->order->getId(),rsp.MatchVol,rsp.MatchPrice));
		delInventory(pInventory);
	}
}


//////////////////////////////////////////////////////////////////////
// EsunnyDealerSpi
//////////////////////////////////////////////////////////////////////
void EsunnyDealerSpi::Convert2Uppercase(char content[], int length)
{
	for(int i=0;i<length;i++)
	{
		if(content[i] == 0x00) break;
		if(content[i]>='a' && content[i]<='z')
		{
			content[i] -= 0x20;
		}

	}
}
//报单的网关接口实现方法
int EsunnyDealerSpi::InsertOrder(int orderRef, char exchangeCode, Order *order)
{
	 // 下单操作
    TEsOrderInsertReqField req;
    memset(&req, 0, sizeof(req));
	///合约代码
	req.Exchange = exchangeCode;
    strncpy_s(req.Contract1, sizeof(req.Contract1), order->getContract().c_str(), sizeof(req.Contract1) - 1);
	Convert2Uppercase(req.Contract1,sizeof(req.Contract1));
	///买卖方向: '1'-买入,'3'-卖出 
	if(order->getBuySell()==BUY)
		req.Direct = '1';
	else
		req.Direct = '3';
    ///组合开平标志: '1'-开仓,'2'-平今(上期所平今),'3'-平仓(上期所平昨) 
	if(order->getKaiPing()==KAI_CANG)
		req.Offset = '1';
	else if (order->getKaiPing()==PING_CANG)
		req.Offset = '3';
	else
		req.Offset = '2';

    req.OrderPrice  = order->getPrice(); // 价格
    req.OrderVol    = order->getLot();   //本次报单的目标手数
    req.RequestID   = orderRef;
	/// 固定参数
	req.Hedge       = '1'; //投保标识 '1'-投机,'3'-保值 
    req.CmbType     = 0;    //组合类型 0-单腿,1-跨期,2-跨品种 
    req.OrderType   = 0;    //委托类型 0-限价,1-市价

	int iResult = m_pDealer->pUserApi->OrderInsert(req);

	TRACE_LOG("Exchange:%c, RequestID: %d \n",req.Exchange, req.RequestID);
	TRACE_LOG("[EsunnyTrade网关]报单录入请求: reqid:%d, %s,[%c,%c],%.3f, lot:%d, %s.",
		orderRef,req.Contract1,req.Offset,req.Direct,req.OrderPrice,req.OrderVol,
		((iResult == 0) ? "发送成功" : "发送失败"));
	return iResult;
}

//撤单的网关接口实现方法
int EsunnyDealerSpi::CancelOrder(char exchangeCode, string orderNo, string sysNo, Order *order)
{
	TEsOrderActionReqField req;
	memset(&req, 0, sizeof(req));
	// 现在仅填委托号,系统号和合约名,交易所
	strncpy_s(req.OrderNo, sizeof(req.OrderNo),
            orderNo.c_str(), sizeof(req.OrderNo) - 1);
	strncpy_s(req.SystemNo, sizeof(req.SystemNo),
            sysNo.c_str(), sizeof(req.SystemNo) - 1);
	strncpy_s(req.Contract, sizeof(req.Contract),
            order->getContract().c_str(), sizeof(req.Contract) - 1);
	Convert2Uppercase(req.Contract,sizeof(req.Contract));
	req.Exchange = exchangeCode;
	
	int iResult = m_pDealer->pUserApi->OrderAction(req);
	TRACE_LOG("[EsunnyTrade网关]撤单操作请求: 合约：%s, 委托号%s, 系统号%s, %s.",
		req.Contract, req.OrderNo,req.SystemNo,((iResult == 0) ? "发送成功" : "发送失败"));
	return iResult;
}


void __cdecl EsunnyDealerSpi::OnOpen()
{
	m_iConnState = STATE_CONNECTED;
	TRACE_LOG("[EsunnyTrade网关回调]IP连接已建立.");
	// start logon
	int iRet = m_pDealer->pUserApi->Login(LOGIN);
	TRACE_LOG("[EsunnyTrade网关]发送用户登录请求: %s,%s.",
		LOGIN.LoginNo,LOGIN.Password,((iRet == 0) ? "发送成功" : "发送失败"));
}

void __cdecl EsunnyDealerSpi::OnClose()
{
	m_iConnState = STATE_DISCONNECTED; 
	TRACE_LOG("[EsunnyTrade网关回调]IP连接中断.");
}

void __cdecl EsunnyDealerSpi::OnLogin(TEsRspField& rsp)
{
	if(rsp.ErrorCode==0)
	{
		m_iConnState = STATE_LOGIN;
		TRACE_LOG("[EsunnyTrade网关回调]用户登录成功.");
		::SetEvent(m_pDealer->hLogin);
	}
	else
	{
		TRACE_LOG("[EsunnyTrade网关回调]用户登录失败:[code=%d,info=%s]",rsp.ErrorCode,rsp.ErrorInfo);
	}

	// 成功登录交易服务器后才能连接行情服务器
	/*
	TEsAddressField addr1;
        strncpy_s(addr1.Ip, sizeof(addr1.Ip), "123.15.58.21", sizeof(addr1.Ip) - 1);
        addr1.Port = 6161;

        bool bRet = g_oQuoteApi.m_Api->Open(addr1);
        if (!bRet)
        {
            EnterCriticalSection(&g_CrtalSec);
            std::cout << "发送连接请求到行情服务器失败！" << std::endl;
            LeaveCriticalSection(&g_CrtalSec);
        }
	*/
}

void __cdecl EsunnyDealerSpi::OnQryOrder(TEsOrderInsertRspField* pRsp, TEsIsLastType islast)
{
	// ***********Begin of for test only **********
	char szOrderNo[22];
	memset(szOrderNo,0,22);
	strncpy_s(szOrderNo,21,pRsp->OrderNo,21);
	TRACE_DEBUG("[EsunnyTrade网关回调调试]OnQryOrder入口处 请求号:%d 对应的委托号:%s,状态:%s.",
		 pRsp->ReqData.RequestID, szOrderNo ,EsunnyDealer::GetOrderStateDesc(pRsp->OrderState));
	// *********** End of for test only **********
	m_pDealer->OnReturnQryOrder(pRsp);
}

void __cdecl EsunnyDealerSpi::OnRtnOrder(TEsOrderInsertRspField& rsp)
{
	// ***********Begin of for test only **********
	char szOrderNo[22];
	memset(szOrderNo,0,22);
	strncpy_s(szOrderNo,21,rsp.OrderNo,21);
	char szSysNo[22];
	memset(szSysNo,0,22);
	strncpy_s(szSysNo,21,rsp.SystemNo,21);
	char szErrInfo[52];
	memset(szErrInfo,0,52);
	strncpy_s(szErrInfo,51,rsp.OrderEcho,51);
	TRACE_DEBUG("[EsunnyTrade网关回调调试]报单状态通知入口处 委托号:%s,请求号:%d,系统号:%s,状态:%s,附加信息:%s.",
		szOrderNo,rsp.ReqData.RequestID,szSysNo,EsunnyDealer::GetOrderStateDesc(rsp.OrderState),szErrInfo);
	// *********** End of for test only **********
	m_pDealer->OnReturnOrder(rsp);
}

void __cdecl EsunnyDealerSpi::OnRtnMatch(TEsMatchRspField& rsp)
{
	// ***********Begin of for test only **********
	char szSysNo[22];
	memset(szSysNo,0,22);
	strncpy_s(szSysNo,21,rsp.SystemNo,21);
	char szContract[22];
	memset(szContract,0,22);
	strncpy_s(szContract,21,rsp.Contract,21);
	TRACE_DEBUG("[EsunnyTrade网关回调调试]成交通知入口处 系统号:%s,交易所:%c,合约:%s,价格:%.3f,手数:%d.",
		szSysNo,rsp.Exchange,szContract,rsp.MatchPrice,rsp.MatchVol);
	// *********** End of for test only **********
	m_pDealer->OnTrade(rsp);
}
