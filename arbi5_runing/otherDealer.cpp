#include "otherDealer.h"
#include "LogStore.h"
#include <process.h>
#include <cstdlib>
#include <fstream> 



static char  BoHaiInvestorId[16];// 投资者帐号代码


otherDealerInventory::otherDealerInventory(Order *pOrder)
{
	memset(sysID, 0, 64);
	order = pOrder;
	localRef = 0;
	remoteRef = -1;
	placeStatus = 0;
	dealedLot = 0;
}

otherDealerInventory::~otherDealerInventory()
{

}



//-------------------------------------------------------------------
otherDealer::otherDealer()
{
    started = false;
	hLogin = ::CreateEvent(NULL,TRUE,FALSE,NULL);
	InitializeCriticalSection(&cs);
	iNextOrderRef=0;
	ReadIniConfig();
}

otherDealer::~otherDealer()
{

}

void otherDealer::ReadIniConfig()
{
	char szInvestorId[16];
	if(GetPrivateProfileString("BOHAI_TRADE", "otherInvestorId",  "80000003", szInvestorId, 16, ".\\AIB.ini"))
	{
        strcpy_s(BoHaiInvestorId, 16, szInvestorId);
	}
}

bool otherDealer::startRedis()
{
    struct timeval timeout = { 1, 500000 }; // 1.5 seconds

	WSADATA wsaData;
	WSAStartup(MAKEWORD( 2, 2 ), &wsaData);

	c = redisConnect((char*)"192.168.11.105", 6379);
	w = redisConnect((char*)"192.168.11.105", 6379);
	if (c->err || w->err) {
        printf("Connection error: %s\n", c->errstr);
        return false;
    }
	return true;
}

bool wait_for_write (string path)
{
    HANDLE handle = FindFirstChangeNotification(LPCSTR(path.c_str()), FALSE, FILE_NOTIFY_CHANGE_LAST_WRITE);
    WaitForSingleObject(handle, INFINITE);
    CloseHandle(handle);
    return true;
}

void otherDealer::receiveOrderReport(void* pParams) 
{
	
	otherDealer * ps = (otherDealer*)pParams;
	char reply[64];
	memset(reply, 0, sizeof(reply));
	//redisAppendCommand(ps->w,"SUBSCRIBE Status-MiscEX");
	while(wait_for_write("Status"))
	{
		/*
		ps->reply = NULL;
		redisGetReply(ps->w,(void **)&(ps->reply));
		ps->printReply(ps->reply);
		if(ps->reply!=NULL)	freeReplyObject(ps->reply);	
		*/
		ifstream statusfile("Status\\Status.csv");
		if(statusfile.fail()) {
			TRACE_LOG("open Status.txt fail");
			return;
		}
		statusfile >> reply;
		ps->compute(reply);
	}
	
}

void otherDealer::printReply(redisReply *reply)
{
	/*
	if(reply == NULL) return;
	if(reply->type == REDIS_REPLY_ARRAY)
	{
		//TRACE_LOG("Redis reveive REDIS_REPLY_ARRAY\n");
		redisReply * r1 = *(reply->element + 2);
		if(r1 == NULL) TRACE_LOG("reply error");
		if(r1->type == REDIS_REPLY_STRING) compute(r1);
	} 
	else 
	{
		TRACE_LOG("Redis reveive other type\n");
	}
	*/
}

