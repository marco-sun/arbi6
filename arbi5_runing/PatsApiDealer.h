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
	// �������ص��������,ͬ��������������ʧ�ܣ�����־��ֱ���˳�����
	void start();

	//Dealer�ӿڵ�ʵ�ַ���
	virtual void placeOrder(Order *order);
	virtual void cancelOrder(Order *order);

	// �������صĺ�̨��Ϣ�߳�
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
	
	// һ������£�dealer����Ҫ���е�¼��������ΪPriceSource�Ѿ���¼
	// ���ﱣ����¼��������Engine����ʶ���ظ���½�������ظ���½ʱֱ�ӻ��͵�¼�ɹ���Ϣ
	HANDLE hLogin;	//��¼ʱ��ͬ���ȴ��¼�
	bool isLogonOK;
	CRITICAL_SECTION cs;
};

