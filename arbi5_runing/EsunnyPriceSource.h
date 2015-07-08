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
	//������������
	virtual void __cdecl OnQuote(TEsQuoteRspField& rsp);
	//�����������
	virtual void __cdecl OnCmbQuote(TEsCmbQuoteRspField& rsp){}

    // ��ȡ���鴦��ʵ��������״̬
    int GetConnectState() const
    {
        return m_iConnState;
    }
	void Convert2Lowercase(char content[], int length);

private:
    int                 m_iConnState;
};

#endif