void otherDealer::compute(char *reply)
{
	TRACE_LOG("otherDealer receive: %s",reply);
	EnterCriticalSection(&cs);
	char *e;
	strings pars;	
	strings::iterator it;		
	int npar = 0;
	int code = 0;
	m_sPPublic.divide_string(reply , pars ,"," , TRUE);
	char  OrderID[64];
	char  AccountID[64];
	char  State[64];
	double  Lot;
	double  Price;
	for(npar = 0,it = pars.begin(); it != pars.end();it++,npar++)
	{
		switch(npar) {
		case 0:
			memset(OrderID, 0, 64);
			strcpy_s(OrderID, 64, (* it).c_str());
			break;
		case 1:
			memset(AccountID, 0, 64);
			strcpy_s(AccountID, 64, (* it).c_str());
			break;
		case 2:
			Price = strtod((* it).c_str(), &e);
			if(*e != 0) return;
			break;
		case 3:
			Lot = strtod((* it).c_str(), &e);
			if(*e != 0) return;
			break;
		case 4:
			memset(State, 0, 64);
			strcpy_s(State, 64, (* it).c_str());
			break;
		default:
			break;
		}
	}
	otherDealerInventory* pInventory = findInventorybySysID(OrderID);
	if(pInventory == NULL)
	{
		TRACE_LOG("otherDealer 无法查到单号：%s", OrderID);
	}
	else
	{
		TRACE_LOG("otherDealer 单号：%s, 全部成交", pInventory->sysID);
		
		pInventory->placeStatus = 3;
		pInventory->dealedLot += Lot;
		pInventory->order->addDetail(new OrderDetail(pInventory->order->getId(),Lot,Price));
		delInventory(pInventory);
	}
	LeaveCriticalSection(&cs);
}

void otherDealer::start()
{
	TRACE_LOG("启动 otherDealer.....");
    if(started) {
	    TRACE_LOG("otherDealer already started.");
	    return;
	}
	if (startRedis())
	{
		TRACE_LOG("otherDealer Redis服务器连接成功");
	}
	else
	{
		TRACE_LOG("otherDealer Redis服务器连接失败");
		exit(1);
	}
    started = true;	
	_beginthread(receiveOrderReport, 0, (void *)this);
	TRACE_LOG("otherDealer started _beginthread receiveOrderReport");
	return;
}

void otherDealer::stop()
{

}

void otherDealer::placeOrder(Order *order)
{
	EnterCriticalSection(&cs);
	int ref;
	TRACE_LOG("otherDealer::placeOrder, orderid:%s.",order->getId().c_str());
	if(!started) 
	{
		TRACE_LOG("otherDealer::placeOrder failed. otherDealer not started.");
		order->setRejected();
		return;
	}
	
	otherDealerInventory * pInventory = addInventory(order);
	if(pInventory == NULL)
	{//orderID与正在处理中的重复，要拒绝这次下单请求
		LeaveCriticalSection(&cs);
		TRACE_LOG("otherDealer::placeOrder failed. orderID is duplicated.");
		return;
	}
	//分配localRef
	ref = getCurOrderRef();
	pInventory->localRef = ref;
	strcpy(pInventory->sysID, order->getId().c_str());

	//组织消息
	//PUBLISH Order-MiscEX 10:10:10_128,018020371,BCK,Buy,CloseToday,1500,6
	char szplaceOrder[256]; memset(szplaceOrder, 0, sizeof(szplaceOrder));
	char szprice[32]; memset(szprice, 0, sizeof(szprice));
	char szQty[32]; memset(szQty, 0, sizeof(szQty));
	char szRef[16]; memset(szRef, 0, sizeof(szRef));
	
    strcat_s(szplaceOrder, 256, order->getId().c_str());
	strcat_s(szplaceOrder, 256, ",");
	strcat_s(szplaceOrder, 256, BoHaiInvestorId);
	strcat_s(szplaceOrder, 256, ",");
	strcat_s(szplaceOrder, 256, order->getContract().c_str());
	if (order->getBuySell() == BUY) 
		strcat_s(szplaceOrder, 256, ",Buy,");
	else 
		strcat_s(szplaceOrder, 256, ",Sell,");
	if (order->getKaiPing() == KAI_CANG) 
		strcat_s(szplaceOrder, 256, "Open,");
	else if(order->getKaiPing() == PING_CANG)
		strcat_s(szplaceOrder, 256, "CloseYesterday,");
	else 
		strcat_s(szplaceOrder, 256, "CloseToday,");

	sprintf_s(szprice, 32, "%0.2f", order->getPrice());
	strcat_s(szplaceOrder, 256, szprice);
	strcat_s(szplaceOrder, 256, ",");
	sprintf_s(szQty, 32, "%d", order->getLot());
	strcat_s(szplaceOrder, 256, szQty);

	TRACE_LOG("otherDealer Send placeOrder : %s", szplaceOrder);
	//redisCommand(c,"PUBLISH Order-MiscEX %s",szplaceOrder);
	//TODO: write command to file
	ofstream file("Redis\\Order.csv");
    if(file.fail()) {
		TRACE_LOG("open Order.txt fail");
		return;
	}
	file << szplaceOrder;
	LeaveCriticalSection(&cs);
}

