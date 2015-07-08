// SfitDealer.cpp: implementation of the SfitDealer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SfitDealer.h"
#include "LogStore.h"
#include <process.h>
#include "Contract.h"

// ���ò���,���������ļ��ж���
static char FRONT_ADDR[128];
static TThostFtdcBrokerIDType	BROKER_ID;			// ���͹�˾����
static TThostFtdcInvestorIDType INVESTOR_ID;		// Ͷ���ߴ���
static TThostFtdcPasswordType  PASSWORD;			// �û�����

//******************class SfitDealerInventory**********************************
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
SfitDealerInventory::SfitDealerInventory(Order *pOrder)
{
	order = pOrder;
	localRef = 0;
	remoteRef = "";
	placeStatus = 0;
	dealedLot = 0;
}

SfitDealerInventory::~SfitDealerInventory()
{
}
//******************class SfitDealer**********************************
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SfitDealer::SfitDealer()
{
    started = false;
	hLogin = ::CreateEvent(NULL,TRUE,FALSE,NULL);
	ReadIniConfig();
	InitializeCriticalSection(&cs);
}

SfitDealer::~SfitDealer()
{
	if(started)
	{
		stop();
	}
	DeleteCriticalSection(&cs);
	::CloseHandle(hLogin);
}

SfitDealerInventory* SfitDealer::addInventory(Order *order)
{
	SfitDealerInventory* ret = NULL;
	string key = order->getId();
	//���Ƿ����ظ���key
	if(dealerInventoryMap.count(key)!=0)
	{
		return ret;
	}
	ret = new SfitDealerInventory(order);
	dealerInventoryMap.insert(make_pair(key,ret));
	return ret;
}

bool SfitDealer::delInventory(SfitDealerInventory* pDealerInventory)
{
	dealerInventoryMap.erase(pDealerInventory->order->getId());
	delete pDealerInventory;
	return TRUE;
}

SfitDealerInventory* SfitDealer::getInventory(string key)
{
	SfitDealerInventory* ret = NULL;
	map<string, SfitDealerInventory*>::iterator iter = dealerInventoryMap.find(key);
	if(iter == dealerInventoryMap.end()) return ret;
	return iter->second;
}

SfitDealerInventory* SfitDealer::findInventorybyLocal(int localRef)
{
	SfitDealerInventory* ret = NULL;
	map<string, SfitDealerInventory*>::iterator iter = dealerInventoryMap.begin();
	while(iter != dealerInventoryMap.end())
	{
		if(iter->second->localRef == localRef)
		{
			ret = iter->second;
			break;
		}
		iter ++;
	}
	return ret;
}

SfitDealerInventory* SfitDealer::findInventorybyRemote(string remoteRef)
{
	SfitDealerInventory* ret = NULL;
	map<string, SfitDealerInventory*>::iterator iter = dealerInventoryMap.begin();
	while(iter != dealerInventoryMap.end())
	{
		if(iter->second->remoteRef == remoteRef)
		{
			ret = iter->second;
			break;
		}
		iter ++;
	}
	return ret;
}

void SfitDealer::ReadIniConfig()
{
	char szFrontAddr[128];
	char szBrokerId[16];
	char szInvestorId[16];
	char szPassword[16];
	memset(szFrontAddr,  0, sizeof(szFrontAddr));
	memset(szBrokerId,   0, sizeof(szBrokerId));
	memset(szInvestorId, 0, sizeof(szInvestorId));
	memset(szPassword,   0, sizeof(szPassword));

	if(GetPrivateProfileString("TRADE", "FRONT_ADDR", "tcp://116.228.223.29:26205", szFrontAddr, 128, ".\\AIB.ini"))
	{
		strcpy_s(FRONT_ADDR, 128, szFrontAddr);
	}
	if(GetPrivateProfileString("TRADE", "BROKER_ID", "4010", szBrokerId, 16, ".\\AIB.ini"))
	{
        strcpy_s(BROKER_ID, 11, szBrokerId);
	}
	if(GetPrivateProfileString("TRADE", "INVESTOR_ID",  "80000003", szInvestorId, 16, ".\\AIB.ini"))
	{
        strcpy_s(INVESTOR_ID, 13, szInvestorId);
	}
	if(GetPrivateProfileString("TRADE", "PASSWORD", "999999", szPassword, 16, ".\\AIB.ini"))
	{
        strcpy_s(PASSWORD, 41, szPassword);
	}
}

