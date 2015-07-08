#if !defined(AFX_ESUNNYPRICESOURCE_H)
#define AFX_ESUNNYPRICESOURCE_H

#pragma once

#include "pricesource.h"
#include ".\esunny\EsunnyApi.h"

class EsunnyPriceSource :
	public PriceSource
{
public:
	static EsunnyPriceSource* getInstance();
	virtual ~EsunnyPriceSource(void);
	virtual	void start(void);
	virtual void add(string contract);
	void asyncInform(Pricing pricing);
	static void threadPrice( void* pParams ) ;
private:
	EsunnyPriceSource(void);
	void Convert2Uppercase(char content[], int length);
	void ReadIniConfig();
	bool started;
};

class EsunnyQuoteSpi : public IEsunnyQuoteSpi
{
	public:
	EsunnyQuoteSpi() : m_iConnState(0)
	{		
	}
	
	virtual void __cdecl OnOpen();
	
	virtual void __cdecl OnClose();
	//单腿行情数据
	virtual void __cdecl OnQuote(TEsQuoteRspField& rsp);
	//组合行情数据
	virtual void __cdecl OnCmbQuote(TEsCmbQuoteRspField& rsp){}

    // 获取行情处理实例的连接状态
    int GetConnectState() const
    {
        return m_iConnState;
    }
	void Convert2Lowercase(char content[], int length);

private:
    int                 m_iConnState;
};

#endif
