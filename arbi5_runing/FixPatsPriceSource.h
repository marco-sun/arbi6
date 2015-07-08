#pragma once
#include "pricesource.h"
#include "FixThread.h"
#include <windows.h>
#include <map>
using namespace std;

typedef struct _PriceNote
{
	double ask;
	double bid;
	double lastPrice;
} PriceNote;

class FixPatsPriceSource :
	public PriceSource, public FixThread
{
public:
	FixPatsPriceSource(void);
	virtual ~FixPatsPriceSource(void);

	// Fix价格网关的启动入口,同步函数，若启动失败，记日志并直接退出程序
	virtual	void start();

	// 添加合约的监听
	virtual void add(string contract);
	
	// Fix价格网关的后台消息线程
	bool OnMsg(TRawMsg *msg, IMsg *remote_src);

private:
	void ReadIniConfig();
	string assembleID(string contract,string pricetype);
	string ID2Contract(const char* id);
	string ID2PriceType(const char* id);
	void SendListenContractMsg(string& contract);

	bool isStarted;
	bool isThreadStarted;
	// config
	int m_HeartBeatInterval;
	char m_szUsername[16];
	char m_szPassword[16];
	int m_useSimEngine;
	// engine point
	IMsg* m_pFixEngine;

	map<string,PriceNote> contractsMap;
	map<string,string>	contractsRetryMap;  // 记录该合约连续几次监听价格失败
	double ask;
	double bid;

	HANDLE hLogin;	//登录时的同步等待事件
	bool isLogonOK;
	CRITICAL_SECTION cs;
};

