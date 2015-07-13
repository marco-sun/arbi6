// SfitDealer.h: interface for the SfitDealer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SFITDEALER_H__CB51A25E_ACAA_4BD4_A2AA_BFA808926191__INCLUDED_)
#define AFX_SFITDEALER_H__CB51A25E_ACAA_4BD4_A2AA_BFA808926191__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../arbi/Dealer.h"
#include "ThostFtdcTraderApi.h"
#include <windows.h>
#include <map>
using namespace std;

class SfitDealerInventory
{
public:
	SfitDealerInventory(Order *pOrder);
	virtual ~SfitDealerInventory();

	Order *order;
	int localRef;
	string remoteRef; //OrderSysID
	int placeStatus; // 0 -- new order; 1 -- received by remote; 2 -- partial dealed; 3 -- all dealed; 4 -- canceled; 5 -- aborted.
	int dealedLot;
};

class CSFITTraderSpi;
class SfitDealer : public Dealer  
{
public:
	SfitDealer();
	virtual ~SfitDealer();
	
	void start();
	void stop();
	//Dealer接口的实现方法
	virtual void placeOrder(Order *order);
	virtual void cancelOrder(Order *order);
	void OnReturnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder);
	void OnReturnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction);
	void OnReturnOrder(CThostFtdcOrderField *pOrder);
	void OnTrade(CThostFtdcTradeField *pTrade);
	void ReadIniConfig();

	HANDLE hLogin;	//登录时的同步等待事件

	CThostFtdcTraderApi* pUserApi;
	CSFITTraderSpi* pUserSpi;

	bool started;
	

private:
	int readCommodityProp(const char* szCommodityName);
	void handleOrderActionReport(SfitDealerInventory* pInventory, CThostFtdcInputOrderActionField *pInputOrder);
	void handleOrderInsertReport(SfitDealerInventory* pInventory, CThostFtdcInputOrderField *pInputOrder);
	void handleOrderStatusReport(SfitDealerInventory* pInventory, CThostFtdcOrderField *pOrder);
	void handleTradeReport(SfitDealerInventory* pInventory, CThostFtdcTradeField *pTrade);

	SfitDealerInventory* addInventory(Order *order);
	bool delInventory(SfitDealerInventory* pDealerInventory);
	SfitDealerInventory* getInventory(string key);
	SfitDealerInventory* findInventorybyLocal(int localRef);
	SfitDealerInventory* findInventorybyRemote(string remoteRef);

	CRITICAL_SECTION cs;
	map<string, SfitDealerInventory*> dealerInventoryMap;

};

class CSFITTraderSpi : public CThostFtdcTraderSpi
{
public:
	CSFITTraderSpi();
	virtual ~CSFITTraderSpi();
	int CancelOrder(int orderRef, Order *order);
	int InsertOrder(int orderRef, Order *order, int commodityProp);
	int getCurOrderRef();
	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected();

	//客户端认证
	virtual void OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///登录请求响应
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///投资者结算结果确认响应
	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///报单录入请求响应
	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///报单操作请求响应
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///报单通知
	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder);

	///成交通知
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);

	//***begin 暂时不用的函数*********
	///错误应答
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	virtual void OnFrontDisconnected(int nReason);
	///心跳超时警告。当长时间未收到报文时，该方法被调用。
	virtual void OnHeartBeatWarning(int nTimeLapse);
	//***end 暂时不用的函数*********

	SfitDealer* m_pDealer;
	
private:
	TThostFtdcOrderSysIDType m_OrderSysID;
	// 是否收到成功的响应
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);
	// 是否我的报单回报
	bool IsMyOrder(CThostFtdcOrderField *pOrder);
	// 会话参数
	TThostFtdcFrontIDType	FRONT_ID;	//前置编号
	TThostFtdcSessionIDType	SESSION_ID;	//会话编号
	int iNextOrderRef; //报单引用
	// 请求编号
	int iRequestID;
	
	
};

#endif // !defined(AFX_SFITDEALER_H__CB51A25E_ACAA_4BD4_A2AA_BFA808926191__INCLUDED_)
