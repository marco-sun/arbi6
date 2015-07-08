#if !defined(AFX_ESUNNYDEALER_H)
#define AFX_ESUNNYDEALER_H

#pragma once

#include "Dealer.h"
#include ".\esunny\EsunnyApi.h"
#include <windows.h>
#include <map>
using namespace std;

enum EsunnyTradeConnectState
{
    STATE_DISCONNECTED  = 1,    //未连接
    STATE_CONNECTED     = 2,    //已连接
    STATE_LOGIN         = 3     //已登录
};

enum EsunnyTradeOrderState
{
    ORDER_INVALID       = 0,    //无效单
    ORDER_NEW           = 1,    //新单
    ORDER_IN_QUEUE      = 2,    //已排队
    ORDER_ALL_MATCH     = 3,    //完全成交
    ORDER_IN_CANCEL     = 4,    //撤单请求
    ORDER_CANCELED      = 5,    //已撤单
    ORDER_CHECK			= 6,	//埋单
    ORDER_CHECK_DELETE	= 7,    //埋单删除
    ORDER_ALREADY_SENT  = 8,    //已发送
    ORDER_TO_CANCEL		= 9,    //待撤
    ORDER_PART_MATCH    = 10,   //部分成交
    ORDER_NON_TRADING_HOURS     = 11,  //非交易时间
    ORDER_INSUFFICIENT_FUNDS    = 12,  //资金不足
    ORDER_TRADE_CLOSED          = 13,  //交易关闭
    ORDER_INSTRUCT_FAILED       = 14,  //指令失败
    ORDER_WITHDRAWALS_FAILED    = 15,  //撤单失败
    ORDER_SYSTEM_WITHDRAWALS	= 16,  //系统撤单
    ORDER_TRIGGER_ORDER	        = 17,  //触发单
    ORDER_TRIGGER_ORDER_DELETE  = 18,  //触发单删除
    ORDER_SUSPEND	            = 19,  //挂起
    ORDER_ACTIVATE	            = 20,  //激活
    ORDER_ACCEPTED	            = 21,  //已受理
    ORDER_AUTO		            = 22,  //自动单
    ORDER_AUTO_DELTE	        = 23   //自动单删除
};

class EsunnyDealerInventory
{
public:
	EsunnyDealerInventory(Order *pOrder);
	virtual ~EsunnyDealerInventory();

	Order *order;
	int localRef;  // 请求编号-place order时本方生成
	string remoteRef; //委托号 orderno - 易盛系统接受报单后生成
	string systemNo; // 系统号-交易所生成 
	int placeStatus; // 0 -- new order; 1 -- received by remote; 2 -- partial dealed; 3 -- all dealed; 4 -- canceled; 5 -- aborted.
	int dealedLot;
};

class IEsunnyTradeSpi;
class EsunnyDealerSpi;
class EsunnyDealer : public Dealer  
{
public:
	static EsunnyDealer* getInstance();
	static const char* GetOrderStateDesc(unsigned int iState);

	virtual ~EsunnyDealer(void);
	void start();
	void stop();
	//Dealer接口的实现方法
	virtual void placeOrder(Order *order);
	virtual void cancelOrder(Order *order);

	
	void OnReturnQryOrder(TEsOrderInsertRspField* pRsp);
	void OnReturnOrder(TEsOrderInsertRspField& rsp);
	void OnTrade(TEsMatchRspField& rsp);
	void ReadIniConfig();

	HANDLE hLogin;	//登录时的同步等待事件

	IEsunnyTradeApi* pUserApi;
	EsunnyDealerSpi* pUserSpi;

	bool started;
	

private:
	
	EsunnyDealer(void);
	void handleOrderStatusReport(EsunnyDealerInventory* pInventory, TEsOrderInsertRspField& rsp);
	void handleTradeReport(EsunnyDealerInventory* pInventory, TEsMatchRspField& rsp);

	EsunnyDealerInventory* addInventory(Order *order);
	bool delInventory(EsunnyDealerInventory* pDealerInventory);
	EsunnyDealerInventory* getInventory(string key);
	EsunnyDealerInventory* findInventorybyLocal(int localRef);
	EsunnyDealerInventory* findInventorybyRemote(string remoteRef);
	EsunnyDealerInventory* findInventorybySys(string sysNo);

	CRITICAL_SECTION cs;
	map<string, EsunnyDealerInventory*> dealerInventoryMap;
};

class EsunnyDealerSpi : public IEsunnyTradeSpi
{
public:
    EsunnyDealerSpi() 
        : m_iConnState(STATE_DISCONNECTED),
        m_iRequestID(0)
	{		
	}
public:
	int InsertOrder(int orderRef, char exchangeCode, Order *order);
	int CancelOrder(char exchangeCode, string orderNo, string sysNo, Order *order);
	void Convert2Uppercase(char content[], int length);

public:
	virtual void __cdecl OnOpen();
	
	virtual void __cdecl OnClose();

	virtual void __cdecl OnLogin(TEsRspField& rsp);
	
	// 修改密码应答
	virtual void __cdecl OnSetPass(TEsRspField& rsp){}

	//资金查询应答
	virtual void __cdecl OnQryMoney(TEsMoneyRspField& rsp){}

	//资金变化应答
	virtual void __cdecl OnRtnMoney(TEsMoneyRspField& rsp){}

	//委托查询应答
	virtual void __cdecl OnQryOrder(TEsOrderInsertRspField* pRsp, TEsIsLastType islast);

	//委托变化应答
	virtual void __cdecl OnRtnOrder(TEsOrderInsertRspField& rsp);

	//成交查询应答
	virtual void __cdecl OnQryMatch(TEsMatchRspField* rsp, TEsIsLastType islast){}

	//成交变化应答
	virtual void __cdecl OnRtnMatch(TEsMatchRspField& rsp);

	//持仓查询应答
	virtual void __cdecl OnQryHold(TEsHoldRspField* rsp, TEsIsLastType islast){}

	//持仓变化应答
	virtual void __cdecl OnRtnHold(TEsHoldRspField& rsp){}

	//市场状态查询应答
	virtual void __cdecl OnQryExchangeState(TEsExchangeStateRspField* rsp, TEsIsLastType islast){}

	//市场状态变化应答
	virtual void __cdecl OnRtnExchangeState(TEsExchangeStateRspField& rsp){}

	//商品查询应答
	virtual void __cdecl OnQryCommodity(TEsCommodityRspField* rsp, TEsIsLastType islast){}

	//合约查询应答
	virtual void __cdecl OnQryContract(TEsContractRspField* rsp, TEsIsLastType islast){}

	//组合合约查询应答
	virtual void __cdecl OnQryCmbContract(TEsCmbContractRspField* rsp, TEsIsLastType islast){}
	
	//交易员下属客户信息查询应答
	virtual void __cdecl OnQryClient(TEsClientInfoField* rsp, TEsIsLastType islast){}

	//转账客户查询应答
	virtual void __cdecl OnQryBankClient(TEsBankClientInfoField* rsp, TEsIsLastType islast){}

	//转账流水查询应答
	virtual void __cdecl OnQryBankCash(TEsBankCashRspField* rsp, TEsIsLastType islast){}

	//转账流水变化应答
	virtual void __cdecl OnRtnBankCash(TEsBankCashRspField& rsp){}

    // 获取当前连接类的连接状态
    int GetConnectState() const
    {
        return m_iConnState;
    }

    // 获取本客户端的报单请求ID
    int GenerateReqID()
    {
        return m_iRequestID++;
    }

	EsunnyDealer* m_pDealer;

private:
    int m_iRequestID;
    int m_iConnState;
};

#endif
