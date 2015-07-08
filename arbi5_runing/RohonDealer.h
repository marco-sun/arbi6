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
	//Dealer�ӿڵ�ʵ�ַ���
	virtual void placeOrder(Order *order);
	virtual void cancelOrder(Order *order);
	void OnReturnRspOrderInsert(CRohonInputOrderField *pInputOrder);
	void OnReturnRspOrderAction(CRohonInputOrderActionField *pInputOrderAction);
	void OnReturnOrder(CRohonOrderField *pOrder);
	void OnTrade(CRohonTradeField *pTrade);
	void ReadIniConfig();

	HANDLE hLogin;	//��¼ʱ��ͬ���ȴ��¼�

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
	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected();

	///��¼������Ӧ
	virtual void OnRspUserLogin(CRohonRspUserLoginField *pRspUserLogin, CRohonRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///Ͷ���߽�����ȷ����Ӧ
	virtual void OnRspSettlementInfoConfirm(CRohonSettlementInfoConfirmField *pSettlementInfoConfirm, CRohonRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ��Լ��Ӧ
	virtual void OnRspQryInstrument(CRohonInstrumentField *pInstrument, CRohonRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///����¼��������Ӧ
	virtual void OnRspOrderInsert(CRohonInputOrderField *pInputOrder, CRohonRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��������������Ӧ
	virtual void OnRspOrderAction(CRohonInputOrderActionField *pInputOrderAction, CRohonRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����֪ͨ
	virtual void OnRtnOrder(CRohonOrderField *pOrder);

	///�ɽ�֪ͨ
	virtual void OnRtnTrade(CRohonTradeField *pTrade);

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	virtual void OnFrontDisconnected(int nReason);

	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	virtual void OnHeartBeatWarning(int nTimeLapse);


	RohonDealer* m_pDealer;
	
private:
	TRohonOrderSysIDType m_OrderSysID;
	// �Ƿ��յ��ɹ�����Ӧ
	bool IsErrorRspInfo(CRohonRspInfoField *pRspInfo);
	// �Ƿ��ҵı����ر�
	bool IsMyOrder(CRohonOrderField *pOrder);
	// �Ự����
	TRohonFrontIDType	FRONT_ID;	//ǰ�ñ��
	TRohonSessionIDType	SESSION_ID;	//�Ự���
	int iNextOrderRef; //��������
	// ������
	int iRequestID;
	
	
};


#endif