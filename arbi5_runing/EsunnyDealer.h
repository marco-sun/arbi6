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
    STATE_DISCONNECTED  = 1,    //δ����
    STATE_CONNECTED     = 2,    //������
    STATE_LOGIN         = 3     //�ѵ�¼
};

enum EsunnyTradeOrderState
{
    ORDER_INVALID       = 0,    //��Ч��
    ORDER_NEW           = 1,    //�µ�
    ORDER_IN_QUEUE      = 2,    //���Ŷ�
    ORDER_ALL_MATCH     = 3,    //��ȫ�ɽ�
    ORDER_IN_CANCEL     = 4,    //��������
    ORDER_CANCELED      = 5,    //�ѳ���
    ORDER_CHECK			= 6,	//��
    ORDER_CHECK_DELETE	= 7,    //��ɾ��
    ORDER_ALREADY_SENT  = 8,    //�ѷ���
    ORDER_TO_CANCEL		= 9,    //����
    ORDER_PART_MATCH    = 10,   //���ֳɽ�
    ORDER_NON_TRADING_HOURS     = 11,  //�ǽ���ʱ��
    ORDER_INSUFFICIENT_FUNDS    = 12,  //�ʽ���
    ORDER_TRADE_CLOSED          = 13,  //���׹ر�
    ORDER_INSTRUCT_FAILED       = 14,  //ָ��ʧ��
    ORDER_WITHDRAWALS_FAILED    = 15,  //����ʧ��
    ORDER_SYSTEM_WITHDRAWALS	= 16,  //ϵͳ����
    ORDER_TRIGGER_ORDER	        = 17,  //������
    ORDER_TRIGGER_ORDER_DELETE  = 18,  //������ɾ��
    ORDER_SUSPEND	            = 19,  //����
    ORDER_ACTIVATE	            = 20,  //����
    ORDER_ACCEPTED	            = 21,  //������
    ORDER_AUTO		            = 22,  //�Զ���
    ORDER_AUTO_DELTE	        = 23   //�Զ���ɾ��
};

class EsunnyDealerInventory
{
public:
	EsunnyDealerInventory(Order *pOrder);
	virtual ~EsunnyDealerInventory();

	Order *order;
	int localRef;  // ������-place orderʱ��������
	string remoteRef; //ί�к� orderno - ��ʢϵͳ���ܱ���������
	string systemNo; // ϵͳ��-���������� 
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
	//Dealer�ӿڵ�ʵ�ַ���
	virtual void placeOrder(Order *order);
	virtual void cancelOrder(Order *order);

	
	void OnReturnQryOrder(TEsOrderInsertRspField* pRsp);
	void OnReturnOrder(TEsOrderInsertRspField& rsp);
	void OnTrade(TEsMatchRspField& rsp);
	void ReadIniConfig();

	HANDLE hLogin;	//��¼ʱ��ͬ���ȴ��¼�

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
	
	// �޸�����Ӧ��
	virtual void __cdecl OnSetPass(TEsRspField& rsp){}

	//�ʽ��ѯӦ��
	virtual void __cdecl OnQryMoney(TEsMoneyRspField& rsp){}

	//�ʽ�仯Ӧ��
	virtual void __cdecl OnRtnMoney(TEsMoneyRspField& rsp){}

	//ί�в�ѯӦ��
	virtual void __cdecl OnQryOrder(TEsOrderInsertRspField* pRsp, TEsIsLastType islast);

	//ί�б仯Ӧ��
	virtual void __cdecl OnRtnOrder(TEsOrderInsertRspField& rsp);

	//�ɽ���ѯӦ��
	virtual void __cdecl OnQryMatch(TEsMatchRspField* rsp, TEsIsLastType islast){}

	//�ɽ��仯Ӧ��
	virtual void __cdecl OnRtnMatch(TEsMatchRspField& rsp);

	//�ֲֲ�ѯӦ��
	virtual void __cdecl OnQryHold(TEsHoldRspField* rsp, TEsIsLastType islast){}

	//�ֱֲ仯Ӧ��
	virtual void __cdecl OnRtnHold(TEsHoldRspField& rsp){}

	//�г�״̬��ѯӦ��
	virtual void __cdecl OnQryExchangeState(TEsExchangeStateRspField* rsp, TEsIsLastType islast){}

	//�г�״̬�仯Ӧ��
	virtual void __cdecl OnRtnExchangeState(TEsExchangeStateRspField& rsp){}

	//��Ʒ��ѯӦ��
	virtual void __cdecl OnQryCommodity(TEsCommodityRspField* rsp, TEsIsLastType islast){}

	//��Լ��ѯӦ��
	virtual void __cdecl OnQryContract(TEsContractRspField* rsp, TEsIsLastType islast){}

	//��Ϻ�Լ��ѯӦ��
	virtual void __cdecl OnQryCmbContract(TEsCmbContractRspField* rsp, TEsIsLastType islast){}
	
	//����Ա�����ͻ���Ϣ��ѯӦ��
	virtual void __cdecl OnQryClient(TEsClientInfoField* rsp, TEsIsLastType islast){}

	//ת�˿ͻ���ѯӦ��
	virtual void __cdecl OnQryBankClient(TEsBankClientInfoField* rsp, TEsIsLastType islast){}

	//ת����ˮ��ѯӦ��
	virtual void __cdecl OnQryBankCash(TEsBankCashRspField* rsp, TEsIsLastType islast){}

	//ת����ˮ�仯Ӧ��
	virtual void __cdecl OnRtnBankCash(TEsBankCashRspField& rsp){}

    // ��ȡ��ǰ�����������״̬
    int GetConnectState() const
    {
        return m_iConnState;
    }

    // ��ȡ���ͻ��˵ı�������ID
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
