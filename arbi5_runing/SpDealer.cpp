// SpDealer.cpp: implementation of the SpDealer class.
//
//////////////////////////////////////////////////////////////////////

#include "SPDealer.h"
#include "LogStore.h"
#include <process.h>
#include <assert.h>

// 配置参数

static char  SPServerIp[32] ;		        // 前置地址
static int	 SPServerPort ;				    // 端口号
static char  SPInvestorId[16] ;			    // 投资者帐号代码
static char  SPUserId[16] ;		            
static char  SPPassword[16] ;			       

//******************class SpDealerInventory**********************************
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
SpDealerInventory::SpDealerInventory(Order *pOrder)
{
	order = pOrder;
	localRef = 0;
	remoteRef = -1;  //prevent to be found by a null nIntOrderNo domain.
	placeStatus = 0;
	dealedLot = 0;
}

SpDealerInventory::~SpDealerInventory()
{
}

//******************class SPDealer**********************************
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
SPDealer::SPDealer()
{
    started = false;
	hLogin = ::CreateEvent(NULL,TRUE,FALSE,NULL);
	InitializeCriticalSection(&cs);
	iNextOrderRef=0;
	ReadIniConfig();
}

SPDealer::~SPDealer()
{
	::CloseHandle(hLogin);
	DeleteCriticalSection(&cs);
}

//SpDealer 的全局接收线程
void SPDealer::dealer_receive_tcp( void* pParams )
{ 
    SPDealer *sPDealer = (SPDealer *) pParams;
	if (sPDealer == NULL) {
		TRACE_LOG(" err: sPDealer is NULL");
		return;
	}
	int len = 0;
	int code = 0;
	char szmsg[1024];
	while(1) {
		memset(szmsg, 0, sizeof(szmsg));
		//len = sPDealer->m_cTcpClinet.TCPRecive(szmsg);
		len = sPDealer->m_cTcpClinet.TCPRecvLine(szmsg);
		
		if (len > 0)
		{
			TRACE_LOG("receive msg : %s. sPDealer%p", szmsg,sPDealer);
			code = sPDealer->DecodeMsgHead(szmsg);
			switch(code) {
			case MSGID_USER_LOGIN:
				sPDealer->DealUserLoginReply(szmsg);
				break;
			case MSGID_USER_LOGOUT:
				break;
			case MSGID_ORDER_REQ:
				sPDealer->DealOrderReply(szmsg);
				break;
			case MSGID_ACCOUNT_LOGIN:
			case MSGID_ACCOUNT_LOGOUT:
				break;
			case MSGID_CLEAR_TRADE:
				TRACE_LOG("sPDealer %p ", sPDealer);
				sPDealer->DealClearTradeReply(szmsg);
				break;
			default:
				break;
			}
		}
		else
			Sleep(500);

	}
	_endthread();
}

SpDealerInventory* SPDealer::addInventory(Order *order)
{
	SpDealerInventory* ret = NULL;
	string key = order->getId();
	//看是否有重复的key
	if(dealerInventoryMap.count(key)!=0)
	{
		return ret;
	}
	ret = new SpDealerInventory(order);
	dealerInventoryMap.insert(make_pair(key,ret));
	return ret;
}

bool SPDealer::delInventory(SpDealerInventory* pDealerInventory)
{
	dealerInventoryMap.erase(pDealerInventory->order->getId());
	delete pDealerInventory;
	return TRUE;
}

SpDealerInventory* SPDealer::getInventory(string key)
{
	SpDealerInventory* ret = NULL;
	map<string, SpDealerInventory*>::iterator iter = dealerInventoryMap.find(key);
	if(iter == dealerInventoryMap.end()) return ret;
	return iter->second;
}

