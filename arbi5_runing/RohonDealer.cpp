// RohonDealer.cpp: implementation of the RohonDealer class.
//
//////////////////////////////////////////////////////////////////////

#include "RohonDealer.h"
#include "NameConverter.h"
#include "LogStore.h"
#include <process.h>

// ���ò���,���������ļ��ж���
static char FRONT_ADDR[128];		    // ǰ�õ�ַ
static char INVESTOR_ID[16];			// Ͷ���ߴ���
static char PASSWORD[41] ;				// �û�����

//******************class RohonDealerInventory**********************************
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
RohonDealerInventory::RohonDealerInventory(Order *pOrder)
{
	order = pOrder;
	localRef = 0;
	remoteRef = "";
	placeStatus = 0;
	dealedLot = 0;
}

RohonDealerInventory::~RohonDealerInventory()
{
}

//******************class RohonDealer**********************************
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RohonDealer::RohonDealer()
{
    started = false;
	hLogin = ::CreateEvent(NULL,TRUE,FALSE,NULL);
	ReadIniConfig();
	InitializeCriticalSection(&cs);
}

RohonDealer::~RohonDealer()
{
	if(started)
	{
		stop();
	}
	DeleteCriticalSection(&cs);
	::CloseHandle(hLogin);
}

RohonDealerInventory* RohonDealer::addInventory(Order *order)
{
	RohonDealerInventory* ret = NULL;
	string key = order->getId();
	//���Ƿ����ظ���key
	if(dealerInventoryMap.count(key)!=0)
	{
		return ret;
	}
	ret = new RohonDealerInventory(order);
	dealerInventoryMap.insert(make_pair(key,ret));
	return ret;
}

bool RohonDealer::delInventory(RohonDealerInventory* pDealerInventory)
{
	dealerInventoryMap.erase(pDealerInventory->order->getId());
	delete pDealerInventory;
	return TRUE;
}

RohonDealerInventory* RohonDealer::getInventory(string key)
{
	RohonDealerInventory* ret = NULL;
	map<string, RohonDealerInventory*>::iterator iter = dealerInventoryMap.find(key);
	if(iter == dealerInventoryMap.end()) return ret;
	return iter->second;
}