// ret=1 means BZ
int SfitDealer::readCommodityProp(const char* szCommodityName)
{
	int ret = 0;
	char szProp[32];
	memset(szProp,  0, sizeof(szProp));

	GetPrivateProfileString("TRADE", szCommodityName, "N/A", szProp, 32, ".\\AIB.ini");
	if(strcmp("BZ",szProp)==0)
	{
		ret = 1;
	}
	return ret;
}

//ͬ����start,�ȵ�¼���ŷ���
void SfitDealer::start()
{
    if(started) {
	    TRACE_LOG("SfitDealer already started.");
		return;
	}
	pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi(".\\dealstream\\",false);			// ����UserApi
	pUserSpi = new CSFITTraderSpi();
	pUserSpi->m_pDealer = this;
	pUserApi->RegisterSpi((CThostFtdcTraderSpi*)pUserSpi);			// ע���¼���
	pUserApi->SubscribePublicTopic(THOST_TERT_QUICK);				// ע�ṫ����
	pUserApi->SubscribePrivateTopic(THOST_TERT_QUICK);			// ע��˽����
	//pUserApi->SubscribePublicTopic(THOST_TERT_RESTART);				// ע�ṫ����
	//pUserApi->SubscribePrivateTopic(THOST_TERT_RESTART);			// ע��˽����
	
	pUserApi->RegisterFront(FRONT_ADDR);
	::ResetEvent(hLogin);
	pUserApi->Init();
	//wait for login event signaled. ::SetEvent(hLogin)
	DWORD dwRet = WaitForSingleObject(hLogin,10*1000); //wait 10 secs at most.
	if (dwRet == WAIT_TIMEOUT)
	{//������ʱ���Ǹ澯��־���˳�����
		TRACE_ALARM("SFITDealer Starts timeout!");
		exit(1);
	}
	else if (dwRet == WAIT_OBJECT_0)
	{//��������
		//�� ÿ�γ��µ�������ר���̣߳��������µ��ӵĺ�����
		// _beginthread( workDealReportFunction, 0, (void *)this);
		TRACE_LOG("SfitDealer Starts OK.");
	    started = true;		
		return;
	}
	else //WAIT_ABANDONED
	{
		TRACE_ALARM("SfitDealer Starts failed!");
		exit(1);
	}
}

void SfitDealer::stop()
{
	if(pUserApi)
	{
		pUserApi->RegisterSpi(NULL);
		pUserApi->Release();
		pUserApi=NULL;
	}
	if(pUserSpi)
	{
		delete pUserSpi;
		pUserSpi = NULL;
	}
	started = false;
}

void SfitDealer::placeOrder(Order *order)
{
	int ref;
	//����map��,ִ����Ӧ����,�����ȴ��߳�
	TRACE_LOG("SfitDealer::placeOrder, orderid:%s.",order->getId().c_str());
	if(!started)
	{
		TRACE_LOG("SfitDealer::placeOrder failed. sfit not started.");
		order->setRejected();
		return;
	}
	//����map��
	EnterCriticalSection(&cs);
	SfitDealerInventory* pInventory = addInventory(order);
	if(pInventory == NULL)
	{//orderID�����ڴ����е��ظ���Ҫ�ܾ�����µ�����
		LeaveCriticalSection(&cs);
		TRACE_LOG("SfitDealer::placeOrder failed. orderID is duplicated.");
		//order->setRejected();
		return;
	}
	//����localRef
	ref = pUserSpi->getCurOrderRef();
	pInventory->localRef = ref;
	LeaveCriticalSection(&cs);
	//ִ���µ�����
	// commodityProp==0 means Ͷ��; commodityProp==1 means �ױ�
	int commodityProp = this->readCommodityProp(Contract::get(order->getContract())->getComodityName().c_str());
	if(0 != pUserSpi->InsertOrder(ref,order,commodityProp))
	{//�µ�ֱ��ʧ��
		EnterCriticalSection(&cs);
		delInventory(pInventory);
		LeaveCriticalSection(&cs);
		TRACE_LOG("SfitDealer::placeOrder failed. �µ�ֱ��ʧ��");
		order->setRejected();
		return;
	}
	return;
}

