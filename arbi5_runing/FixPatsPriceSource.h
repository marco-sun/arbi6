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

	// Fix�۸����ص��������,ͬ��������������ʧ�ܣ�����־��ֱ���˳�����
	virtual	void start();

	// ��Ӻ�Լ�ļ���
	virtual void add(string contract);
	
	// Fix�۸����صĺ�̨��Ϣ�߳�
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
	map<string,string>	contractsRetryMap;  // ��¼�ú�Լ�������μ����۸�ʧ��
	double ask;
	double bid;

	HANDLE hLogin;	//��¼ʱ��ͬ���ȴ��¼�
	bool isLogonOK;
	CRITICAL_SECTION cs;
};