RohonDealerInventory* RohonDealer::findInventorybyLocal(int localRef)
{
	RohonDealerInventory* ret = NULL;
	map<string, RohonDealerInventory*>::iterator iter = dealerInventoryMap.begin();
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

RohonDealerInventory* RohonDealer::findInventorybyRemote(string remoteRef)
{
	RohonDealerInventory* ret = NULL;
	map<string, RohonDealerInventory*>::iterator iter = dealerInventoryMap.begin();
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

void RohonDealer::ReadIniConfig()
{
	char szFrontAddr[128];
	char szInvestorId[16];
	char szPassword[41];
	memset(szFrontAddr,  0, sizeof(szFrontAddr));
	memset(szInvestorId, 0, sizeof(szInvestorId));
	memset(szPassword,   0, sizeof(szPassword));

	if(GetPrivateProfileString("ROHONTRADE", "FRONT_ADDR", "tcp://61.152.108.102:5101", szFrontAddr, 128, ".\\AIB.ini"))
	{
		strcpy_s(FRONT_ADDR, 128, szFrontAddr);
	}
	if(GetPrivateProfileString("ROHONTRADE", "INVESTOR_ID",  "demo000", szInvestorId, 16, ".\\AIB.ini"))
	{
        strcpy_s(INVESTOR_ID, 16, szInvestorId);
	}
	if(GetPrivateProfileString("ROHONTRADE", "PASSWORD", "888888", szPassword, 41, ".\\AIB.ini"))
	{
        strcpy_s(PASSWORD, 41, szPassword);
	}
}

//ͬ����start,�ȵ�¼���ŷ���
void RohonDealer::start()
{
    if(started) {
	    TRACE_LOG("RohonDealer already started.");
		return;
	}
	//pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi(".\\dealstream\\",false);			// ����UserApi

	pUserApi = CRohonTraderClientApi::CreateRHTraderApi();
	pUserSpi = new CROHONTraderSpi();
	pUserSpi->m_pDealer = this;
	pUserApi->RegisterSpi(pUserSpi);			// ע���¼���
	///���Ĺ�������
	///@param nResumeType �������ش���ʽ  
	///        ROHON_TERT_RESTART:�ӱ������տ�ʼ�ش�
	///        ROHON_TERT_RESUME:���ϴ��յ�������
	///        ROHON_TERT_QUICK:ֻ���͵�¼�󹫹���������
	///@remark �÷���Ҫ��Init����ǰ���á����������򲻻��յ������������ݡ�
	pUserApi->SubscribePublicTopic(ROHON_TERT_QUICK);				// ע�ṫ����
	///����˽������
	///@param nResumeType ˽�����ش���ʽ  
	///        ROHON_TERT_RESTART:�ӱ������տ�ʼ�ش�
	///        ROHON_TERT_RESUME:���ϴ��յ�������
	///        ROHON_TERT_QUICK:ֻ���͵�¼��˽����������
	///@remark �÷���Ҫ��Init����ǰ���á����������򲻻��յ�˽���������ݡ�
	pUserApi->SubscribePrivateTopic(ROHON_TERT_QUICK);			// ע��˽����

	pUserApi->RegisterFront(FRONT_ADDR);
	::ResetEvent(hLogin);
	pUserApi->Init();

	//pUserApi->SubscribePublicTopic(THOST_TERT_RESTART);				// ע�ṫ����
	//pUserApi->SubscribePrivateTopic(THOST_TERT_RESTART);			// ע��˽����
	
	//wait for login event signaled. ::SetEvent(hLogin)
	DWORD dwRet = WaitForSingleObject(hLogin,10*1000); //wait 10 secs at most.
	if (dwRet == WAIT_TIMEOUT)
	{//������ʱ���Ǹ澯��־���˳�����
		TRACE_ALARM("RohonDealer Starts timeout!");
		printf("RohonDealer Starts timeout!\n");
		exit(1);
	}
	else if (dwRet == WAIT_OBJECT_0)
	{//��������
		//�� ÿ�γ��µ�������ר���̣߳��������µ��ӵĺ�����
		// _beginthread( workDealReportFunction, 0, (void *)this);
		TRACE_LOG("RohonDealer Starts OK.");
		printf("RohonDealer Starts OK.\n");
	    started = true;		
		return;
	}
	else //WAIT_ABANDONED
	{
		TRACE_ALARM("RohonDealer Starts failed!");
		printf("RohonDealer Starts failed!\n");
		exit(1);
	}
}

void RohonDealer::stop()
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

void RohonDealer::queryAllInstrument()
{
	if(started)
	{
		pUserSpi->QueryAllInstrument();
	}
}

void RohonDealer::placeOrder(Order *order)
{
	int ref;
	//����map��,ִ����Ӧ����,�����ȴ��߳�
	TRACE_LOG("RohonDealer::placeOrder, orderid:%s.",order->getId().c_str());
	if(!started)
	{
		TRACE_LOG("RohonDealer::placeOrder failed. RohonDealer not started.");
		order->setRejected();
		return;
	}
	//����map��
	EnterCriticalSection(&cs);
	RohonDealerInventory* pInventory = addInventory(order);
	if(pInventory == NULL)
	{//orderID�����ڴ����е��ظ���Ҫ�ܾ�����µ�����
		LeaveCriticalSection(&cs);
		TRACE_LOG("RohonDealer::placeOrder failed. orderID is duplicated.");
		//order->setRejected();
		return;
	}
	//����localRef
	ref = pUserSpi->getCurOrderRef();
	pInventory->localRef = ref;
	LeaveCriticalSection(&cs);
	//ִ���µ�����
	if(0 != pUserSpi->InsertOrder(ref,order))
	{//�µ�ֱ��ʧ��
		EnterCriticalSection(&cs);
		delInventory(pInventory);
		LeaveCriticalSection(&cs);
		TRACE_LOG("RohonDealer::placeOrder failed. �µ�ֱ��ʧ��");
		order->setRejected();
		return;
	}
	return;
}

void RohonDealer::cancelOrder(Order *order)
{
	//�޸�map����Ӧ���ӵ�״̬��ִ����Ӧ����
	TRACE_LOG("RohonDealer::cancelOrder, orderid:%s.",order->getId().c_str());
	if(!started)
	{
		TRACE_LOG("RohonDealer::cancelOrder failed. sfit not started.");
		return;
	}
	//�鿴order״̬���Ƿ���Ҫ����
	if(order->isCanceled()||order->isDealed()||order->isRejected())
	{
		TRACE_LOG("RohonDealer::cancelOrder failed. orderid:%s is not in right status.",order->getId().c_str());
		return;
	}
	//�Ӳֿ����ó�ref
	int ref;
	EnterCriticalSection(&cs);
	RohonDealerInventory* pInventory = getInventory(order->getId());
	if(pInventory==NULL)
	{
		LeaveCriticalSection(&cs);
		TRACE_LOG("RohonDealer::cancelOrder failed. orderid:%s is not in this dealer.",order->getId().c_str());
		return;
	}
	ref = pInventory->localRef;
	LeaveCriticalSection(&cs);
	//ִ����Ӧ����
	if(0 != pUserSpi->CancelOrder(ref,order))
	{
		TRACE_LOG("RohonDealer::cancelOrder failed. orderid:%s ����ֱ��ʧ��",order->getId().c_str());
		return;
	}
	return;
}

void RohonDealer::OnReturnRspOrderInsert(CRohonInputOrderField *pInputOrder)
{
	int ref = atoi(pInputOrder->OrderRef);
	//�ڲ��н��ײ����ǰ����,ͨ��ɨ��inventoryMap���refӳ��
	EnterCriticalSection(&cs);
	RohonDealerInventory* pInventory = findInventorybyLocal(ref);
	handleOrderInsertReport(pInventory,pInputOrder);
	LeaveCriticalSection(&cs);
}

void RohonDealer::OnReturnRspOrderAction(CRohonInputOrderActionField *pInputOrderAction)
{
	int ref = atoi(pInputOrderAction->OrderRef);
	//�ڲ��н��ײ����ǰ����,ͨ��ɨ��inventoryMap���refӳ��
	EnterCriticalSection(&cs);
	RohonDealerInventory* pInventory = findInventorybyLocal(ref);
	handleOrderActionReport(pInventory,pInputOrderAction);
	LeaveCriticalSection(&cs);
}

void RohonDealer::handleOrderInsertReport(RohonDealerInventory* pInventory, CRohonInputOrderField *pInputOrder)
{
	if(pInventory == NULL) return;
	pInventory->placeStatus = 5;
	TRACE_DEBUG("[RohonDealer���ػص�]������������,�ж�Ϊ��������.���㱨. ref=%d.",pInventory->localRef);
	pInventory->order->setRejected();
	delInventory(pInventory);
}

void RohonDealer::handleOrderActionReport(RohonDealerInventory* pInventory, CRohonInputOrderActionField *pInputOrder)
{
	if(pInventory == NULL) return;
	if(pInventory->placeStatus == 0) // new order without any response from remote server.
	{
		TRACE_DEBUG("[RohonDealer���ػص�]��������,�ж�Ϊԭ����δ������.���㱨�����ɹ�����ɾ���õ���¼. ref=%d.",pInventory->localRef);
		pInventory->order->setCancel();
		delInventory(pInventory);
	}
}

//���ػص�������,ref���ݴ���0�ı���״̬֪ͨ
//ͨ��refӳ���inventory,�ٸ���pOrder�����޸�inventory
void RohonDealer::OnReturnOrder(CRohonOrderField *pOrder)
{
	int ref = atoi(pOrder->OrderRef);
	//�ڲ��н��ײ����ǰ����,ͨ��ɨ��inventoryMap���refӳ��
	EnterCriticalSection(&cs);
	RohonDealerInventory* pInventory = findInventorybyLocal(ref);
	handleOrderStatusReport(pInventory,pOrder);
	LeaveCriticalSection(&cs);
}

//Ҫ�ֱ�������Ϣ:�µ��ɹ�(��¼orderSysID),�µ�ʧ��,�����ɹ�,����ʧ��.
/////////////////////////////////////////////////////////////////////////
///TFtdcOrderSubmitStatusType��һ�������ύ״̬����
/////////////////////////////////////////////////////////////////////////
///�Ѿ��ύ			#define ROHON_TRADE_OSS_InsertSubmitted '0'
///�����Ѿ��ύ		#define ROHON_TRADE_OSS_CancelSubmitted '1'
///�޸��Ѿ��ύ		#define ROHON_TRADE_OSS_ModifySubmitted '2'
///�Ѿ�����			#define ROHON_TRADE_OSS_Accepted '3'
///�����Ѿ����ܾ�	#define ROHON_TRADE_OSS_InsertRejected '4'
///�����Ѿ����ܾ�	#define ROHON_TRADE_OSS_CancelRejected '5'
///�ĵ��Ѿ����ܾ�	#define ROHON_TRADE_OSS_ModifyRejected '6'
//
/////////////////////////////////////////////////////////////////////////
///TFtdcOrderStatusType��һ������״̬����
/////////////////////////////////////////////////////////////////////////
///ȫ���ɽ�				#define ROHON_TRADE_OST_AllTraded '0'
///���ֳɽ����ڶ�����	#define ROHON_TRADE_OST_PartTradedQueueing '1'
///���ֳɽ����ڶ�����	#define ROHON_TRADE_OST_PartTradedNotQueueing '2'
///δ�ɽ����ڶ�����		#define ROHON_TRADE_OST_NoTradeQueueing '3'
///δ�ɽ����ڶ�����		#define ROHON_TRADE_OST_NoTradeNotQueueing '4'
///����					#define ROHON_TRADE_OST_Canceled '5'
///δ֪					#define ROHON_TRADE_OST_Unknown 'a'
///��δ����				#define ROHON_TRADE_OST_NotTouched 'b'
///�Ѵ���				#define ROHON_TRADE_OST_Touched 'c'
///���ύ				#define ROHON_TRADE_OST_Submitted 'd'
///�����޸�				#define ROHON_TRADE_OST_Amending 'm'
//
void RohonDealer::handleOrderStatusReport(RohonDealerInventory* pInventory, CRohonOrderField *pOrder)
{
	if(pInventory == NULL) return;
	TRACE_DEBUG("[RohonDealer���ػص�]����״̬֪ͨ��Ref:%s,OrderSysID:%s,SubmitStatus:%c,OrderStatus:%c.",
		pOrder->OrderRef,pOrder->OrderSysID,pOrder->OrderSubmitStatus,pOrder->OrderStatus);	
	printf("[RohonDealer���ػص�]����״̬֪ͨ��Ref:%s,OrderSysID:%s,SubmitStatus:%c,OrderStatus:%c.\n",
		pOrder->OrderRef,pOrder->OrderSysID,pOrder->OrderSubmitStatus,pOrder->OrderStatus);	

	if (pOrder->OrderSubmitStatus == ROHON_TRADE_OSS_CancelRejected)
	{//��������
		TRACE_DEBUG("[RohonDealer���ػص�]����״̬֪ͨ,�ж�Ϊ��������.");
	}
	else if (pOrder->OrderStatus == ROHON_TRADE_OST_Canceled)
	{//�����ɹ�
		pInventory->placeStatus = 4;
		TRACE_DEBUG("[RohonDealer���ػص�]����״̬֪ͨ,�ж�Ϊ�����ɹ�.���㱨.");
		pInventory->order->setCancel();
		delInventory(pInventory);
	}
	else if(pOrder->OrderSubmitStatus == ROHON_TRADE_OSS_InsertRejected)
	{//��������
		pInventory->placeStatus = 5;
		TRACE_DEBUG("[RohonDealer���ػص�]����״̬֪ͨ,�ж�Ϊ��������.���㱨.");
		pInventory->order->setRejected();
		delInventory(pInventory);
	}
	else
	{//�����Ǳ��������
		pInventory->remoteRef = pOrder->OrderSysID;
		if(pInventory->remoteRef != "" && pInventory->placeStatus == 0) //�±����Ѿ�������
			pInventory->placeStatus = 1;
		if (pOrder->OrderSubmitStatus == ROHON_TRADE_OSS_Accepted && pOrder->OrderStatus == ROHON_TRADE_OST_NoTradeQueueing)
		{//�ҵ������
			TRACE_DEBUG("[RohonDealer���ػص�]����״̬֪ͨ,�ж�Ϊ�ҵ�.");
		}
		else if (pOrder->OrderSubmitStatus == ROHON_TRADE_OSS_InsertSubmitted && pOrder->OrderStatus == ROHON_TRADE_OST_AllTraded)
		{//���̳ɽ������
			TRACE_DEBUG("[RohonDealer���ػص�]����״̬֪ͨ,�ж�Ϊȫ���ɽ�.");
		}
		else if (pOrder->OrderSubmitStatus == ROHON_TRADE_OSS_InsertSubmitted && pOrder->OrderStatus == ROHON_TRADE_OST_PartTradedQueueing)
		{//���ֳɽ����ֹҵ������
			TRACE_DEBUG("[RohonDealer���ػص�]����״̬֪ͨ,�ж�Ϊ���ֳɽ����ֹҵ�.");
		}
		else
		{//�������
			TRACE_DEBUG("[RohonDealer���ػص�]����״̬֪ͨ,�ж�Ϊ�������.");
		}
	}
}


//���ػص�������,���гɽ�֪ͨ
//ͨ��orderSysIDӳ���inventory,�ٸ���pTrade�����޸�inventory
void RohonDealer::OnTrade(CRohonTradeField *pTrade)
{
	//�ڲ��н��ײ����ǰ����,ͨ��ɨ��inventoryMap���orderSysIDӳ��
	EnterCriticalSection(&cs);
	RohonDealerInventory* pInventory = findInventorybyRemote(string(pTrade->OrderSysID));
	handleTradeReport(pInventory,pTrade);
	LeaveCriticalSection(&cs);
}


void RohonDealer::handleTradeReport(RohonDealerInventory* pInventory, CRohonTradeField *pTrade)
{
	if(pInventory == NULL) return;
	TRACE_LOG("[RohonDealer���ػص�]��ע�ĳɽ������ݣ�%s,ordersysid:%s,[%c,%c],%.3f,%d hand at %s %s.",
			pTrade->InstrumentID,pTrade->OrderSysID,
			pTrade->OffsetFlag,pTrade->Direction,
			pTrade->Price,pTrade->Volume,
			pTrade->TradeDate,pTrade->TradeTime);
	printf("[RohonDealer���ػص�]��ע�ĳɽ������ݣ�%s,ordersysid:%s,[%c,%c],%.3f,%d hand at %s %s.\n",
			pTrade->InstrumentID,pTrade->OrderSysID,
			pTrade->OffsetFlag,pTrade->Direction,
			pTrade->Price,pTrade->Volume,
			pTrade->TradeDate,pTrade->TradeTime);
	pInventory->dealedLot += pTrade->Volume;
	if(pInventory->dealedLot < pInventory->order->getLot())
	{//partial dealed.
		pInventory->placeStatus = 2;
		pInventory->order->addDetail(new OrderDetail(pInventory->order->getId(),pTrade->Volume,pTrade->Price));
		TRACE_LOG("[RohonDealer���ػص�]partial dealed,baseName:%s.",pInventory->order->getContract().c_str());
		printf("[RohonDealer���ػص�]partial dealed,baseName:%s.\n",pInventory->order->getContract().c_str());
	}
	else
	{//all dealed.
		pInventory->placeStatus = 3;
		pInventory->order->addDetail(new OrderDetail(pInventory->order->getId(),pTrade->Volume,pTrade->Price));
		TRACE_LOG("[RohonDealer���ػص�]all dealed,baseName:%s.",pInventory->order->getContract().c_str());
		printf("[RohonDealer���ػص�]all dealed,baseName:%s.\n",pInventory->order->getContract().c_str());
		delInventory(pInventory);
	}
}


//**********************class CROHONTraderSpi**************************
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CROHONTraderSpi::CROHONTraderSpi()
{
	SESSION_ID=0;
	FRONT_ID=0;
	iRequestID=0;
	iNextOrderRef=0;
	memset(m_OrderSysID,0,sizeof(m_OrderSysID));
}
CROHONTraderSpi::~CROHONTraderSpi()
{
}
//////////////////////////////////////////////////////////////////////
// CALLBACKS
//////////////////////////////////////////////////////////////////////
void CROHONTraderSpi::OnFrontConnected()
{
	TRACE_LOG("[RohonDealer���ػص�]IP�����ѽ���.");
	//request login.
	CRohonReqUserLoginField req;
	memset(&req, 0, sizeof(req));

	strcpy_s(req.UserID, 16, INVESTOR_ID);
	strcpy_s(req.Password, 41, PASSWORD);
	int iResult = m_pDealer->pUserApi->ReqUserLogin(&req, ++iRequestID);
	TRACE_LOG("[RohonDealer����]�����û���¼����: %s, %s.",INVESTOR_ID,((iResult == 0) ? "���ͳɹ�" : "����ʧ��"));
}

void CROHONTraderSpi::OnRspUserLogin(CRohonRspUserLoginField *pRspUserLogin, CRohonRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		// ����Ự����
		FRONT_ID = pRspUserLogin->FrontID;
		SESSION_ID = pRspUserLogin->SessionID;
		iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
		iNextOrderRef++;
		TRACE_LOG("[RohonDealer���ػص�]�û���¼�ɹ�: ������%s,ǰ�ñ��%d,�Ự���%d,��������%d.",
		m_pDealer->pUserApi->GetTradingDay(),FRONT_ID,SESSION_ID,iNextOrderRef);
		///Ͷ���߽�����ȷ��
		CRohonSettlementInfoConfirmField req;
		memset(&req, 0, sizeof(req));
		strcpy_s(req.InvestorID, 16, INVESTOR_ID);
		int iResult = m_pDealer->pUserApi->ReqSettlementInfoConfirm(&req, ++iRequestID);
		TRACE_LOG("[RohonDealer����]���ͽ�������: %s, %s.",INVESTOR_ID,((iResult == 0) ? "���ͳɹ�" : "����ʧ��"));
	}
	else
	{
		TRACE_LOG("[RohonDealer���ػص�]�û���¼ʧ��.");
	}
}

void CROHONTraderSpi::OnRspSettlementInfoConfirm(CRohonSettlementInfoConfirmField *pSettlementInfoConfirm, CRohonRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		TRACE_LOG("[RohonDealer���ػص�]����ɹ�.");
		::SetEvent(m_pDealer->hLogin);
	}
	else
	{
		TRACE_LOG("[RohonDealer���ػص�]����ʧ��.");
	}
}