void SfitDealer::cancelOrder(Order *order)
{
	//�޸�map����Ӧ���ӵ�״̬��ִ����Ӧ����
	TRACE_LOG("SfitDealer::cancelOrder, orderid:%s.",order->getId().c_str());
	if(!started)
	{
		TRACE_LOG("SfitDealer::cancelOrder failed. sfit not started.");
		return;
	}
	//�鿴order״̬���Ƿ���Ҫ����
	if(order->isCanceled()||order->isDealed()||order->isRejected())
	{
		TRACE_LOG("SfitDealer::cancelOrder failed. orderid:%s is not in right status.",order->getId().c_str());
		return;
	}
	//�Ӳֿ����ó�ref
	int ref;
	EnterCriticalSection(&cs);
	SfitDealerInventory* pInventory = getInventory(order->getId());
	if(pInventory==NULL)
	{
		LeaveCriticalSection(&cs);
		TRACE_LOG("SfitDealer::cancelOrder failed. orderid:%s is not in this dealer.",order->getId().c_str());
		return;
	}
	ref = pInventory->localRef;
	LeaveCriticalSection(&cs);
	//ִ����Ӧ����
	if(0 != pUserSpi->CancelOrder(ref,order))
	{
		TRACE_LOG("SfitDealer::cancelOrder failed. orderid:%s ����ֱ��ʧ��",order->getId().c_str());
		return;
	}
	return;
}

void SfitDealer::OnReturnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder)
{
	int ref = atoi(pInputOrder->OrderRef);
	//�ڲ��н��ײ����ǰ����,ͨ��ɨ��inventoryMap���refӳ��
	EnterCriticalSection(&cs);
	SfitDealerInventory* pInventory = findInventorybyLocal(ref);
	handleOrderInsertReport(pInventory,pInputOrder);
	LeaveCriticalSection(&cs);
}

void SfitDealer::OnReturnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction)
{
	int ref = atoi(pInputOrderAction->OrderRef);
	//�ڲ��н��ײ����ǰ����,ͨ��ɨ��inventoryMap���refӳ��
	EnterCriticalSection(&cs);
	SfitDealerInventory* pInventory = findInventorybyLocal(ref);
	handleOrderActionReport(pInventory,pInputOrderAction);
	LeaveCriticalSection(&cs);
}

void SfitDealer::handleOrderInsertReport(SfitDealerInventory* pInventory, CThostFtdcInputOrderField *pInputOrder)
{
	if(pInventory == NULL) return;
	pInventory->placeStatus = 5;
	TRACE_DEBUG("[���ػص�]������������,�ж�Ϊ��������.���㱨. ref=%d.",pInventory->localRef);
	pInventory->order->setRejected();
	delInventory(pInventory);
}

void SfitDealer::handleOrderActionReport(SfitDealerInventory* pInventory, CThostFtdcInputOrderActionField *pInputOrder)
{
	if(pInventory == NULL) return;
	if(pInventory->placeStatus == 0) // new order without any response from remote server.
	{
		TRACE_DEBUG("[���ػص�]��������,�ж�Ϊԭ����δ������.���㱨�����ɹ�����ɾ���õ���¼. ref=%d.",pInventory->localRef);
		pInventory->order->setCancel();
		delInventory(pInventory);
	}
}

//���ػص�������,ref���ݴ���0�ı���״̬֪ͨ
//ͨ��refӳ���inventory,�ٸ���pOrder�����޸�inventory
void SfitDealer::OnReturnOrder(CThostFtdcOrderField *pOrder)
{
	int ref = atoi(pOrder->OrderRef);
	//�ڲ��н��ײ����ǰ����,ͨ��ɨ��inventoryMap���refӳ��
	EnterCriticalSection(&cs);
	SfitDealerInventory* pInventory = findInventorybyLocal(ref);
	handleOrderStatusReport(pInventory,pOrder);
	LeaveCriticalSection(&cs);
}

