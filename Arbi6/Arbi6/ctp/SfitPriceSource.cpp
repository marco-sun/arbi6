// SfitPriceSource.cpp: implementation of the SfitPriceSource class.
//
//////////////////////////////////////////////////////////////////////

#include "SfitPriceSource.h"
#include <iostream>
#include "../LogStore.h"
#include <process.h>
#include <assert.h>
#include "../arbi/Contract.h"

// UserApi����
static CThostFtdcMdApi* pUserApi;
// ���ò���
static char FRONT_ADDR[128];		                    // ǰ�õ�ַ
static TThostFtdcBrokerIDType	BROKER_ID ;				// ���͹�˾����
static TThostFtdcInvestorIDType INVESTOR_ID ;			// Ͷ���ߴ���
static TThostFtdcPasswordType   PASSWORD ;			    // �û�����


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SfitPriceSource::SfitPriceSource()
{
	m_contracts.clear();
	ReadIniConfig();
	started = false;
}

SfitPriceSource::~SfitPriceSource()
{

}

//���̻߳㱨�۸�Ҳ��������
void SfitPriceSource::asyncAutoSubscribe()
{
	_beginthread( threadPrice, 0, (void *)this);
}

void SfitPriceSource::threadPrice( void* pParams ) 
{
	TRACE_LOG("���������첽���ļ۸��߳�.");
	SfitPriceSource * ps = (SfitPriceSource*)pParams;
	if(ps!=NULL)
		ps->subscribeBatch();
	TRACE_LOG("���������첽���ļ۸��߳̽���.");
	_endthread();
}

void SfitPriceSource::subscribeBatch()
{
	TRACE_LOG("���������첽���ļ۸�, �����ĺ�Լ��%d��.",m_contracts.size());
	set<string>::iterator iter = m_contracts.begin();
	while(iter!=m_contracts.end())
	{
		string contract = *iter;
		char InstrumentID[10];
		char *ppInstrumentID[1];
		strcpy_s(InstrumentID, 10, contract.c_str());
		ppInstrumentID[0] = InstrumentID;
		int iResult = pUserApi->SubscribeMarketData(ppInstrumentID, 1);
		TRACE_LOG("Send subscribe for contract %s, and result %s.", InstrumentID, ((iResult == 0) ? "�ɹ�" : "ʧ��") ) ;
		iter++;
	}
}

void SfitPriceSource::add(string contract)
{
	char InstrumentID[10];
	char *ppInstrumentID[1];
	strcpy_s(InstrumentID, 10, contract.c_str());
	ppInstrumentID[0] = InstrumentID;
	m_contracts.insert(contract);
	TRACE_LOG("Send subscribe, %d contracts in Sets.",m_contracts.size());
	int iResult = pUserApi->SubscribeMarketData(ppInstrumentID, 1);
	TRACE_LOG("Send subscribe for contract %s, and result %s.", InstrumentID, ((iResult == 0) ? "�ɹ�" : "ʧ��") ) ;
}

void SfitPriceSource::start(void)
{

	if(started) {
        TRACE_LOG("SfitPriceSource already started.");
	    return;
	}
	
	// ��ʼ��UserApi
	pUserApi = CThostFtdcMdApi::CreateFtdcMdApi(".\\pricestream\\",false);		// ����UserApi
	CSFITMdSpi* sfitMdSpi = new CSFITMdSpi();
	sfitMdSpi->ps = this;
	CThostFtdcMdSpi* pUserSpi = sfitMdSpi;
	pUserApi->RegisterSpi(pUserSpi);						// ע���¼���
	pUserApi->RegisterFront(FRONT_ADDR);					// connect
	TRACE_LOG("connect %s.",FRONT_ADDR);
	pUserApi->Init();
	
    TRACE_LOG("SfitPriceSource started.");	
	started = true;
	Sleep(500);
	//pUserApi->Join();
	//pUserApi->Release();
	return;
}

void SfitPriceSource::ReadIniConfig()
{
	char szFrontAddr[128];
	char szBrokerId[16];
	char szInvestorId[16];
	char szPassword[16];

	memset(szFrontAddr,  0, sizeof(szFrontAddr));
	memset(szBrokerId,   0, sizeof(szBrokerId));
	memset(szInvestorId, 0, sizeof(szInvestorId));
	memset(szPassword,   0, sizeof(szPassword));

	if(GetPrivateProfileString("AIB", "FRONT_ADDR", "tcp://116.228.223.29:26213", szFrontAddr, 128, ".\\AIB.ini"))
	{
		strcpy_s(FRONT_ADDR, 128, szFrontAddr);
	}
	if(GetPrivateProfileString("AIB", "BROKER_ID", "4010", szBrokerId, 16, ".\\AIB.ini"))
	{
        strcpy_s(BROKER_ID, 11, szBrokerId);
	}
	if(GetPrivateProfileString("AIB", "INVESTOR_ID",  "80000003", szInvestorId, 16, ".\\AIB.ini"))
	{
        strcpy_s(INVESTOR_ID, 13, szInvestorId);
	}
	if(GetPrivateProfileString("AIB", "PASSWORD", "999999", szPassword, 16, ".\\AIB.ini"))
	{
        strcpy_s(PASSWORD, 41, szPassword);
	}


}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSFITMdSpi::CSFITMdSpi()
{
	iRequestID = 1;
	ps = NULL;
}

