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

	// Fix�������ص��������,ͬ��������������ʧ�ܣ�����־��ֱ���˳�����
	void start();

	//Dealer�ӿڵ�ʵ�ַ���
	virtual void placeOrder(Order *order);
	virtual void cancelOrder(Order *order);

	// Fix�������صĺ�̨��Ϣ�߳�
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
	
	// һ������£�dealer����Ҫ���е�¼��������ΪPriceSource�Ѿ���¼
	// ���ﱣ����¼��������FixEngine����ʶ���ظ���½�������ظ���½ʱֱ�ӻ��͵�¼�ɹ���Ϣ
	HANDLE hLogin;	//��¼ʱ��ͬ���ȴ��¼�
	bool isLogonOK;
	CRITICAL_SECTION cs;
};