//Ҫ�ֱ�������Ϣ:�µ��ɹ�(��¼orderSysID),�µ�ʧ��,�����ɹ�,����ʧ��.
/////////////////////////////////////////////////////////////////////////
///typedef char TThostFtdcOrderSubmitStatusType;��һ�������ύ״̬����
/////////////////////////////////////////////////////////////////////////
///�Ѿ��ύ #define THOST_FTDC_OSS_InsertSubmitted '0'
///�����Ѿ��ύ #define THOST_FTDC_OSS_CancelSubmitted '1'
///�޸��Ѿ��ύ #define THOST_FTDC_OSS_ModifySubmitted '2'
///�Ѿ����� #define THOST_FTDC_OSS_Accepted '3'
///�����Ѿ����ܾ� #define THOST_FTDC_OSS_InsertRejected '4'
///�����Ѿ����ܾ� #define THOST_FTDC_OSS_CancelRejected '5'
///�ĵ��Ѿ����ܾ� #define THOST_FTDC_OSS_ModifyRejected '6'
/////////////////////////////////////////////////////////////////////////
///typedef char TThostFtdcOrderStatusType;��һ������״̬����
/////////////////////////////////////////////////////////////////////////
///ȫ���ɽ� #define THOST_FTDC_OST_AllTraded '0'
///���ֳɽ����ڶ����� #define THOST_FTDC_OST_PartTradedQueueing '1'
///���ֳɽ����ڶ����� #define THOST_FTDC_OST_PartTradedNotQueueing '2'
///δ�ɽ����ڶ����� #define THOST_FTDC_OST_NoTradeQueueing '3'
///δ�ɽ����ڶ����� #define THOST_FTDC_OST_NoTradeNotQueueing '4'
///���� #define THOST_FTDC_OST_Canceled '5'
///δ֪ #define THOST_FTDC_OST_Unknown 'a'
///��δ���� #define THOST_FTDC_OST_NotTouched 'b'
///�Ѵ��� #define THOST_FTDC_OST_Touched 'c'

void SfitDealer::handleOrderStatusReport(SfitDealerInventory* pInventory, CThostFtdcOrderField *pOrder)
{
	if(pInventory == NULL) return;
	TRACE_DEBUG("[���ػص�]����״̬֪ͨ��Ref:%s,OrderSysID:%s,SubmitStatus:%c,OrderStatus:%c.",
		pOrder->OrderRef,pOrder->OrderSysID,pOrder->OrderSubmitStatus,pOrder->OrderStatus);		

	if (pOrder->OrderSubmitStatus == THOST_FTDC_OSS_CancelRejected)
	{//��������
		TRACE_DEBUG("[���ػص�]����״̬֪ͨ,�ж�Ϊ��������.");
	}
	else if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled)
	{//�����ɹ�
		pInventory->placeStatus = 4;
		TRACE_DEBUG("[���ػص�]����״̬֪ͨ,�ж�Ϊ�����ɹ�.���㱨.");
		pInventory->order->setCancel();
		delInventory(pInventory);
	}
	else if(pOrder->OrderSubmitStatus == THOST_FTDC_OSS_InsertRejected)
	{//��������
		pInventory->placeStatus = 5;
		TRACE_DEBUG("[���ػص�]����״̬֪ͨ,�ж�Ϊ��������.���㱨.");
		pInventory->order->setRejected();
		delInventory(pInventory);
	}
	else
	{//�����Ǳ��������
		pInventory->remoteRef = pOrder->OrderSysID;
		if(pInventory->remoteRef != "" && pInventory->placeStatus == 0) //�±����Ѿ�������
			pInventory->placeStatus = 1;
		if (pOrder->OrderSubmitStatus == THOST_FTDC_OSS_Accepted && pOrder->OrderStatus == THOST_FTDC_OST_NoTradeQueueing)
		{//�ҵ������
			TRACE_DEBUG("[���ػص�]����״̬֪ͨ,�ж�Ϊ�ҵ�.");
		}
		else if (pOrder->OrderSubmitStatus == THOST_FTDC_OSS_InsertSubmitted && pOrder->OrderStatus == THOST_FTDC_OST_AllTraded)
		{//���̳ɽ������
			TRACE_DEBUG("[���ػص�]����״̬֪ͨ,�ж�Ϊ���̳ɽ�.");
		}
		else if (pOrder->OrderSubmitStatus == THOST_FTDC_OSS_InsertSubmitted && pOrder->OrderStatus == THOST_FTDC_OST_PartTradedQueueing)
		{//���ֳɽ����ֹҵ������
			TRACE_DEBUG("[���ػص�]����״̬֪ͨ,�ж�Ϊ���ֳɽ����ֹҵ�.");
		}
		else
		{//�������
			TRACE_DEBUG("[���ػص�]����״̬֪ͨ,�ж�Ϊ�������.");
		}
	}
}


