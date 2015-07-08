#pragma once
#include "dealer.h"
#include "FixThread.h"
#include "fixmsgdef.h"
#include <windows.h>
#include <map>
using namespace std;

class FixPatsDealerInventory
{
public:
	FixPatsDealerInventory(Order *pOrder);
	virtual ~FixPatsDealerInventory();

	Order *order;
	string tag11;  // Fix's CIOrdID
	string tag37; //PATS system's OrderID
	// -1 -- prepare, order is now in FixPatsDealer; 
	// 0 -- new, order is working at exchange; 
	// 1 -- partial dealed; 
	// 2 -- all dealed; 
	// 4 -- canceled, Any remaining volume is cancelled; 
	// 6 -- pending cancel.
	// 8 -- rejected, the order was rejected.
	int status; 
};

class FixPatsDealer :
	public Dealer, public FixThread
{
public:
	FixPatsDealer(void);
	virtual ~FixPatsDealer(void);

	// Fix交易网关的启动入口,同步函数，若启动失败，记日志并直接退出程序
	void start();

	//Dealer接口的实现方法
	virtual void placeOrder(Order *order);
	virtual void cancelOrder(Order *order);

	// Fix交易网关的后台消息线程
	bool OnMsg(TRawMsg *msg, IMsg *remote_src);

private:
	void ReadIniConfig();
	string assembleID(string orderguid);
	void assembleNOSMsg(string tag11, Order *order, TNewOrderSingleMsg &msg);
	void assembleOrderCancelMsg(string tag11, string tag37, Order *order, TOrderCancelMsg &msg);
	void handleStatus(TOrderRespMsg *pResp, int status);

	FixPatsDealerInventory* addInventory(Order *order);
	bool delInventory(FixPatsDealerInventory* pDealerInventory);
	FixPatsDealerInventory* getInventory(string key);
	FixPatsDealerInventory* findInventorybyTag11(string tag11);

	int getInvokeID();
	static int s_InvokeID;

	bool isStarted;
	bool isThreadStarted;
	// config
	int m_HeartBeatInterval;
	char m_szUsername[16];
	char m_szPassword[16];
	int m_useSimEngine;
	// engine point
	IMsg* m_pFixEngine;

	map<string, FixPatsDealerInventory*> dealerInventoryMap;
	
	// 一般情况下，dealer不需要进行登录操作，因为PriceSource已经登录
	// 这里保留登录动作，由FixEngine负责识别重复登陆，并在重复登陆时直接回送登录成功消息
	HANDLE hLogin;	//登录时的同步等待事件
	bool isLogonOK;
	CRITICAL_SECTION cs;
};

