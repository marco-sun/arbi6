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

	// �۸����ص��������,ͬ��������������ʧ�ܣ�����־��ֱ���˳�����
	virtual	void start();

	// ��Ӻ�Լ�ļ���
	virtual void add(string contract);
	
	// �۸����صĺ�̨��Ϣ�߳�
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

	map<string,string> convertNameMap; // ��Լ��patsapi���Ƶ�arbi���Ƶ�ת��
	map<string,string>	contractsRetryMap;  // ��¼�ú�Լ�������μ����۸�ʧ��

	HANDLE hLogin;	//��¼ʱ��ͬ���ȴ��¼�
	bool isLogonOK;
	CRITICAL_SECTION cs;
};

