// RohonPriceSource.cpp: implementation of the RohonPriceSource class.
//
//////////////////////////////////////////////////////////////////////

#include "RohonPriceSource.h"
#include <iostream>
#include "LogStore.h"
#include <process.h>
//#include <assert.h>
#include "Contract.h"
#include "NameConverter.h"
#include "KnownContract.h"

// UserApi����
static CRohonMdApi* pUserApi;
// ���ò���
static char FRONT_ADDR[128];		    // ǰ�õ�ַ
static char INVESTOR_ID[16];			// Ͷ���ߴ���
static char PASSWORD[41] ;				// �û�����



RohonPriceSource::RohonPriceSource()
{
	m_contracts.clear();
	ReadIniConfig();
	started = false;
}

RohonPriceSource::~RohonPriceSource()
{

}

//�����������Զ�����
void RohonPriceSource::asyncAutoSubscribe()
{
	_beginthread( threadPrice, 0, (void *)this);
}

void RohonPriceSource::threadPrice( void* pParams ) 
{
	TRACE_LOG("Rohon�����첽���ļ۸��߳�.");
	RohonPriceSource * ps = (RohonPriceSource*)pParams;
	if(ps!=NULL)
		ps->subscribeBatch();
	TRACE_LOG("Rohon�����첽���ļ۸��߳̽���.");
	_endthread();
}


/*
	char **ppInstrumentID = new char* [1];
	const char* pchInstrument = "CME_CBT CORN DEC12";
	ppInstrumentID[0] = new char [strlen(pchInstrument) + 1];
	strcpy(ppInstrumentID[0], pchInstrument);
	m_pApi->SubscribeMarketData(ppInstrumentID, 1);
*/
void RohonPriceSource::subscribeBatch()
{
	TRACE_LOG("Rohon�����첽���ļ۸�, �����ĺ�Լ��%d��.",m_contracts.size());
	set<string>::iterator iter = m_contracts.begin();
	while(iter!=m_contracts.end())
	{
		string contract = *iter;
		char InstrumentID[40];
		char *ppInstrumentID[1];
		strcpy_s(InstrumentID, 40, contract.c_str());
		ppInstrumentID[0] = InstrumentID;
		int iResult = pUserApi->SubscribeMarketData(ppInstrumentID, 1);
		TRACE_LOG("Send subscribe for contract %s, and result %s.", InstrumentID, ((iResult == 0) ? "�ɹ�" : "ʧ��") ) ;
		iter++;
	}
}

void RohonPriceSource::add(string contract)
{  
	//TRACE_LOG("enter RohonPriceSource::add");
	string rohonName = KnownContract::getRonhonNameFor(contract);
	//NameConverter* pNC = NameConverter::getInstance();
	//string rohonName = pNC->base2RohonName(contract);
	TRACE_LOG("spname is: %s and changed name is: %s",contract.c_str(),rohonName.c_str());
	if(rohonName=="") 
	{
		TRACE_LOG("[RohonPriceSource::add]Warning, NO RohonName found according to BaseName:%s.",contract.c_str());
		return;
	}
	char InstrumentID[40];
	char *ppInstrumentID[1];
	strcpy_s(InstrumentID, 40, rohonName.c_str());
	ppInstrumentID[0] = InstrumentID;
	m_contracts.insert(rohonName);
	TRACE_LOG("Send subscribe, %d contracts in Sets.",m_contracts.size());
	int iResult = pUserApi->SubscribeMarketData(ppInstrumentID, 1);
	TRACE_LOG("Send subscribe for contract %s, and result %s.", InstrumentID, ((iResult == 0) ? "�ɹ�" : "ʧ��") ) ;
}

