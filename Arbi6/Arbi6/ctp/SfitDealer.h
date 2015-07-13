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
	//Dealer�ӿڵ�ʵ�ַ���
	virtual void placeOrder(Order *order);
	virtual void cancelOrder(Order *order);
	void OnReturnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder);
	void OnReturnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction);
	void OnReturnOrder(CThostFtdcOrderField *pOrder);
	void OnTrade(CThostFtdcTradeField *pTrade);
	void ReadIniConfig();

	HANDLE hLogin;	//��¼ʱ��ͬ���ȴ��¼�

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
	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected();

	//�ͻ�����֤
	virtual void OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��¼������Ӧ
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///Ͷ���߽�����ȷ����Ӧ
	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///����¼��������Ӧ
	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��������������Ӧ
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����֪ͨ
	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder);

	///�ɽ�֪ͨ
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);

	//***begin ��ʱ���õĺ���*********
	///����Ӧ��
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	virtual void OnFrontDisconnected(int nReason);
	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	virtual void OnHeartBeatWarning(int nTimeLapse);
	//***end ��ʱ���õĺ���*********

	SfitDealer* m_pDealer;
	
private:
	TThostFtdcOrderSysIDType m_OrderSysID;
	// �Ƿ��յ��ɹ�����Ӧ
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);
	// �Ƿ��ҵı����ر�
	bool IsMyOrder(CThostFtdcOrderField *pOrder);
	// �Ự����
	TThostFtdcFrontIDType	FRONT_ID;	//ǰ�ñ��
	TThostFtdcSessionIDType	SESSION_ID;	//�Ự���
	int iNextOrderRef; //��������
	// ������
	int iRequestID;
	
	
};

#endif // !defined(AFX_SFITDEALER_H__CB51A25E_ACAA_4BD4_A2AA_BFA808926191__INCLUDED_)
