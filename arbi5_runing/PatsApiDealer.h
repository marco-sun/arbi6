#pragma once
#include "dealer.h"
#include "FixThread.h"
#include "patsapimsgdef.h"
#include <windows.h>
#include <map>
using namespace std;
class PatsApiDealer : public Dealer, public FixThread
{
public:
	PatsApiDealer(void);
	virtual ~PatsApiDealer(void);
	// 交易网关的启动入口,同步函数，若启动失败，记日志并直接退出程序
	void start();

	//Dealer接口的实现方法
	virtual void placeOrder(Order *order);
	virtual void cancelOrder(Order *order);

	// 交易网关的后台消息线程
	bool OnMsg(TRawMsg *msg, IMsg *remote_src);

private:
	void ReadIniConfig();
	Order* getOrder(string orderid);
	bool isStarted;
	bool isThreadStarted;
	// config
	char m_szTraderAccount[21];
	int m_useSimEngine;
	// engine point
	IMsg* m_pEngine;

	map<string, Order*> dealerInventoryMap;
	
	// 一般情况下，dealer不需要进行登录操作，因为PriceSource已经登录
	// 这里保留登录动作，由Engine负责识别重复登陆，并在重复登陆时直接回送登录成功消息
	HANDLE hLogin;	//登录时的同步等待事件
	bool isLogonOK;
	CRITICAL_SECTION cs;
};

