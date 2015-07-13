// SfitPriceSource.h: interface for the SfitPriceSource class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SFITPRICESOURCE_H__B7AA0948_D2D0_4D4A_90AE_D25B8617F7FC__INCLUDED_)
#define AFX_SFITPRICESOURCE_H__B7AA0948_D2D0_4D4A_90AE_D25B8617F7FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ThostFtdcMdApi.h"
#include "../arbi/PriceSource.h"
#include <set>
using namespace std;

typedef struct _TAsyncPriceReporterParam
{
	PriceSource* ps;
	Pricing pricing;
} TAsyncPriceReporterParam;

class SfitPriceSource : public PriceSource  
{
public:
	SfitPriceSource();
	virtual ~SfitPriceSource();
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

class CSFITMdSpi : public CThostFtdcMdSpi  
{
public:
	CSFITMdSpi();
	virtual ~CSFITMdSpi();

	SfitPriceSource* ps;
	virtual void OnFrontConnected();
	virtual void OnFrontDisconnected(int nReason);
	virtual void OnHeartBeatWarning(int nTimeLapse);
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///登出请求响应
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///错误应答
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///订阅行情应答
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///取消订阅行情应答
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///深度行情通知
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

private:
	int iRequestID;
	void ReqUserLogin();
	void SubscribeMarketData();
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);

};

#endif // !defined(AFX_SFITPRICESOURCE_H__B7AA0948_D2D0_4D4A_90AE_D25B8617F7FC__INCLUDED_)
