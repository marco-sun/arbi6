// RohonPriceSource.h: interface for the RohonPriceSource class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(ROHON_PRICE_SOURCE_H)
#define ROHON_PRICE_SOURCE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include ".\rohon\RohonMdApi.h"
#include "PriceSource.h"
#include <set>
using namespace std;

class RohonPriceSource : public PriceSource  
{
public:
	RohonPriceSource();
	virtual ~RohonPriceSource();
	virtual	void start(void);
	virtual void add(string contract);
	void asyncAutoSubscribe();
	static void threadPrice( void* pParams ) ;
private:
	void subscribeBatch();
	void ReadIniConfig();
	bool started;
	set<string> m_contracts;

};

class CRohonMD : public CRohonMdSpi
{
public:
	CRohonMD();
	~CRohonMD();

	RohonPriceSource* ps;

public:
	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected();
	
	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
	virtual void OnFrontDisconnected(int nReason);
	
	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
	virtual void OnHeartBeatWarning(int nTimeLapse);
	
	
	///��¼������Ӧ
	virtual void OnRspUserLogin(CRohonRspUserLoginField *pRspUserLogin, CRohonRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
	
	///�ǳ�������Ӧ
	virtual void OnRspUserLogout(CRohonUserLogoutField *pUserLogout, CRohonRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///����Ӧ��
	virtual void OnRspError(CRohonRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///��������Ӧ��
	virtual void OnRspSubMarketData(CRohonSpecificInstrumentField *pSpecificInstrument, CRohonRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///ȡ����������Ӧ��
	virtual void OnRspUnSubMarketData(CRohonSpecificInstrumentField *pSpecificInstrument, CRohonRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///�������֪ͨ
	virtual void OnRtnDepthMarketData(CRohonDepthMarketDataField *pDepthMarketData);
	
	//����������֪ͨ
	virtual void OnRtnUpdateDepthMarketData(CRohonInstrumentUpdateMarketData* pDepthMarketUpdateData);


private:
	void Login();
	bool IsErrorRspInfo(CRohonRspInfoField *pRspInfo);
	int AllocRquestID() { return ++m_nRquestID; }

private:
	int						m_nRquestID;
};


#endif