// RohonDealer.h: interface for the RohonDealer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(ROHON_DEALER_H)
#define ROHON_DEALER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Dealer.h"
#include ".\rohon\RohonTraderClientApi.h"
#include <windows.h>
#include <map>
using namespace std;

class RohonDealerInventory
{
public:
	RohonDealerInventory(Order *pOrder);
	virtual ~RohonDealerInventory();

	Order *order;
	int localRef;
	string remoteRef; //OrderSysID
	int placeStatus; // 0 -- new order; 1 -- received by remote; 2 -- partial dealed; 3 -- all dealed; 4 -- canceled; 5 -- aborted.
	int dealedLot;
};

class CROHONTraderSpi;
class RohonDealer : public Dealer  
{
public:
	RohonDealer();
	virtual ~RohonDealer();
	
	void start();
	void stop();
	void queryAllInstrument();
	//Dealer接口的实现方法
	virtual void placeOrder(Order *order);
	virtual void cancelOrder(Order *order);
	void OnReturnRspOrderInsert(CRohonInputOrderField *pInputOrder);
	void OnReturnRspOrderAction(CRohonInputOrderActionField *pInputOrderAction);
	void OnReturnOrder(CRohonOrderField *pOrder);
	void OnTrade(CRohonTradeField *pTrade);
	void ReadIniConfig();

	HANDLE hLogin;	//登录时的同步等待事件

	CRohonTraderClientApi* pUserApi;
	CROHONTraderSpi* pUserSpi;

	bool started;
	

private:
	void handleOrderActionReport(RohonDealerInventory* pInventory, CRohonInputOrderActionField *pInputOrder);
	void handleOrderInsertReport(RohonDealerInventory* pInventory, CRohonInputOrderField *pInputOrder);
	void handleOrderStatusReport(RohonDealerInventory* pInventory, CRohonOrderField *pOrder);
	void handleTradeReport(RohonDealerInventory* pInventory, CRohonTradeField *pTrade);

	RohonDealerInventory* addInventory(Order *order);
	bool delInventory(RohonDealerInventory* pDealerInventory);
	RohonDealerInventory* getInventory(string key);
	RohonDealerInventory* findInventorybyLocal(int localRef);
	RohonDealerInventory* findInventorybyRemote(string remoteRef);

	CRITICAL_SECTION cs;
	map<string, RohonDealerInventory*> dealerInventoryMap;

};


class CROHONTraderSpi : public CRohonTraderClientSpi
{
public:
	CROHONTraderSpi();
	virtual ~CROHONTraderSpi();
	int CancelOrder(int orderRef, Order *order);
	int InsertOrder(int orderRef, Order *order);
	int QueryAllInstrument();
	int getCurOrderRef();
	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected();

	///登录请求响应
	virtual void OnRspUserLogin(CRohonRspUserLoginField *pRspUserLogin, CRohonRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///投资者结算结果确认响应
	virtual void OnRspSettlementInfoConfirm(CRohonSettlementInfoConfirmField *pSettlementInfoConfirm, CRohonRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询合约响应
	virtual void OnRspQryInstrument(CRohonInstrumentField *pInstrument, CRohonRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///报单录入请求响应
	virtual void OnRspOrderInsert(CRohonInputOrderField *pInputOrder, CRohonRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///报单操作请求响应
	virtual void OnRspOrderAction(CRohonInputOrderActionField *pInputOrderAction, CRohonRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///报单通知
	virtual void OnRtnOrder(CRohonOrderField *pOrder);

	///成交通知
	virtual void OnRtnTrade(CRohonTradeField *pTrade);

	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	virtual void OnFrontDisconnected(int nReason);

	///心跳超时警告。当长时间未收到报文时，该方法被调用。
	virtual void OnHeartBeatWarning(int nTimeLapse);


	RohonDealer* m_pDealer;
	
private:
	TRohonOrderSysIDType m_OrderSysID;
	// 是否收到成功的响应
	bool IsErrorRspInfo(CRohonRspInfoField *pRspInfo);
	// 是否我的报单回报
	bool IsMyOrder(CRohonOrderField *pOrder);
	// 会话参数
	TRohonFrontIDType	FRONT_ID;	//前置编号
	TRohonSessionIDType	SESSION_ID;	//会话编号
	int iNextOrderRef; //报单引用
	// 请求编号
	int iRequestID;
	
	
};


#endif