CSFITMdSpi::~CSFITMdSpi()
{

}

void CSFITMdSpi::OnFrontConnected()
{
	TRACE_LOG("RECV OnFrontConnected.");
	ReqUserLogin();
}

void CSFITMdSpi::OnFrontDisconnected(int nReason)
{
	TRACE_LOG("RECV OnFrontDisconnected.");
}

void CSFITMdSpi::OnHeartBeatWarning(int nTimeLapse)
{
	TRACE_LOG("RECV OnHeartBeatWarning.");
}

void CSFITMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	TRACE_LOG("RECV OnRspUserLogin.");
	if (!IsErrorRspInfo(pRspInfo))
	{
		///��ȡ��ǰ������
		TRACE_LOG(" Current day is %s .", pUserApi->GetTradingDay());
		// 
		// �Զ���Ӷ�������
		if(ps!=NULL)
			ps->asyncAutoSubscribe();
			
	}
}

///�ǳ�������Ӧ
void CSFITMdSpi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	TRACE_LOG("RECV OnRspUserLogout.");
}

	///����Ӧ��
void CSFITMdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	TRACE_LOG("RECV OnRspError.");
	if (!IsErrorRspInfo(pRspInfo))
	{
		//TRACE_LOG("Remote acknowledged the subscribe.");
	}
}

	///��������Ӧ��
void CSFITMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	TRACE_LOG("RECV OnRspSubMarketData.");
	if (!IsErrorRspInfo(pRspInfo))
	{
		TRACE_LOG("Remote acknowledged the subscribe.");
	}
}

	///ȡ����������Ӧ��
void CSFITMdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	TRACE_LOG("RECV OnRspUnSubMarketData.");
}

	///�������֪ͨ
void CSFITMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	//time_t now;
	//time( &now );
	Pricing tmpPrice(pDepthMarketData->InstrumentID,pDepthMarketData->AskPrice1,pDepthMarketData->BidPrice1);
	//tmpPrice.time = now;
	tmpPrice.contract = pDepthMarketData->InstrumentID;
	tmpPrice.ask = pDepthMarketData->AskPrice1;
	tmpPrice.bid = pDepthMarketData->BidPrice1;
	//tmpPrice.lowerLimit = pDepthMarketData->LowerLimitPrice;
	//tmpPrice.upperLimit = pDepthMarketData->UpperLimitPrice;
	//printf("time: %ld\n", tmpPrice.time);
	//TRACE_LOG("RECV contract %s ask %f bid %f lowerLimit %f upperLimit %f", tmpPrice.contract.c_str(), tmpPrice.ask, tmpPrice.bid, tmpPrice.lowerLimit, tmpPrice.upperLimit);

    if (tmpPrice.ask > 1000000 || tmpPrice.bid > 1000000 ) {
	    TRACE_LOG("Abnormal price due to ZhangTing or DieTing.");	
		tmpPrice.ask = 0;
		tmpPrice.bid = 0;
	}	
  	//��REST�ܹ��£��Ƿ���Ҫ���̻߳㱨�۸�Ҳ��������
    //SfitPriceSource * ps = SfitPriceSource::getInstance();
	//ps->asyncInform(tmpPrice);
	//Contract::get(tmpPrice.contract)->setPrice(0,tmpPrice.ask,tmpPrice.bid);
	Contract::get(tmpPrice.contract)->setPrice(0,tmpPrice.ask,tmpPrice.bid, pDepthMarketData->LastPrice);
}

void CSFITMdSpi::ReqUserLogin()
{                                                           
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy_s(req.BrokerID, 11, BROKER_ID);
	strcpy_s(req.UserID, 13, INVESTOR_ID);
	strcpy_s(req.Password, 41, PASSWORD);
	int iResult = pUserApi->ReqUserLogin(&req, ++iRequestID);
	TRACE_LOG("Send login. %s.", ((iResult == 0) ? "�ɹ�" : "ʧ��") ) ;
}

bool CSFITMdSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	// ���ErrorID != 0, ˵���յ��˴������Ӧ
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		TRACE_LOG("Error ID=%d, ErrorMsg=%s.", pRspInfo->ErrorID, pRspInfo->ErrorMsg) ;
		printf("Error ID=%d, ErrorMsg=%s.", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	return bResult;
}