//���ػص�������,���гɽ�֪ͨ
//ͨ��orderSysIDӳ���inventory,�ٸ���pTrade�����޸�inventory
void SfitDealer::OnTrade(CThostFtdcTradeField *pTrade)
{
	//�ڲ��н��ײ����ǰ����,ͨ��ɨ��inventoryMap���orderSysIDӳ��
	EnterCriticalSection(&cs);
	SfitDealerInventory* pInventory = findInventorybyRemote(string(pTrade->OrderSysID));
	handleTradeReport(pInventory,pTrade);
	LeaveCriticalSection(&cs);
}


void SfitDealer::handleTradeReport(SfitDealerInventory* pInventory, CThostFtdcTradeField *pTrade)
{
	if(pInventory == NULL) return;
	TRACE_LOG("[���ػص�]��ע�ĳɽ������ݣ�%s,ordersysid:%s,[%c,%c],%.3f,%d hand at %s %s.",
			pTrade->InstrumentID,pTrade->OrderSysID,
			pTrade->OffsetFlag,pTrade->Direction,
			pTrade->Price,pTrade->Volume,
			pTrade->TradeDate,pTrade->TradeTime);
	pInventory->dealedLot += pTrade->Volume;
	if(pInventory->dealedLot < pInventory->order->getLot())
	{//partial dealed.
		pInventory->placeStatus = 2;
		pInventory->order->addDetail(new OrderDetail(pInventory->order->getId(),pTrade->Volume,pTrade->Price));
	}
	else
	{//all dealed.
		pInventory->placeStatus = 3;
		pInventory->order->addDetail(new OrderDetail(pInventory->order->getId(),pTrade->Volume,pTrade->Price));
		delInventory(pInventory);
	}
}



//**********************class CSFITTraderSpi**************************
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSFITTraderSpi::CSFITTraderSpi()
{
	SESSION_ID=0;
	FRONT_ID=0;
	iRequestID=0;
	iNextOrderRef=0;
	memset(m_OrderSysID,0,sizeof(m_OrderSysID));
}
CSFITTraderSpi::~CSFITTraderSpi()
{
}
//////////////////////////////////////////////////////////////////////
// CALLBACKS
//////////////////////////////////////////////////////////////////////
void CSFITTraderSpi::OnFrontConnected()
{
	TRACE_LOG("[���ػص�]IP�����ѽ���.");
	/*
	//request login.
	CThostFtdcReqAuthenticateField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.UserID, INVESTOR_ID);
	strcpy(req.AuthCode,"C6LKF3H2FJLSCDFN");  //xujun
	//strcpy(req.AuthCode,"MPZ7SSQIO5C8PCU6"); //xiaoyuetest
    strcpy(req.UserProductInfo, "xujun");
	//strcpy(req.UserProductInfo, "abc"); 
	int iResult = m_pDealer->pUserApi->ReqAuthenticate(&req, ++iRequestID);
	TRACE_LOG("[����]������֤����: %s,%s, %s.",
		BROKER_ID,INVESTOR_ID,((iResult == 0) ? "���ͳɹ�" : "����ʧ��"));
	*/
	
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy_s(req.BrokerID, 11, BROKER_ID);
	strcpy_s(req.UserID, 13, INVESTOR_ID);
	strcpy_s(req.Password, 41, PASSWORD);
	int iResult = m_pDealer->pUserApi->ReqUserLogin(&req, ++iRequestID);
	TRACE_LOG("[����]�����û���¼����: %s,%s, %s.",
		BROKER_ID,INVESTOR_ID,((iResult == 0) ? "���ͳɹ�" : "����ʧ��"));
	
	
}

