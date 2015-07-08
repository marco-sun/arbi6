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
	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected();
	
	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param nReason 错误原因
	///        0x1001 网络读失败
	///        0x1002 网络写失败
	///        0x2001 接收心跳超时
	///        0x2002 发送心跳失败
	///        0x2003 收到错误报文
	virtual void OnFrontDisconnected(int nReason);
	
	///心跳超时警告。当长时间未收到报文时，该方法被调用。
	///@param nTimeLapse 距离上次接收报文的时间
	virtual void OnHeartBeatWarning(int nTimeLapse);
	
	
	///登录请求响应
	virtual void OnRspUserLogin(CRohonRspUserLoginField *pRspUserLogin, CRohonRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
	
	///登出请求响应
	virtual void OnRspUserLogout(CRohonUserLogoutField *pUserLogout, CRohonRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///错误应答
	virtual void OnRspError(CRohonRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///订阅行情应答
	virtual void OnRspSubMarketData(CRohonSpecificInstrumentField *pSpecificInstrument, CRohonRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///取消订阅行情应答
	virtual void OnRspUnSubMarketData(CRohonSpecificInstrumentField *pSpecificInstrument, CRohonRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///深度行情通知
	virtual void OnRtnDepthMarketData(CRohonDepthMarketDataField *pDepthMarketData);
	
	//深度行情更新通知
	virtual void OnRtnUpdateDepthMarketData(CRohonInstrumentUpdateMarketData* pDepthMarketUpdateData);


private:
	void Login();
	bool IsErrorRspInfo(CRohonRspInfoField *pRspInfo);
	int AllocRquestID() { return ++m_nRquestID; }

private:
	int						m_nRquestID;
};


#endif