void RohonPriceSource::start(void)
{

	if(started) {
        TRACE_LOG("RohonPriceSource already started.");
	    return;
	}
		
	// ��ʼ��UserApi
	pUserApi = CRohonMdApi::CreateFtdcMdApi();
	//pUserApi = CThostFtdcMdApi::CreateFtdcMdApi(".\\pricestream\\",false);		// ����UserApi
	CRohonMD* rohonMdSpi = new CRohonMD();
	rohonMdSpi->ps = this;
	pUserApi->RegisterSpi(rohonMdSpi);		// ע���¼���
	pUserApi->RegisterFront(FRONT_ADDR);	// "tcp://61.152.108.102:14100\0"
	TRACE_LOG("connect %s.",FRONT_ADDR);
	pUserApi->Init();
	
    TRACE_LOG("RohonPriceSource started.");	
	started = true;
	Sleep(500);
	//pUserApi->Join();
	//pUserApi->Release();
	return;
}

void RohonPriceSource::ReadIniConfig()
{
	char szFrontAddr[128];
	char szInvestorId[16];
	char szPassword[41];

	memset(szFrontAddr,  0, sizeof(szFrontAddr));
	memset(szInvestorId, 0, sizeof(szInvestorId));
	memset(szPassword,   0, sizeof(szPassword));

	if(GetPrivateProfileString("ROHONPS", "FRONT_ADDR", "tcp://61.152.108.102:14100", szFrontAddr, 128, ".\\AIB.ini"))
	{
		strcpy_s(FRONT_ADDR, 128, szFrontAddr);
	}
	if(GetPrivateProfileString("ROHONPS", "INVESTOR_ID",  "demo000", szInvestorId, 16, ".\\AIB.ini"))
	{
        strcpy_s(INVESTOR_ID, 16, szInvestorId);
	}
	if(GetPrivateProfileString("ROHONPS", "PASSWORD", "888888", szPassword, 41, ".\\AIB.ini"))
	{
        strcpy_s(PASSWORD, 41, szPassword);
	}

}




CRohonMD::CRohonMD()
{
	m_nRquestID = 0;
	ps = NULL;
}

CRohonMD::~CRohonMD()
{

}

///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
void CRohonMD::OnFrontConnected()
{
	TRACE_LOG("[Rohon���ػص�]RECV OnFrontConnected");
	printf("[Rohon���ػص�]RECV OnFrontConnected\n");
	Login();
}

///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
///@param nReason ����ԭ��
///        0x1001 �����ʧ��
///        0x1002 ����дʧ��
///        0x2001 ����������ʱ
///        0x2002 ��������ʧ��
///        0x2003 �յ�������
void CRohonMD::OnFrontDisconnected(int nReason)
{
	TRACE_LOG("[Rohon���ػص�]RECV OnFrontDisconnected, Reason=%x.",nReason);
	printf("[Rohon���ػص�]RECV OnFrontDisconnected, Reason=%x.\n",nReason);
}

void CRohonMD::OnHeartBeatWarning(int nTimeLapse)
{
	TRACE_LOG("[Rohon���ػص�]RECV OnHeartBeatWarning, TimeLapse=%d.",nTimeLapse);
	printf("[Rohon���ػص�]RECV OnHeartBeatWarning, TimeLapse=%d.\n",nTimeLapse);
}

///��¼������Ӧ
void CRohonMD::OnRspUserLogin(CRohonRspUserLoginField *pRspUserLogin, CRohonRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	TRACE_LOG("[Rohon���ػص�]RECV OnRspUserLogin");
	printf("[Rohon���ػص�]RECV OnRspUserLogin\n");
	if (!IsErrorRspInfo(pRspInfo))
	{
		///��ȡ��ǰ������
		TRACE_LOG(" Current day is %s .", pUserApi->GetTradingDay());
		printf(" Current day is %s .\n", pUserApi->GetTradingDay());
		// 
		// �Զ���Ӷ�������
		if(ps!=NULL)
			ps->asyncAutoSubscribe();
			
	}
}

///�ǳ�������Ӧ
void CRohonMD::OnRspUserLogout(CRohonUserLogoutField *pUserLogout, CRohonRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	TRACE_LOG("[Rohon���ػص�]RECV OnRspUserLogout");
}

void CRohonMD::OnRspError(CRohonRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	TRACE_LOG("[Rohon���ػص�]RECV OnRspError");
}