void CROHONTraderSpi::OnRspQryInstrument(CRohonInstrumentField *pInstrument, CRohonRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (!IsErrorRspInfo(pRspInfo) && pInstrument!=NULL)
	{
		TRACE_LOG("[RohonDealer���ػص�]��ѯ���к�Լ�����%s, ��������%s, ��Լ��%s", pInstrument->InstrumentID, pInstrument->ExchangeID, pInstrument->InstrumentName);
		printf("[RohonDealer���ػص�]��ѯ���к�Լ�����%s, ��������%s, ��Լ��%s\n", pInstrument->InstrumentID, pInstrument->ExchangeID, pInstrument->InstrumentName);
	}
	if (bIsLast)
	{
		TRACE_LOG("[RohonDealer���ػص�]��ѯ���к�Լ��� ����");
		printf("[RohonDealer���ػص�]��ѯ���к�Լ��� ����\n");
	}
}

void CROHONTraderSpi::OnRspOrderInsert(CRohonInputOrderField *pInputOrder, CRohonRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	TRACE_DEBUG("[RohonDealer���ػص�]����Ӧ��.");
	if(IsErrorRspInfo(pRspInfo))
	{
		TRACE_LOG("[RohonDealer���ػص�]����Ӧ����������д�.");
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

void CROHONTraderSpi::OnRspOrderAction(CRohonInputOrderActionField *pInputOrderAction, CRohonRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	TRACE_DEBUG("[RohonDealer���ػص�]����Ӧ��.");
	if(IsErrorRspInfo(pRspInfo))
	{
		TRACE_LOG("[RohonDealer���ػص�]����Ӧ�������Ӧ�����ѳ����ѳɽ�.");
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
void CROHONTraderSpi::OnRtnOrder(CRohonOrderField *pOrder)
{
	if (!IsMyOrder(pOrder)) return;

	m_pDealer->OnReturnOrder(pOrder);
}

///�ɽ�֪ͨ,˽����
void CROHONTraderSpi::OnRtnTrade(CRohonTradeField *pTrade)
{
	m_pDealer->OnTrade(pTrade);
}

void CROHONTraderSpi:: OnFrontDisconnected(int nReason)
{
	TRACE_DEBUG("OnFrontDisconnected. Reason=%d.",nReason);
}
		
void CROHONTraderSpi::OnHeartBeatWarning(int nTimeLapse)
{
	TRACE_DEBUG("OnHeartBeatWarning. nTimerLapse=%d.",nTimeLapse);
}

bool CROHONTraderSpi::IsErrorRspInfo(CRohonRspInfoField *pRspInfo)
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
bool CROHONTraderSpi::IsMyOrder(CRohonOrderField *pOrder)
{
	int ref = 0;
	ref = atoi(pOrder->OrderRef);
	return ((pOrder->FrontID == FRONT_ID) &&
			(pOrder->SessionID == SESSION_ID) &&
			(ref != 0));
}

//���������ؽӿ�ʵ�ַ���
int CROHONTraderSpi::InsertOrder(int orderRef, Order *order)
{
	CRohonInputOrderField req;
	memset(&req, 0, sizeof(req));
	strcpy_s(req.InvestorID, 13, INVESTOR_ID);
	sprintf_s(req.OrderRef, 13, "%d", orderRef);
	req.OrderPriceType = ROHON_TRADE_OPT_LimitPrice;
	req.CombHedgeFlag[0] = ROHON_TRADE_HF_Speculation;
	req.TimeCondition = ROHON_TRADE_TC_GFD;

	// ���������ͳɽ�����ָʾ
	req.VolumeTotalOriginal = order->getLot();  //���α�����Ŀ������
	req.VolumeCondition = ROHON_TRADE_VC_AV;  //�޳ɽ������ҵ�
	req.MinVolume = 1;  //��С�ɽ���������ͬ��Ŀ������������һ����ΪROHON_TRADE_VC_MV�������ã������𱣻����壩

	req.ContingentCondition = ROHON_TRADE_CC_Immediately;
	req.ForceCloseReason = ROHON_TRADE_FCC_NotForceClose;
	req.IsAutoSuspend = 0;
	req.UserForceClose = 0;

	///��Լ����
	NameConverter* pNC = NameConverter::getInstance();
	string rohonName = pNC->base2RohonName(order->getContract());
	if(rohonName=="") 
	{
		TRACE_LOG("[CROHONTraderSpi::InsertOrder]Warning, NO RohonName found according to BaseName:%s.",order->getContract().c_str());
		return 1;
	}
	strcpy_s(req.InstrumentID, 81, rohonName.c_str());

	///��������: 
	if(order->getBuySell()==BUY)
		req.Direction = ROHON_TRADE_D_Buy;
	else
		req.Direction = ROHON_TRADE_D_Sell;
	///��Ͽ�ƽ��־: ����
	if(order->getKaiPing()==KAI_CANG)
		req.CombOffsetFlag[0] = ROHON_TRADE_OF_Open;
	else if (order->getKaiPing()==PING_CANG)
		req.CombOffsetFlag[0] = ROHON_TRADE_OF_Close;
	else
		req.CombOffsetFlag[0] = ROHON_TRADE_OF_CloseToday;
	///�۸�
	req.LimitPrice = order->getPrice();

	int iResult = m_pDealer->pUserApi->ReqOrderInsert(&req, ++iRequestID);
	TRACE_LOG("[RohonDealer����]����¼������: %s,[%c,%c],%.3f, lot:%d, %s.",
		req.InstrumentID,req.CombOffsetFlag[0],req.Direction,req.LimitPrice,req.VolumeTotalOriginal,
		((iResult == 0) ? "���ͳɹ�" : "����ʧ��"));
	return iResult;
}

//���������ؽӿ�ʵ�ַ���
int CROHONTraderSpi::CancelOrder(int orderRef, Order *order)
{
	CRohonInputOrderActionField req;
	memset(&req, 0, sizeof(req));
	strcpy_s(req.InvestorID, 13, INVESTOR_ID);

	NameConverter* pNC = NameConverter::getInstance();
	string rohonName = pNC->base2RohonName(order->getContract());
	if(rohonName=="") 
	{
		TRACE_LOG("[CROHONTraderSpi::CancelOrder]Warning, NO RohonName found according to BaseName:%s.",order->getContract().c_str());
		return 1;
	}
	strcpy_s(req.InstrumentID, 81, rohonName.c_str());
	
	sprintf_s(req.OrderRef, 13, "%d", orderRef); 

	req.FrontID = FRONT_ID;
	req.SessionID = SESSION_ID;
	req.ActionFlag = ROHON_TRADE_AF_Delete;

	int iResult = m_pDealer->pUserApi->ReqOrderAction(&req, ++iRequestID);
	TRACE_LOG("[RohonDealer����]������������: %s, %.3f, %s.",
		req.InstrumentID,req.LimitPrice,((iResult == 0) ? "���ͳɹ�" : "����ʧ��"));
	return iResult;
}

int CROHONTraderSpi::QueryAllInstrument()
{
	CRohonQryInstrumentField field;
	memset(&field, 0, sizeof(CRohonQryInstrumentField));
	int iResult = m_pDealer->pUserApi->ReqQryInstrument(&field, ++iRequestID);
	TRACE_LOG("[RohonDealer����]��ѯ���к�Լ��������: %s.",((iResult == 0) ? "���ͳɹ�" : "����ʧ��"));
	return iResult;

}

int CROHONTraderSpi::getCurOrderRef()
{
	int ret = iNextOrderRef;
	iNextOrderRef++;
	return ret;
}



