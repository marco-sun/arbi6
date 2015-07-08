//#include "stdafx.h"
#include "EsunnyPriceSource.h"
#include "EsunnyDealer.h"
#include <iostream>
#include "LogStore.h"
#include <process.h>
#include <assert.h>
#include "Contract.h"

// ���ò���,���������ļ��ж���
static TEsAddressField ADDR;

static IEsunnyQuoteApi* pUserApi;

static EsunnyPriceSource* sp_esPriceSource = NULL;

EsunnyPriceSource* EsunnyPriceSource::getInstance()
{
	if(sp_esPriceSource==NULL) sp_esPriceSource = new EsunnyPriceSource();
	return sp_esPriceSource;
}

EsunnyPriceSource::EsunnyPriceSource(void)
{
	ReadIniConfig();
	started = false;
}


EsunnyPriceSource::~EsunnyPriceSource(void)
{
}

void EsunnyPriceSource::Convert2Uppercase(char content[], int length)
{
	for(int i=0;i<length;i++)
	{
		if(content[i] == 0x00) break;
		if(content[i]>='a' && content[i]<='z')
		{
			content[i] -= 0x20;
		}

	}
}

void EsunnyPriceSource::add(string contract)
{
	TRACE_LOG("Send subscribe");
	TEsQuoteReqField sub;
    memset(&sub, 0, sizeof(sub));
    sub.CmbType = 0;
    sub.SubQuote = 'S';
    sub.Exchange = Contract::get(contract)->getExchangeNumberForYiSheng();
    strncpy_s(sub.Contract1, sizeof(sub.Contract1), contract.c_str(), sizeof(sub.Contract1));
	Convert2Uppercase(sub.Contract1,sizeof(sub.Contract1));
	int iResult = pUserApi->SubQuote(sub);
	TRACE_LOG("Send subscribe for contract %s, and result %s.", sub.Contract1, ((iResult == 0) ? "�ɹ�" : "ʧ��") ) ;
}

void EsunnyPriceSource::start(void)
{
	if(started) {
        TRACE_LOG("EsunnyPriceSource already started.");
	    return;
	}
	// �ȳ�ʼ��EsunnyDealer������,�������Ӽ۸�
	
	EsunnyDealer* pDealer = EsunnyDealer::getInstance();
	pDealer->start();
		
	// ��ʼ��UserApi
	pUserApi = CreateEsunnyQuoteApi();	// ����UserApi
	EsunnyQuoteSpi* pUserSpi = new EsunnyQuoteSpi();
	pUserApi->SetSpi(pUserSpi);			// ע���¼���
	bool bRet = pUserApi->Open(ADDR);
	if (!bRet)
    {
		TRACE_LOG("���������������������ʧ��.");
    }
    TRACE_LOG("EsunnyPriceSource started.");	
	started = true;
	Sleep(1500);
	return;
}

void EsunnyPriceSource::ReadIniConfig()
{
	char szFrontAddr[16];
	int nPort;

	if(GetPrivateProfileString("ESUNNY", "IP", "222.88.40.170", szFrontAddr, 16, ".\\AIB.ini"))
	{
		strncpy_s(ADDR.Ip, sizeof(ADDR.Ip), szFrontAddr, sizeof(ADDR.Ip) - 1);
	}
	nPort = GetPrivateProfileInt("ESUNNY", "QOUTEPORT", 6161, ".\\AIB.ini");
	ADDR.Port = nPort;
}


void __cdecl EsunnyQuoteSpi::OnOpen() 
{
	m_iConnState = 1; 
    TRACE_LOG("�ɹ����ӵ������������");
}

void __cdecl EsunnyQuoteSpi::OnClose() 
{
	m_iConnState = 0; 
	TRACE_LOG("�Ͽ���������������ӣ�");
}

void __cdecl EsunnyQuoteSpi::OnQuote(TEsQuoteRspField& rsp)
{
	if(rsp.Exchange == '3') 
	{
		Convert2Lowercase(rsp.Contract,sizeof(rsp.Contract));
	}
	string contractname = rsp.Contract;
	double bid = rsp.Buy;
	double ask = rsp.Sell;
	if (ask > 100000)
	{
		TRACE_LOG("Abnormal price due to DieTing.");
		ask = 0;
	}
	if(bid > 100000 ) 
	{
	    TRACE_LOG("Abnormal price due to ZhangTing.");	
		bid = 0;
	}	
	Contract::get(contractname)->setPrice(0,ask,bid);
}

void EsunnyQuoteSpi::Convert2Lowercase(char content[], int length)
{
	for(int i=0;i<length;i++)
	{
		if(content[i] == 0x00) break;
		if(content[i]>='A' && content[i]<='Z')
		{
			content[i] += 0x20;
		}

	}
}