void otherDealer::cancelOrder(Order *order)
{
	//TODO:
}

int otherDealer::startTcp()
{
	return 0;
}

int otherDealer::closeTcp()
{
	return 0;
}

otherDealerInventory * otherDealer::addInventory(Order *order)
{
	otherDealerInventory * ret = NULL;
	string key = order->getId();
	if(dealerInventoryMap.count(key)!=0)
	{
		return ret;
	}
	ret = new otherDealerInventory(order);
	dealerInventoryMap.insert(make_pair(key,ret));
	return ret;
}

bool otherDealer::delInventory(otherDealerInventory * pDealerInventory)
{
	dealerInventoryMap.erase(pDealerInventory->order->getId());
	delete pDealerInventory;
	return TRUE;
}

otherDealerInventory * otherDealer::getInventory(string key)
{
	otherDealerInventory * ret = NULL;
	map<string, otherDealerInventory*>::iterator iter = dealerInventoryMap.find(key);
	if(iter == dealerInventoryMap.end()) return ret;
	return iter->second;
}

otherDealerInventory * otherDealer::findInventorybyLocal(int localRef)
{
	otherDealerInventory * ret = NULL;
	map<string, otherDealerInventory*>::iterator iter = dealerInventoryMap.begin();
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

otherDealerInventory * otherDealer::findInventorybyRemote(int remoteRef)
{
	otherDealerInventory* ret = NULL;
	map<string, otherDealerInventory*>::iterator iter = dealerInventoryMap.begin();
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

otherDealerInventory * otherDealer::findInventorybySysID(const char * ID)
{
	otherDealerInventory* ret = NULL;
	map<string, otherDealerInventory*>::iterator iter = dealerInventoryMap.begin();
	while(iter != dealerInventoryMap.end())
	{
		if(strcmp(iter->second->sysID , ID) == 0)
		{
			ret = iter->second;
			break;
		}
		iter ++;
	}
	return ret;
}

int otherDealer::getCurOrderRef()
{
	return iNextOrderRef++;
}

//PUBLISH Order-MiscEX 10:10:10_128,018020371,Pause,Play,Stop
void otherDealer::redisStart()
{
	EnterCriticalSection(&cs);
	char msg[256]; memset(msg, 0, sizeof(msg));
	strcat_s(msg, 256, "NULL");
	strcat_s(msg, 256, ",");
	strcat_s(msg, 256, BoHaiInvestorId);
	strcat_s(msg, 256, ",Play");
	redisCommand(c,"PUBLISH Order-MiscEX %s",msg);
	LeaveCriticalSection(&cs);
}

void otherDealer::redisPause()
{
	EnterCriticalSection(&cs);
	char msg[256]; memset(msg, 0, sizeof(msg));
	strcat_s(msg, 256, "NULL");
	strcat_s(msg, 256, ",");
	strcat_s(msg, 256, BoHaiInvestorId);
	strcat_s(msg, 256, ",Pause");
	redisCommand(c,"PUBLISH Order-MiscEX %s",msg);
	LeaveCriticalSection(&cs);
}

void otherDealer::redisStop()
{
	EnterCriticalSection(&cs);
	char msg[256]; memset(msg, 0, sizeof(msg));
	strcat_s(msg, 256, "NULL");
	strcat_s(msg, 256, ",");
	strcat_s(msg, 256, BoHaiInvestorId);
	strcat_s(msg, 256, ",Stop");
	redisCommand(c,"PUBLISH Order-MiscEX %s",msg);
	LeaveCriticalSection(&cs);
}