void CSFITTraderSpi::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateFiels, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    TRACE_LOG("OnRspAuthenticate");
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy_s(req.BrokerID, 11, BROKER_ID);
	strcpy_s(req.UserID, 13, INVESTOR_ID);
	strcpy_s(req.Password, 41, PASSWORD);
	int iResult = m_pDealer->pUserApi->ReqUserLogin(&req, ++iRequestID);
	TRACE_LOG("[����]�����û���¼����: %s,%s, %s.",
		BROKER_ID,INVESTOR_ID,((iResult == 0) ? "���ͳɹ�" : "����ʧ��"));
}

void CSFITTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		// ����Ự����
		FRONT_ID = pRspUserLogin->FrontID;
		SESSION_ID = pRspUserLogin->SessionID;
		iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
		iNextOrderRef++;
		TRACE_LOG("[���ػص�]�û���¼�ɹ�: ������%s,ǰ�ñ��%d,�Ự���%d,��������%d.",
		m_pDealer->pUserApi->GetTradingDay(),FRONT_ID,SESSION_ID,iNextOrderRef);
		///Ͷ���߽�����ȷ��
		CThostFtdcSettlementInfoConfirmField req;
		memset(&req, 0, sizeof(req));
		strcpy_s(req.BrokerID, 11, BROKER_ID);
		strcpy_s(req.InvestorID, 13, INVESTOR_ID);
		int iResult = m_pDealer->pUserApi->ReqSettlementInfoConfirm(&req, ++iRequestID);
		TRACE_LOG("[����]���ͽ�������: %s,%s, %s.",
		BROKER_ID,INVESTOR_ID,((iResult == 0) ? "���ͳɹ�" : "����ʧ��"));
	}
	else
	{
		TRACE_LOG("[���ػص�]�û���¼ʧ��.");
	}
}

void CSFITTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		TRACE_LOG("[���ػص�]����ɹ�.");
		::SetEvent(m_pDealer->hLogin);
	}
	else
	{
		TRACE_LOG("[���ػص�]����ʧ��.");
	}
}

void CSFITTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	TRACE_DEBUG("[���ػص�]����Ӧ��.");
	if(IsErrorRspInfo(pRspInfo))
	{
		TRACE_LOG("[���ػص�]����Ӧ����������д�.");
		// ����12��29���ܽܲ����з��ֵ����⣨ƽ���λ���㣩���˴����Ӵ����߼�
		// ���жϱ����Ƿ������ͳ�
		int ref = 0;
		ref = atoi(pInputOrder->OrderRef);
		if(ref > 0)
		{// �ж�Ϊ�Լ������ͳ��ı���
			m_pDealer->OnReturnRspOrderInsert(pInputOrder);
		}
	}
}

void CSFITTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	TRACE_DEBUG("[���ػص�]����Ӧ��.");
	if(IsErrorRspInfo(pRspInfo))
	{
		TRACE_LOG("[���ػص�]����Ӧ�������Ӧ�����ѳ����ѳɽ�.");
		//����3��2���ܽ�ʹ���з��ֵ����⣨�����޻ظ�����µĳ���ʧ��Ӧ������Ϊ�����ɹ������˴����Ӵ����߼�
		// ���жϱ����Ƿ������ͳ�
		int ref = 0;
		ref = atoi(pInputOrderAction->OrderRef);
		if(ref > 0)
		{// �ж�Ϊ�Լ������ͳ��ı���
			m_pDealer->OnReturnRspOrderAction(pInputOrderAction);
		}
	}
}

///����֪ͨ,˽����
void CSFITTraderSpi::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	if (!IsMyOrder(pOrder)) return;

	m_pDealer->OnReturnOrder(pOrder);
}

///�ɽ�֪ͨ,˽����
void CSFITTraderSpi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	m_pDealer->OnTrade(pTrade);
}

void CSFITTraderSpi:: OnFrontDisconnected(int nReason)
{
	TRACE_DEBUG("OnFrontDisconnected. Reason=%d.",nReason);
}
		
void CSFITTraderSpi::OnHeartBeatWarning(int nTimeLapse)
{
	TRACE_DEBUG("OnHeartBeatWarning. nTimerLapse=%d.",nTimeLapse);
}

void CSFITTraderSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	TRACE_ALARM("[���ػص�]���������.");
	IsErrorRspInfo(pRspInfo);
}