SpDealerInventory* SPDealer::findInventorybyLocal(int localRef)
{
	SpDealerInventory* ret = NULL;
	map<string, SpDealerInventory*>::iterator iter = dealerInventoryMap.begin();
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

SpDealerInventory* SPDealer::findInventorybyRemote(int remoteRef)
{
	SpDealerInventory* ret = NULL;
	map<string, SpDealerInventory*>::iterator iter = dealerInventoryMap.begin();
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

void SPDealer::stop()
{
	;
}

void SPDealer::ReadIniConfig()
{
	char szFrontAddr[32];
	char szport[16];
	char szBrokerId[16];
	char szInvestorId[16];
	char szPassword[16];
	memset(szFrontAddr,  0, sizeof(szFrontAddr));
	memset(szBrokerId,   0, sizeof(szBrokerId));
	memset(szInvestorId, 0, sizeof(szInvestorId));
	memset(szPassword,   0, sizeof(szPassword));

	if(GetPrivateProfileString("SP_TRADE", "SPServerIp", "127.0.0.1", szFrontAddr, 32, ".\\AIB.ini"))
	{
		strcpy_s(SPServerIp, 32, szFrontAddr);
	}
	if(GetPrivateProfileString("SP_TRADE", "SPServerPort", "8089", szport, 16, ".\\AIB.ini"))
	{
		SPServerPort = atoi(szport);
	}
	if(GetPrivateProfileString("SP_TRADE", "SPUserId", "80000003", szBrokerId, 16, ".\\AIB.ini"))
	{
        strcpy_s(SPUserId, 16, szBrokerId);
	}
	if(GetPrivateProfileString("SP_TRADE", "SPInvestorId",  "80000003", szInvestorId, 16, ".\\AIB.ini"))
	{
        strcpy_s(SPInvestorId, 16, szInvestorId);
	}
	if(GetPrivateProfileString("SP_TRADE", "SPPassword", "999999", szPassword, 16, ".\\AIB.ini"))
	{
        strcpy_s(SPPassword, 16, szPassword);
	}
}

void SPDealer::start()
{

    if (started) {
	    TRACE_LOG("SPDealer already started.");
		return;
	}
	
	TRACE_LOG("SPDealer started");
	started = true;	
	
	if (1 == startTcp())
	{
		TRACE_LOG("SpDealer TCP服务器连接成功");
	}
	else
	{
		TRACE_ALARM("SpDealer TCP服务器连接失败");
		exit(1);
	}
	_beginthread( dealer_receive_tcp, 0, (void *)this);	
	::PulseEvent(hLogin);

	SPUserLogin();

	//wait for login event signaled. ::SetEvent(hLogin)
	DWORD dwRet = WaitForSingleObject(hLogin,10*1000); //wait 10 secs at most.
	if (dwRet == WAIT_TIMEOUT)
	{//启动超时，记告警日志
		TRACE_ALARM("SPDealer Start timeout!");
		exit(1);
	}
	else if (dwRet == WAIT_OBJECT_0)
	{//正常启动
		TRACE_LOG("SPDealer Start OK!");
		return;
	}
	else //WAIT_ABANDONED
	{
		TRACE_ALARM("SPDealer Start failed!");
		exit(1);
	}

}
int  SPDealer::startTcp()
{
	int ret;
	m_cTcpClinet.StartSock(SPServerIp, SPServerPort);
	TRACE_LOG("Connecting %s:%d", SPServerIp, SPServerPort);
	ret = m_cTcpClinet.CallServer();
	return ret;
}

int  SPDealer::closeTcp()
{
	return m_cTcpClinet.TCPClose();
}

//*********解消息头**********
int  SPDealer::DecodeMsgHead(char * pmsg)
{
	strings pars;	
	m_sPPublic.divide_string(pmsg,pars,",",TRUE);
	if(pars.size() > 0)	return atoi(pars[0].c_str());
	else	return 0;
}

//*************login and out functions*******************
void SPDealer::SPUserLogin()
{
	char szUserLogin[256];
	memset(szUserLogin, 0, sizeof(szUserLogin));

	//get UserLogin msg
	strcat_s(szUserLogin, 256, "3101,0,");
	strcat_s(szUserLogin, 256, SPUserId);
	strcat_s(szUserLogin, 256, ",");
	strcat_s(szUserLogin, 256, SPPassword);
	strcat_s(szUserLogin, 256, ",");
	strcat_s(szUserLogin, 256, SPServerIp);
	strcat_s(szUserLogin, 256, "\r\n");
	//<MessageId>,<MessageType>,<UserId>,<Password>,<Host><cr><lf> e.g. 3101,0,CLIENT1,pass1,Host<cr><lf>

	if(1 != m_cTcpClinet.TCPSend(szUserLogin))
	{
		TRACE_LOG("SPdealer: send msg failed");
	}	
	TRACE_LOG("send msg login : %s", szUserLogin);
}

/*回复消息
	<MessageId>,<MessageType>,<ReturnCode>,<ReturnMessage>,<UserId>,<SvrVerNo>,<SvrRelNo>,
	<SvrLangNo>,<LoginTime>,<AccNo><cr><lf>
	e.g. 3101,3,0,OK,CLIENT1,1.0,8.07,0,1234567890,CLIENT1,192.168.1.1<cr><lf>
*/
int  SPDealer::DealUserLoginReply(char * pmsg)
{	
	strings pars;	
	int code = 0;
	int ret = 0;
    char szerr[256];
	memset(szerr, 0, sizeof(szerr));

	m_sPPublic.divide_string(pmsg,pars,",",TRUE);

	if(pars.size()>0) code = atoi(pars[0].c_str());
	if(pars.size()>2) ret = atoi(pars[2].c_str());
	if(pars.size()>3) strcpy_s(szerr,256,pars[3].c_str());
	if (0 != ret)
	{
		TRACE_LOG("msg code %d error: %s.",code, szerr);
		return 0;
	}
	else
	{
		::SetEvent(hLogin);
	}
	return 1;
	
}

void SPDealer::SPAccountLogin()
{
	char szAccountLogin[256];
	memset(szAccountLogin, 0, sizeof(szAccountLogin));

	//get AccountLogin msg
	strcat_s(szAccountLogin, 256, "3121,0,");
	strcat_s(szAccountLogin, 256, SPInvestorId);
	strcat_s(szAccountLogin, 256, "\r\n");
	//e.g. 3121,0,1000   <MessageId>,<MessageType>,<AccNo><cr><lf>

	if(1 != m_cTcpClinet.TCPSend(szAccountLogin))
	{
		TRACE_LOG("SPdealer: send msg failed");
	}
	TRACE_LOG("send msg Account login : %s", szAccountLogin);
}

void SPDealer::SPUserLogout()
{
	char szUserLogout[256];
	memset(szUserLogout, 0, sizeof(szUserLogout));

	//get UserLogout msg
	strcat_s(szUserLogout, 256, "3102,0");
	strcat_s(szUserLogout, 256, "\r\n");
	//<MessageId>,<MessageType><cr><lf> e.g. 3102,0<cr><lf>

	if(1 != m_cTcpClinet.TCPSend(szUserLogout))
	{
		TRACE_LOG("SPdealer: send msg failed");
	}
	TRACE_LOG("send msg logout : %s", szUserLogout);
}

void SPDealer::SPAccountLogout()
{
	char szAccountLogout[256];
	memset(szAccountLogout, 0, sizeof(szAccountLogout));
	
	//get AccountLogout msg
	strcat_s(szAccountLogout, 256, "3122,0,");
	strcat_s(szAccountLogout, 256, SPInvestorId);
	strcat_s(szAccountLogout, 256, "\r\n");
	//e.g. 3122,0,1000 <MessageId>,<MessageType>,<AccNo><cr><lf>

	if(1 != m_cTcpClinet.TCPSend(szAccountLogout))
	{
		TRACE_LOG("SPdealer: send msg failed");
	}
	TRACE_LOG("send msg Account logout : %s", szAccountLogout);
}
//*************End of login and out functions*******************


void SPDealer::placeOrder(Order *order)
{
	int ref;
	TRACE_LOG("SPDealer::placeOrder, orderid:%s.",order->getId().c_str());
	if(!started) 
	{
		TRACE_LOG("SPDealer::placeOrder failed. SPDealer not started.");
		order->setRejected();
		return;
	}
	//放入map中
	EnterCriticalSection(&cs);
	SpDealerInventory* pInventory = addInventory(order);
	if(pInventory == NULL)
	{//orderID与正在处理中的重复，要拒绝这次下单请求
		LeaveCriticalSection(&cs);
		TRACE_LOG("SPDealer::placeOrder failed. orderID is duplicated.");
		//order->setRejected();
		return;
	}
	//分配localRef
	ref = getCurOrderRef();
	pInventory->localRef = ref;
	LeaveCriticalSection(&cs);

	//组织消息
	char szplaceOrder[256]; memset(szplaceOrder, 0, sizeof(szplaceOrder));
	char szprice[32]; memset(szprice, 0, sizeof(szprice));
	char szQty[32]; memset(szQty, 0, sizeof(szQty));
	char szRef[16]; memset(szRef, 0, sizeof(szRef));

	sprintf_s(szplaceOrder, 256, "3103,0,1,");
	strcat_s(szplaceOrder, 256, SPInvestorId);//<AccNo>
	strcat_s(szplaceOrder, 256, ",0,");//<IntOrderNo>
	strcat_s(szplaceOrder, 256, order->getContract().c_str());//<ProductId>
	if (order->getBuySell() == BUY) strcat_s(szplaceOrder, 256, ",B,");
	else strcat_s(szplaceOrder, 256, ",S,");
	sprintf_s(szprice, 32, "%0.3f,", order->getPrice()); 
	strcat_s(szplaceOrder, 256, szprice);//<Price>
	sprintf_s(szQty, 32, "%d", order->getLot());
	strcat_s(szplaceOrder, 256, szQty);//<Qty>
	if (order->getKaiPing() == KAI_CANG) strcat_s(szplaceOrder, 256, ",O,,,,");
	else strcat_s(szplaceOrder, 256, ",C,,,,");
	sprintf_s(szRef, 16, "%d", ref);
	strcat_s(szplaceOrder, 256, szRef);//ref
	TRACE_LOG("SPDealer Send placeOrder : %s", szplaceOrder);
	strcat_s(szplaceOrder, 256, "\r\n");

	if(1 != m_cTcpClinet.TCPSend(szplaceOrder))
	{
		TRACE_LOG("SPdealer: send place order msg failed.");
		EnterCriticalSection(&cs);
		delInventory(pInventory);
		LeaveCriticalSection(&cs);
		order->setRejected();
	}
}

void SPDealer::cancelOrder(Order *order)
{
	TRACE_LOG("SPDealer::cancelOrder, orderid:%s.",order->getId().c_str());
	if(!started)
	{
		TRACE_LOG("SPDealer::cancelOrder failed. SPDealer not started.");
		return;
	}
	//查看order状态，是否需要撤单
	if(order->isCanceled()||order->isDealed()||order->isRejected())
	{
		TRACE_LOG("SPDealer::cancelOrder failed. orderid:%s is not in right status.",order->getId().c_str());
		return;
	}

	//从仓库中拿出nIntOrderNo
	int  nIntOrderNo;
	EnterCriticalSection(&cs);
	SpDealerInventory* pInventory = getInventory(order->getId());
	if(pInventory==NULL)
	{
		LeaveCriticalSection(&cs);
		TRACE_LOG("SPDealer::cancelOrder failed. orderid:%s is not in this dealer.",order->getId().c_str());
		return;
	}
	nIntOrderNo = pInventory->remoteRef;
	LeaveCriticalSection(&cs);

	if(nIntOrderNo<=0) 
	{
		TRACE_LOG("SPDealer::cancelOrder failed. orderid:%s has NOT got a nIntOrderNo yet.",order->getId().c_str());
		return;
	}
 
	//组织消息
	char szcancel[256]; memset(szcancel, 0, sizeof(szcancel));
	char szprice[32]; memset(szprice, 0, sizeof(szprice));
	char szQty[32]; memset(szQty, 0, sizeof(szQty));
	char szIntOrderNo[32]; memset(szIntOrderNo, 0, sizeof(szIntOrderNo));
	
	sprintf_s(szcancel, 256, "3103,0,3,");
	strcat_s(szcancel, 256, SPInvestorId);//<AccNo>
	sprintf_s(szIntOrderNo, 32, ",%d,",nIntOrderNo);
	strcat_s(szcancel, 256, szIntOrderNo);//<IntOrderNo>
	strcat_s(szcancel, 256, order->getContract().c_str());
	if (order->getBuySell() == BUY) strcat_s(szcancel, 256, ",B,");
	else strcat_s(szcancel, 256, ",S,");
	sprintf_s(szprice, 32, "%0.3f", order->getPrice()); 
	strcat_s(szcancel, 256, szprice);//<Price>
	sprintf_s(szQty, 32, ",%d",order->getLot());
	strcat_s(szcancel, 256, szQty);//<Qty>
	if (order->getKaiPing() == KAI_CANG) strcat_s(szcancel, 256, ",O,,,,");
	else strcat_s(szcancel, 256, ",C,,,,");
	TRACE_LOG("SPDealer Send cancel order: %s", szcancel);
	strcat_s(szcancel, 256, "\r\n");

	if(1 != m_cTcpClinet.TCPSend(szcancel))
	{
		TRACE_LOG("SPdealer: send cancel order msg failed");
	}
}

/*  来自SP网关的成交通知
	<MessageId>,<MessageType>,<RecNo>,<AccNo>,<IntOrderNo>,<ProductId>,<BuySell>,<Price>,
	<Qty>,<OpenClose>,<Ref>,<Ref2>,<DealSrc>,<TradeNo>,<Status><cr><lf>
	e.g. 3109,0,12345678,CLIENT1,321,HSIF9,B,15000,10,C,,,1,1098765,9<cr><lf>
*/
int SPDealer::DealClearTradeReply(char * pmsg)
{
	if(!started)
	{
		TRACE_LOG("DealClearTradeReply Failed. SpDealer is NOT started.");
		return -1;
	}
	strings pars;	
	int code=0; int nRecNo=0; int nstatus=0; int qty=0;	int nIntOrderNo=0;
	double dPrice;
    char szProductId[16]; memset(szProductId, 0, sizeof(szProductId));
	char szBuySell[16]; memset(szBuySell, 0, sizeof(szBuySell));
	char szOpenClose[16]; memset(szOpenClose, 0, sizeof(szOpenClose));
	
	m_sPPublic.divide_string(pmsg,pars, ",", TRUE);
	if(pars.size()>0) code = atoi(pars[0].c_str());
	if(pars.size()>2) nRecNo = atoi(pars[2].c_str());
	if(pars.size()>4) nIntOrderNo = atoi(pars[4].c_str());
	if(pars.size()>5) strcpy_s(szProductId, 16, pars[5].c_str());
	if(pars.size()>6) strcpy_s(szBuySell, 16, pars[6].c_str());
	if(pars.size()>7) dPrice = atof(pars[7].c_str());
	if(pars.size()>8) qty = atoi(pars[8].c_str());
	if(pars.size()>9) strcpy_s(szOpenClose, 16, pars[9].c_str());
	if(pars.size()>14) nstatus = atoi(pars[14].c_str());

	TRACE_LOG("SPDealer 收到3109, nIntOrderNo:%d.", nIntOrderNo);

	//修改map中相应单子的状态，执行相应动作
	Order *pOrder = NULL;
	EnterCriticalSection(&cs);
	SpDealerInventory* pInventory = findInventorybyRemote(nIntOrderNo);
	if(pInventory == NULL)
	{//无记录的单号
		TRACE_LOG("SPDealer 3109中的nIntOrderNo无记录,判为不关注的trade.");
	}
	else
	{
		//add dealed lot to judge if this is a All_TRADED.
		pInventory->dealedLot += qty;
		if(pInventory->dealedLot >= pInventory->order->getLot())//nstatus == SP_ALL_TRADED) 
		{
			nstatus = SP_ALL_TRADED;
			pInventory->placeStatus = 3;
			pOrder = pInventory->order;
			delInventory(pInventory);
		}
		else if(pInventory->dealedLot < pInventory->order->getLot())//nstatus == SP_PARTIAL_TRADED)
		{
			nstatus = SP_PARTIAL_TRADED;
			pInventory->placeStatus = 2;
			pOrder = pInventory->order;
		}
	}
	LeaveCriticalSection(&cs);
	
	//*******for debug print***********
	if(pInventory!=NULL)
	{
		TRACE_LOG("SPdealer 关注的3109: %s", pmsg);
		if(nstatus == SP_ALL_TRADED) 
		{
			TRACE_LOG("SPdealer 关注的3109 判为全部成交");
		}
		else if(nstatus == SP_PARTIAL_TRADED)
		{
			TRACE_LOG("SPdealer 关注的3109 判为部分成交");
		}
	}
	

	//发送确认消息
	char szClearTradeAck[256]; memset(szClearTradeAck, 0, sizeof(szClearTradeAck));
	sprintf_s(szClearTradeAck,256, "3109,3,%d\r\n",nRecNo);	
	if(1 != m_cTcpClinet.TCPSend(szClearTradeAck))
	{
		TRACE_LOG("SPdealer: send 3109 ack msg failed");
	}
	//向上层汇报交易结果
	if(pOrder!=NULL)
	{
		TRACE_LOG("SpDealer 交易通知. id:%s, 手数:%d, 价格:%.3f.", pOrder->getId().c_str(),qty,dPrice);
		pOrder->addDetail(new OrderDetail(pOrder->getId(),qty,dPrice));
	}
	return 1;
}

/*  SP网关的报单状态通知
	<MessageId>,<MessageType>,<ReturnCode>,<ReturnMessage>,<Status>,<Action>,<AccNo>,<IntOrderNo>
	,<ProductId>,<BuySell>,<Price>,<Qty>,<OpenClose>,<OrderType>,<ValidType>,<ValidTime>,<Ref>,
	<Initiator><cr><lf>
	e.g. 3103,3,0,OK,1,1,CLIENT1,0,HSIF9,B,15000,10,,,,,<cr><lf>
	(*) or 3103,3,0,OK,1,1,CLIENT1,0,HSIF9,B,15000,10<cr><lf>
*/
int SPDealer::DealOrderReply(char * pmsg)
{
	strings pars;	
	int code=0;	int nret=-1; int nMessageType=0; int nstatus=0;	int nAction=0; int nRef=0; int nIntOrderNo=0; int qty=0;
	double price;
    char szerr[256]; memset(szerr, 0, sizeof(szerr));
	m_sPPublic.divide_string(pmsg,pars, ",", TRUE);
	if(pars.size()>0) code = atoi(pars[0].c_str());
	if(pars.size()>1) nMessageType = atoi(pars[1].c_str());
	if(pars.size()>2) nret = atoi(pars[2].c_str());
	if(pars.size()>3) strcpy_s(szerr, 256, pars[3].c_str());
	if(pars.size()>4) nstatus = atoi(pars[4].c_str());
	if(pars.size()>5) nAction = atoi(pars[5].c_str());
	if(pars.size()>7) nIntOrderNo = atoi(pars[7].c_str());
	if(pars.size()>10) price = atof(pars[10].c_str());
	if(pars.size()>11) qty = atoi(pars[11].c_str());
	if(pars.size()>16) nRef = atoi(pars[16].c_str());
	
	TRACE_LOG("SPDealer 收到3103 nRef:%d,nIntOrderNo:%d.",nRef,nIntOrderNo);
	
	EnterCriticalSection(&cs);
	SpDealerInventory* pInventory = findInventorybyLocal(nRef);
	if(pInventory == NULL)
	{
		TRACE_LOG("SPDealer 3103中的nRef无记录,继续查找nIntOrderNo.");
		pInventory = findInventorybyRemote(nIntOrderNo);
		if(pInventory == NULL)
		{
			TRACE_LOG("SPDealer 3103中的nIntOrderNo无记录,判为不关注的报单状态.");
			LeaveCriticalSection(&cs);
			return TRUE;
		}
	}
	TRACE_LOG("SPDealer 关注的3103: %s", pmsg);
	if(nIntOrderNo!=0 && pInventory->remoteRef==-1) 
	{
		pInventory->remoteRef = nIntOrderNo;
		pInventory->placeStatus = 1;
		TRACE_LOG("SPDealer 关注的3103  判断为报单已接受.");
	}
	if(nAction==1 && nret<0)
	{//报单被拒 nAction==1 means MSGID_ORDER_REQ_ADD
		TRACE_LOG("SPDealer 关注的3103  判断为报单被拒.");
		pInventory->placeStatus = 5;
		pInventory->order->setRejected();
		delInventory(pInventory);
	}
	else if(nAction==3 && (nret <= 0 && nstatus == SP_DELETED))
	{//撤单成功 nAction==3 means MSGID_ORDER_REQ_DEL
		TRACE_LOG("SPDealer 关注的3103  判断为撤单成功.");
		pInventory->placeStatus = 4;
		pInventory->order->setCancel();
		delInventory(pInventory);
	}
	LeaveCriticalSection(&cs);
    return 0;

}

int SPDealer::getCurOrderRef()
{
	return iNextOrderRef++;
}
