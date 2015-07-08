#pragma once
#include "pricesource.h"
#include "FixThread.h"
#include <windows.h>
#include <map>
using namespace std;


class PatsApiPriceSource : public PriceSource, public FixThread
{
public:
	PatsApiPriceSource(void);
	virtual ~PatsApiPriceSource(void);

	// 价格网关的启动入口,同步函数，若启动失败，记日志并直接退出程序
	virtual	void start();

	// 添加合约的监听
	virtual void add(string contract);
	
	// 价格网关的后台消息线程
	bool OnMsg(TRawMsg *msg, IMsg *remote_src);

private:
	void ReadIniConfig();
	string ConvertContract(string& patsapiName);
	string AssemblePatsApiName(const char* exchange, const char* contract, const char* datetime);
	void SendListenContractMsg(string& patsapiName);

	bool isStarted;
	bool isThreadStarted;
	// config
	char m_szUsername[16];
	char m_szPassword[16];
	int m_useSimEngine;
	// engine point
	IMsg* m_pPatsApiEngine;

	map<string,string> convertNameMap; // 合约的patsapi名称到arbi名称的转换
	map<string,string>	contractsRetryMap;  // 记录该合约连续几次监听价格失败

	HANDLE hLogin;	//登录时的同步等待事件
	bool isLogonOK;
	CRITICAL_SECTION cs;
};