bool CSFITTraderSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	// ���ErrorID != 0, ˵���յ��˴������Ӧ
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
	{
		TRACE_ALARM("ErrorID=%d, ErrorMsg=%s.",pRspInfo->ErrorID,pRspInfo->ErrorMsg);
	}
	return bResult;
}

//ͨ���Ự�����ж�˽�����ı���֪ͨ�Ƿ��Լ�������
bool CSFITTraderSpi::IsMyOrder(CThostFtdcOrderField *pOrder)
{
	int ref = 0;
	ref = atoi(pOrder->OrderRef);
	return ((pOrder->FrontID == FRONT_ID) &&
			(pOrder->SessionID == SESSION_ID) &&
			(ref != 0));
}

//���������ؽӿ�ʵ�ַ���
int CSFITTraderSpi::InsertOrder(int orderRef, Order *order, int commodityProp)
{
	CThostFtdcInputOrderField req;
	memset(&req, 0, sizeof(req));
	strcpy_s(req.BrokerID, 11, BROKER_ID);
	strcpy_s(req.InvestorID, 13, INVESTOR_ID);
	sprintf_s(req.OrderRef, 13, "%d", orderRef);
	req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	
	if(commodityProp==1)
	{ // ָ���ױ�
		req.CombHedgeFlag[0] = THOST_FTDC_HF_Hedge;
	}
	else
	{ // Ĭ��Ͷ��
		req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	}
	
	req.TimeCondition = THOST_FTDC_TC_GFD;

	// ���������ͳɽ�����ָʾ
	req.VolumeTotalOriginal = order->getLot();  //���α�����Ŀ������
	req.VolumeCondition = THOST_FTDC_VC_AV;  //�޳ɽ������ҵ�
	req.MinVolume = 1;  //��С�ɽ���������ͬ��Ŀ������������һ����ΪTHOST_FTDC_VC_MV�������ã������𱣻����壩

	req.ContingentCondition = THOST_FTDC_CC_Immediately;
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	req.IsAutoSuspend = 0;
	req.UserForceClose = 0;

	///��Լ����
	strcpy_s(req.InstrumentID, 31, order->getContract().c_str());
	///��������: 
	if(order->getBuySell()==BUY)
		req.Direction = THOST_FTDC_D_Buy;
	else
		req.Direction = THOST_FTDC_D_Sell;
	///��Ͽ�ƽ��־: ����
	if(order->getKaiPing()==KAI_CANG)
		req.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
	else if (order->getKaiPing()==PING_CANG)
		req.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
	else
		req.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
	///�۸�
	req.LimitPrice = order->getPrice();

	int iResult = m_pDealer->pUserApi->ReqOrderInsert(&req, ++iRequestID);
	TRACE_LOG("[����]����¼������: %s,[%c,%c],%.3f, lot:%d, %s, %s.",
		req.InstrumentID,req.CombOffsetFlag[0],req.Direction,req.LimitPrice,req.VolumeTotalOriginal,
		((commodityProp==1)?"�ױ�":"Ͷ��"), ((iResult == 0) ? "���ͳɹ�" : "����ʧ��"));
	return iResult;
}

//���������ؽӿ�ʵ�ַ���
int CSFITTraderSpi::CancelOrder(int orderRef, Order *order)
{
	CThostFtdcInputOrderActionField req;
	memset(&req, 0, sizeof(req));
	strcpy_s(req.BrokerID, 11, BROKER_ID);
	strcpy_s(req.InvestorID, 13, INVESTOR_ID);
	strcpy_s(req.InstrumentID, 31, order->getContract().c_str());
	sprintf_s(req.OrderRef, 13, "%d", orderRef); 

	req.FrontID = FRONT_ID;
	req.SessionID = SESSION_ID;
	req.ActionFlag = THOST_FTDC_AF_Delete;

	int iResult = m_pDealer->pUserApi->ReqOrderAction(&req, ++iRequestID);
	TRACE_LOG("[����]������������: %s, %.3f, %s.",
		req.InstrumentID,req.LimitPrice,((iResult == 0) ? "���ͳɹ�" : "����ʧ��"));
	return iResult;
}

int CSFITTraderSpi::getCurOrderRef()
{
	int ret = iNextOrderRef;
	iNextOrderRef++;
	return ret;
}