void CRohonMD::OnRspSubMarketData(CRohonSpecificInstrumentField *pSpecificInstrument, CRohonRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	TRACE_LOG("[Rohon���ػص�]RECV OnRspSubMarketData");
	printf("[Rohon���ػص�]RECV OnRspSubMarketData\n");
	if (!IsErrorRspInfo(pRspInfo))
	{
		TRACE_LOG("[Rohon���ػص�]Remote acknowledged the subscribe.");
		printf("[Rohon���ػص�]Remote acknowledged the subscribe.\n");
	}
}

void CRohonMD::OnRspUnSubMarketData(CRohonSpecificInstrumentField *pSpecificInstrument, CRohonRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	TRACE_LOG("[Rohon���ػص�]RECV OnRspUnSubMarketData");
}

///�������֪ͨ
void CRohonMD::OnRtnDepthMarketData(CRohonDepthMarketDataField *pDepthMarketData)
{
	//TRACE_DEBUG("[Rohon���ػص�]RECV OnRtnDepthMarketData. contract:%s, ask:%f, bid:%f.",pDepthMarketData->InstrumentID,pDepthMarketData->AskPrice1,pDepthMarketData->BidPrice1);
	//printf("[Rohon���ػص�]RECV OnRtnDepthMarketData. contract:%s, ask:%f, bid:%f.\n",pDepthMarketData->InstrumentID,pDepthMarketData->AskPrice1,pDepthMarketData->BidPrice1);

	Pricing tmpPrice(pDepthMarketData->InstrumentID,pDepthMarketData->AskPrice1,pDepthMarketData->BidPrice1);
	tmpPrice.contract = pDepthMarketData->InstrumentID;
	tmpPrice.ask = pDepthMarketData->AskPrice1;
	tmpPrice.bid = pDepthMarketData->BidPrice1;
	
    if (tmpPrice.ask > 100000 || tmpPrice.bid > 100000 ) {
	    TRACE_LOG("Abnormal price due to ZhangTing or DieTing.");	
		tmpPrice.ask = 0;
		tmpPrice.bid = 0;
	}
	/*//TODO: KnownContract::getSpName(tmpPrice.contract);*/
	/*//*/string baseName = KnownContract::getSpNameFor(tmpPrice.contract);
	//NameConverter* pNC = NameConverter::getInstance();
	//string baseName = pNC->rohon2BaseName(tmpPrice.contract);
	if(baseName=="")
	{
		TRACE_LOG("[Rohon���ػص�]RECV OnRtnDepthMarketData. NO BaseName found according to RohonName:%s.",tmpPrice.contract.c_str());
	}
	else
	{
		Contract::get(baseName)->setPrice(0,tmpPrice.ask,tmpPrice.bid, pDepthMarketData->LastPrice);
	}
}

void CRohonMD::OnRtnUpdateDepthMarketData(CRohonInstrumentUpdateMarketData* pDepthMarketUpdateData)
{
	TRACE_LOG("[Rohon���ػص�]RECV OnRtnUpdateDepthMarketData");
}
	
void CRohonMD::Login()
{
	CRohonReqUserLoginField req;
	memset(&req, 0, sizeof(req));

	strcpy_s(req.UserID, 16, INVESTOR_ID);
	strcpy_s(req.Password, 41, PASSWORD);

	int iResult = pUserApi->ReqUserLogin(&req, AllocRquestID());
	TRACE_LOG("Send login. %s.", ((iResult == 0) ? "�ɹ�" : "ʧ��") ) ;
	printf("Send login. %s.\n", ((iResult == 0) ? "�ɹ�" : "ʧ��") ) ;
}

bool CRohonMD::IsErrorRspInfo(CRohonRspInfoField *pRspInfo)
{
	// ���ErrorID != 0, ˵���յ��˴������Ӧ
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult) 
	{
		TRACE_LOG("Error ID=%d, ErrorMsg=%s.", pRspInfo->ErrorID, pRspInfo->ErrorMsg) ;
		printf("Error ID=%d, ErrorMsg=%s.\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	return bResult;
}
