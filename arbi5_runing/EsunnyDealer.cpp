#include "stdafx.h"
#include "EsunnyDealer.h"
#include "Contract.h"
#include "LogStore.h"
#include <process.h>

// ���ò���,���������ļ��ж���
static TEsAddressField ADDR;
static TEsLoginReqField LOGIN;

static const char* g_szStateDesc[] = {"��Ч��", "�µ�", "���Ŷ�", "��ȫ�ɽ�", "��������", "�ѳ���", "��", 
            "��ɾ��", "�ѷ���", "����", "���ֳɽ�", "�ǽ���ʱ��", "�ʽ���", "���׹ر�", 
            "ָ��ʧ��", "����ʧ��", "ϵͳ����", "������", "������ɾ��", "����", "����", "������", 
            "�Զ���", "�Զ���ɾ��", "δ֪"};


static EsunnyDealer* sp_esDealer = NULL;
//******************class EsunnyDealerInventory**********************************
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
EsunnyDealerInventory::EsunnyDealerInventory(Order *pOrder)
{
	order = pOrder;
	localRef = -1;
	remoteRef = "";
	systemNo = "";
	placeStatus = 0;
	dealedLot = 0;
}

EsunnyDealerInventory::~EsunnyDealerInventory()
{
}

//******************class EsunnyDealer**********************************
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
EsunnyDealer::EsunnyDealer(void)
{
	started = false;
	hLogin = ::CreateEvent(NULL,TRUE,FALSE,NULL);
	ReadIniConfig();
	InitializeCriticalSection(&cs);
}


EsunnyDealer::~EsunnyDealer(void)
{
	if(started)	stop();
	DeleteCriticalSection(&cs);
	::CloseHandle(hLogin);
}

////////////////////////////////////////
/// \fn    GetOrderStateDesc
/// \brief ��ȡ״̬����
/// \param int iState
/// \return   int
////////////////////////////////////////
const char* EsunnyDealer::GetOrderStateDesc(unsigned int iState)
{
    if (iState > sizeof(g_szStateDesc)/sizeof(*g_szStateDesc))
    {
        iState = sizeof(g_szStateDesc)/sizeof(*g_szStateDesc) - 1;
    }
    return g_szStateDesc[iState];
}

EsunnyDealer* EsunnyDealer::getInstance()
{
	if(sp_esDealer == NULL)
		sp_esDealer = new EsunnyDealer();
	return sp_esDealer;
}

//////////////////////////////////////////////////////////////////////
// Inventory Operations
//////////////////////////////////////////////////////////////////////
EsunnyDealerInventory* EsunnyDealer::addInventory(Order *order)
{
	EsunnyDealerInventory* ret = NULL;
	string key = order->getId();
	//���Ƿ����ظ���key
	if(dealerInventoryMap.count(key)!=0)
	{
		return ret;
	}
	ret = new EsunnyDealerInventory(order);
	dealerInventoryMap.insert(make_pair(key,ret));
	return ret;
}

bool EsunnyDealer::delInventory(EsunnyDealerInventory* pDealerInventory)
{
	dealerInventoryMap.erase(pDealerInventory->order->getId());
	delete pDealerInventory;
	return TRUE;
}

EsunnyDealerInventory* EsunnyDealer::getInventory(string key)
{
	EsunnyDealerInventory* ret = NULL;
	map<string, EsunnyDealerInventory*>::iterator iter = dealerInventoryMap.find(key);
	if(iter == dealerInventoryMap.end()) return ret;
	return iter->second;
}

EsunnyDealerInventory* EsunnyDealer::findInventorybyLocal(int localRef)
{
	EsunnyDealerInventory* ret = NULL;
	map<string, EsunnyDealerInventory*>::iterator iter = dealerInventoryMap.begin();
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

EsunnyDealerInventory* EsunnyDealer::findInventorybyRemote(string remoteRef)
{
	EsunnyDealerInventory* ret = NULL;
	map<string, EsunnyDealerInventory*>::iterator iter = dealerInventoryMap.begin();
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

EsunnyDealerInventory* EsunnyDealer::findInventorybySys(string sysNo)
{
	EsunnyDealerInventory* ret = NULL;
	map<string, EsunnyDealerInventory*>::iterator iter = dealerInventoryMap.begin();
	while(iter != dealerInventoryMap.end())
	{
		if(iter->second->systemNo == sysNo)
		{
			ret = iter->second;
			break;
		}
		iter ++;
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////
// Configuration
//////////////////////////////////////////////////////////////////////
void EsunnyDealer::ReadIniConfig()
{
	char szFrontAddr[16];
	int nPort;
	char szInvestorId[16];
	char szPassword[16];
	memset(szFrontAddr,  0, sizeof(szFrontAddr));
	memset(szInvestorId, 0, sizeof(szInvestorId));
	memset(szPassword,   0, sizeof(szPassword));

	memset(&LOGIN,0,sizeof(LOGIN));

	if(GetPrivateProfileString("ESUNNY", "IP", "222.88.40.170", szFrontAddr, 16, ".\\AIB.ini"))
	{
		strncpy_s(ADDR.Ip, sizeof(ADDR.Ip), szFrontAddr, sizeof(ADDR.Ip) - 1);
	}
	nPort = GetPrivateProfileInt("ESUNNY", "PORT", 6060, ".\\AIB.ini");
	ADDR.Port = nPort;
	
	LOGIN.LoginType = 'C';
	if(GetPrivateProfileString("ESUNNY", "LOGIN_NO",  "SSS", szInvestorId, 16, ".\\AIB.ini"))
	{
		strncpy_s(LOGIN.LoginNo, sizeof(LOGIN.LoginNo), szInvestorId, sizeof(LOGIN.LoginNo) - 1);
	}
	if(GetPrivateProfileString("ESUNNY", "PASSWORD", "sss123456", szPassword, 16, ".\\AIB.ini"))
	{
		strncpy_s(LOGIN.Password, sizeof(LOGIN.Password), szPassword, sizeof(LOGIN.Password) - 1);
	}
}

//////////////////////////////////////////////////////////////////////
// Start & Stop
//////////////////////////////////////////////////////////////////////
//ͬ����start,�ȵ�¼���ŷ���
void EsunnyDealer::start()
{
	bool bRet;
    if(started) {
	    TRACE_LOG("EsunnyDealer already started.");
		return;
	}
	// 1. ������֤
	bRet = CertEsunnyApi("4A35B623174363F3A1BDD4D66D1F66AC7A368C423BB68CAFD0C78CDBA812DAAD9AAD1C1052E60447B35F0E2237A83160554F", "");	
    if (!bRet)
    {
       TRACE_ALARM("EsunnyDealer Starts failed! [Cert FAIL!]");
		exit(1);
    }
	// 2. ��ȡ������ʵ��
	pUserSpi = new EsunnyDealerSpi();
	if((pUserApi=CreateEsunnyTradeApi()) == NULL)
	{
		TRACE_ALARM("EsunnyDealer Starts failed! [Create API FAIL!]");
		exit(1);
	}
	pUserSpi->m_pDealer = this;
	pUserApi->SetSpi((IEsunnyTradeSpi*)pUserSpi);			// ע���¼���

	// 3. ��¼����
	::ResetEvent(hLogin);
	bRet = pUserApi->Open(ADDR);
	if (!bRet)
    {
       TRACE_ALARM("EsunnyDealer Starts failed! [pUserApi->Open(ADDR) FAIL!]");
		exit(1);
    }
	// 4. wait for login event signaled. ::SetEvent(hLogin)
	DWORD dwRet = WaitForSingleObject(hLogin,10*1000); //wait 10 secs at most.
	if (dwRet == WAIT_TIMEOUT)
	{//������ʱ���Ǹ澯��־���˳�����
		TRACE_ALARM("EsunnyDealer Starts failed! [WAIT_TIMEOUT]");
		exit(1);
	}
	else if (dwRet == WAIT_OBJECT_0)
	{//��������
		TRACE_LOG("EsunnyDealer Starts OK.");
	    started = true;		
		return;
	}
	else //WAIT_ABANDONED
	{
		TRACE_ALARM("EsunnyDealer Starts failed! [WAIT_ABANDONED]");
		exit(1);
	}
}

void EsunnyDealer::stop()
{
	if(pUserApi)
	{
		if(pUserApi->IsOpen()) pUserApi->Close();
		pUserApi->Free();
		pUserApi=NULL;
	}
	if(pUserSpi)
	{
		delete pUserSpi;
		pUserSpi = NULL;
	}
	started = false;
}

//////////////////////////////////////////////////////////////////////
// Place & Cancel Orders
//////////////////////////////////////////////////////////////////////
void EsunnyDealer::placeOrder(Order *order)
{
	int ref;
	//����map��,ִ����Ӧ����,�����ȴ��߳�
	TRACE_LOG("EsunnyDealer::placeOrder, orderid:%s.",order->getId().c_str());
	if(!started)
	{
		TRACE_LOG("EsunnyDealer::placeOrder failed. EsunnyDealer not started.");
		order->setRejected();
		return;
	}
	//����map��
	EnterCriticalSection(&cs);
	EsunnyDealerInventory* pInventory = addInventory(order);
	if(pInventory == NULL)
	{//orderID�����ڴ����е��ظ���Ҫ�ܾ�����µ�����
		LeaveCriticalSection(&cs);
		TRACE_LOG("EsunnyDealer::placeOrder failed. orderID is duplicated.");
		return;
	}
	//����localRef
	ref = pUserSpi->GenerateReqID();
	pInventory->localRef = ref;
	LeaveCriticalSection(&cs);
	//ִ���µ�����
	char exchangeCode = Contract::get(order->getContract())->getExchangeNumberForYiSheng();
	if(0 != pUserSpi->InsertOrder(ref,exchangeCode,order))
	{//�µ�ֱ��ʧ��
		EnterCriticalSection(&cs);
		delInventory(pInventory);
		LeaveCriticalSection(&cs);
		TRACE_LOG("EsunnyDealer::placeOrder failed. �µ�ֱ��ʧ��");
		order->setRejected();
		return;
	}
	return;
}

void EsunnyDealer::cancelOrder(Order *order)
{
	//�޸�map����Ӧ���ӵ�״̬��ִ����Ӧ����
	string orderNo="";
	string sysNo="";
	TRACE_LOG("EsunnyDealer::cancelOrder, orderid:%s.",order->getId().c_str());
	if(!started)
	{
		TRACE_LOG("EsunnyDealer::cancelOrder failed. EsunnyDealer not started.");
		return;
	}
	//�鿴order״̬���Ƿ���Ҫ����
	if(order->isCanceled()||order->isDealed()||order->isRejected())
	{
		TRACE_LOG("EsunnyDealer::cancelOrder failed. orderid:%s is not in right status.",order->getId().c_str());
		return;
	}
	//�Ӳֿ����ó�ί�кź�ϵͳ��
	EnterCriticalSection(&cs);
	EsunnyDealerInventory* pInventory = getInventory(order->getId());
	if(pInventory==NULL)
	{
		LeaveCriticalSection(&cs);
		TRACE_LOG("EsunnyDealer::cancelOrder failed. orderid:%s is not in this dealer.",order->getId().c_str());
		return;
	}
	orderNo = pInventory->remoteRef;
	sysNo = pInventory->systemNo;
	LeaveCriticalSection(&cs);
	//ִ����Ӧ����
	char exchangeCode = Contract::get(order->getContract())->getExchangeNumberForYiSheng();
	if(0 != pUserSpi->CancelOrder(exchangeCode,orderNo,sysNo,order))
	{
		TRACE_LOG("EsunnyDealer::cancelOrder failed. orderid:%s ����ֱ��ʧ��",order->getId().c_str());
		return;
	}
	return;
}


//���ػص������ı���Ӧ��
//ͨ��������refӳ���inventory,
//�ٸ���pOrder�����޸�inventory
void EsunnyDealer::OnReturnQryOrder(TEsOrderInsertRspField* pRsp)
{
	int reqId = pRsp->ReqData.RequestID;
	string orderNo = pRsp->OrderNo;
	EnterCriticalSection(&cs);
	EsunnyDealerInventory* pInventory = findInventorybyLocal(reqId);
	if(pInventory!=NULL)
	{
		pInventory->remoteRef = orderNo;
		TRACE_DEBUG("[EsunnyTrade���ػص�]������Ӧ�����������:%d,����ί�к�:%s,״̬:%s,������Ϣ:%s.",
			pRsp->ReqData.RequestID,pRsp->OrderNo,GetOrderStateDesc(pRsp->OrderState),pRsp->OrderEcho);	
		if(pRsp->OrderState == ORDER_INVALID || pRsp->OrderState == ORDER_NON_TRADING_HOURS || pRsp->OrderState == ORDER_INSUFFICIENT_FUNDS)
		{//��������
			pInventory->placeStatus = 5;
			TRACE_DEBUG("[EsunnyTrade���ػص�]������Ӧ,�ж�Ϊ��������.���㱨.");
			pInventory->order->setRejected();
			delInventory(pInventory);
		}
	}
	LeaveCriticalSection(&cs);
}

//���ػص������ı���״̬֪ͨ
//ͨ��ί�к�remoterefӳ���inventory
//�ٸ���pOrder�����޸�inventory
void EsunnyDealer::OnReturnOrder(TEsOrderInsertRspField& rsp)
{
	string orderNo = rsp.OrderNo;;
	
	EnterCriticalSection(&cs);
	EsunnyDealerInventory* pInventory = findInventorybyRemote(orderNo);
	handleOrderStatusReport(pInventory,rsp);
	LeaveCriticalSection(&cs);
}

//���ػص������ĳɽ�֪ͨ
//ͨ��ϵͳ��ӳ���inventory,�ٸ���pOrder�����޸�inventory
void EsunnyDealer::OnTrade(TEsMatchRspField& rsp)
{
	string sysNo = rsp.SystemNo;
	EnterCriticalSection(&cs);
	EsunnyDealerInventory* pInventory = findInventorybySys(sysNo);
	handleTradeReport(pInventory,rsp);
	LeaveCriticalSection(&cs);
}


//ί��״̬ 0-��Ч��,1-�µ�,2-���Ŷ�,3-��ȫ�ɽ�,4-��������,5-�ѳ���,6-��,7-��ɾ��,
//8-�ѷ���, 9-����,10-���ֳɽ�,11-�ǽ���ʱ��,12-�ʽ���,13-���׹ر�,14-ָ��ʧ��,
//15-����ʧ��,16-ϵͳ����, 17-������,18-������ɾ��,19-����,20-����,21-������,22-�Զ���,23-�Զ���ɾ�� 
void EsunnyDealer::handleOrderStatusReport(EsunnyDealerInventory* pInventory, TEsOrderInsertRspField& rsp)
{
	if(pInventory == NULL) return;
	TRACE_DEBUG("[EsunnyTrade���ػص�]����״̬֪ͨ������:%s,״̬:%s,������Ϣ:%s.",
		rsp.OrderNo,GetOrderStateDesc(rsp.OrderState),rsp.OrderEcho);		

	if (rsp.OrderState == ORDER_WITHDRAWALS_FAILED)
	{//��������
		TRACE_DEBUG("[EsunnyTrade���ػص�]����״̬֪ͨ,�ж�Ϊ��������.");
	}
	else if (rsp.OrderState == ORDER_CANCELED)
	{//�����ɹ�
		pInventory->placeStatus = 4;
		TRACE_DEBUG("[EsunnyTrade���ػص�]����״̬֪ͨ,�ж�Ϊ�����ɹ�.���㱨.");
		pInventory->order->setCancel();
		delInventory(pInventory);
	}
	else if(rsp.OrderState == ORDER_INVALID || rsp.OrderState == ORDER_NON_TRADING_HOURS || rsp.OrderState == ORDER_INSUFFICIENT_FUNDS)
	{//��������
		pInventory->placeStatus = 5;
		TRACE_DEBUG("[EsunnyTrade���ػص�]����״̬֪ͨ,�ж�Ϊ��������.���㱨.");
		pInventory->order->setRejected();
		delInventory(pInventory);
	}
	else
	{//�����Ǳ��������
		if(pInventory->systemNo == "") pInventory->systemNo = rsp.SystemNo;

		if(rsp.OrderState == ORDER_NEW) //�±����Ѿ�������
		{
			TRACE_DEBUG("[EsunnyTrade���ػص�]����״̬֪ͨ,�ж�Ϊ�±����Ѿ�������.");
			pInventory->placeStatus = 1;
		}
		else if (rsp.OrderState == ORDER_IN_QUEUE)
		{//�ҵ������
			TRACE_DEBUG("[EsunnyTrade���ػص�]����״̬֪ͨ,�ж�Ϊ�ҵ�.");
		}
		else if (rsp.OrderState == ORDER_ALL_MATCH)
		{//���̳ɽ������
			TRACE_DEBUG("[EsunnyTrade���ػص�]����״̬֪ͨ,�ж�Ϊȫ���ɽ�.");
		}
		else if (rsp.OrderState == ORDER_PART_MATCH)
		{//���ֳɽ����ֹҵ������
			TRACE_DEBUG("[EsunnyTrade���ػص�]����״̬֪ͨ,�ж�Ϊ���ֳɽ����ֹҵ�.");
		}
		else
		{//�������
			TRACE_DEBUG("[EsunnyTrade���ػص�]����״̬֪ͨ,�ж�Ϊ�������.");
		}
	}
}

void EsunnyDealer::handleTradeReport(EsunnyDealerInventory* pInventory, TEsMatchRspField& rsp)
{
	if(pInventory == NULL) return;
	TRACE_LOG("[EsunnyTrade���ػص�]��ע�ĳɽ���orderID:%s,sysNo:%s,�� %.3f �ɽ� %d ��, ʱ�� %s.",
		pInventory->order->getId().c_str(),rsp.SystemNo,rsp.MatchPrice,rsp.MatchVol,rsp.MatchTime);

	pInventory->dealedLot += rsp.MatchVol;
	if(pInventory->dealedLot < pInventory->order->getLot())
	{//partial dealed.
		pInventory->placeStatus = 2;
		pInventory->order->addDetail(new OrderDetail(pInventory->order->getId(),rsp.MatchVol,rsp.MatchPrice));
	}
	else
	{//all dealed.
		pInventory->placeStatus = 3;
		pInventory->order->addDetail(new OrderDetail(pInventory->order->getId(),rsp.MatchVol,rsp.MatchPrice));
		delInventory(pInventory);
	}
}


//////////////////////////////////////////////////////////////////////
// EsunnyDealerSpi
//////////////////////////////////////////////////////////////////////
void EsunnyDealerSpi::Convert2Uppercase(char content[], int length)
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
//���������ؽӿ�ʵ�ַ���
int EsunnyDealerSpi::InsertOrder(int orderRef, char exchangeCode, Order *order)
{
	 // �µ�����
    TEsOrderInsertReqField req;
    memset(&req, 0, sizeof(req));
	///��Լ����
	req.Exchange = exchangeCode;
    strncpy_s(req.Contract1, sizeof(req.Contract1), order->getContract().c_str(), sizeof(req.Contract1) - 1);
	Convert2Uppercase(req.Contract1,sizeof(req.Contract1));
	///��������: '1'-����,'3'-���� 
	if(order->getBuySell()==BUY)
		req.Direct = '1';
	else
		req.Direct = '3';
    ///��Ͽ�ƽ��־: '1'-����,'2'-ƽ��(������ƽ��),'3'-ƽ��(������ƽ��) 
	if(order->getKaiPing()==KAI_CANG)
		req.Offset = '1';
	else if (order->getKaiPing()==PING_CANG)
		req.Offset = '3';
	else
		req.Offset = '2';

    req.OrderPrice  = order->getPrice(); // �۸�
    req.OrderVol    = order->getLot();   //���α�����Ŀ������
    req.RequestID   = orderRef;
	/// �̶�����
	req.Hedge       = '1'; //Ͷ����ʶ '1'-Ͷ��,'3'-��ֵ 
    req.CmbType     = 0;    //������� 0-����,1-����,2-��Ʒ�� 
    req.OrderType   = 0;    //ί������ 0-�޼�,1-�м�

	int iResult = m_pDealer->pUserApi->OrderInsert(req);

	TRACE_LOG("Exchange:%c, RequestID: %d \n",req.Exchange, req.RequestID);
	TRACE_LOG("[EsunnyTrade����]����¼������: reqid:%d, %s,[%c,%c],%.3f, lot:%d, %s.",
		orderRef,req.Contract1,req.Offset,req.Direct,req.OrderPrice,req.OrderVol,
		((iResult == 0) ? "���ͳɹ�" : "����ʧ��"));
	return iResult;
}

//���������ؽӿ�ʵ�ַ���
int EsunnyDealerSpi::CancelOrder(char exchangeCode, string orderNo, string sysNo, Order *order)
{
	TEsOrderActionReqField req;
	memset(&req, 0, sizeof(req));
	// ���ڽ���ί�к�,ϵͳ�źͺ�Լ��,������
	strncpy_s(req.OrderNo, sizeof(req.OrderNo),
            orderNo.c_str(), sizeof(req.OrderNo) - 1);
	strncpy_s(req.SystemNo, sizeof(req.SystemNo),
            sysNo.c_str(), sizeof(req.SystemNo) - 1);
	strncpy_s(req.Contract, sizeof(req.Contract),
            order->getContract().c_str(), sizeof(req.Contract) - 1);
	Convert2Uppercase(req.Contract,sizeof(req.Contract));
	req.Exchange = exchangeCode;
	
	int iResult = m_pDealer->pUserApi->OrderAction(req);
	TRACE_LOG("[EsunnyTrade����]������������: ��Լ��%s, ί�к�%s, ϵͳ��%s, %s.",
		req.Contract, req.OrderNo,req.SystemNo,((iResult == 0) ? "���ͳɹ�" : "����ʧ��"));
	return iResult;
}


void __cdecl EsunnyDealerSpi::OnOpen()
{
	m_iConnState = STATE_CONNECTED;
	TRACE_LOG("[EsunnyTrade���ػص�]IP�����ѽ���.");
	// start logon
	int iRet = m_pDealer->pUserApi->Login(LOGIN);
	TRACE_LOG("[EsunnyTrade����]�����û���¼����: %s,%s.",
		LOGIN.LoginNo,LOGIN.Password,((iRet == 0) ? "���ͳɹ�" : "����ʧ��"));
}

void __cdecl EsunnyDealerSpi::OnClose()
{
	m_iConnState = STATE_DISCONNECTED; 
	TRACE_LOG("[EsunnyTrade���ػص�]IP�����ж�.");
}

void __cdecl EsunnyDealerSpi::OnLogin(TEsRspField& rsp)
{
	if(rsp.ErrorCode==0)
	{
		m_iConnState = STATE_LOGIN;
		TRACE_LOG("[EsunnyTrade���ػص�]�û���¼�ɹ�.");
		::SetEvent(m_pDealer->hLogin);
	}
	else
	{
		TRACE_LOG("[EsunnyTrade���ػص�]�û���¼ʧ��:[code=%d,info=%s]",rsp.ErrorCode,rsp.ErrorInfo);
	}

	// �ɹ���¼���׷�����������������������
	/*
	TEsAddressField addr1;
        strncpy_s(addr1.Ip, sizeof(addr1.Ip), "123.15.58.21", sizeof(addr1.Ip) - 1);
        addr1.Port = 6161;

        bool bRet = g_oQuoteApi.m_Api->Open(addr1);
        if (!bRet)
        {
            EnterCriticalSection(&g_CrtalSec);
            std::cout << "���������������������ʧ�ܣ�" << std::endl;
            LeaveCriticalSection(&g_CrtalSec);
        }
	*/
}

void __cdecl EsunnyDealerSpi::OnQryOrder(TEsOrderInsertRspField* pRsp, TEsIsLastType islast)
{
	// ***********Begin of for test only **********
	char szOrderNo[22];
	memset(szOrderNo,0,22);
	strncpy_s(szOrderNo,21,pRsp->OrderNo,21);
	TRACE_DEBUG("[EsunnyTrade���ػص�����]OnQryOrder��ڴ� �����:%d ��Ӧ��ί�к�:%s,״̬:%s.",
		 pRsp->ReqData.RequestID, szOrderNo ,EsunnyDealer::GetOrderStateDesc(pRsp->OrderState));
	// *********** End of for test only **********
	m_pDealer->OnReturnQryOrder(pRsp);
}

void __cdecl EsunnyDealerSpi::OnRtnOrder(TEsOrderInsertRspField& rsp)
{
	// ***********Begin of for test only **********
	char szOrderNo[22];
	memset(szOrderNo,0,22);
	strncpy_s(szOrderNo,21,rsp.OrderNo,21);
	char szSysNo[22];
	memset(szSysNo,0,22);
	strncpy_s(szSysNo,21,rsp.SystemNo,21);
	char szErrInfo[52];
	memset(szErrInfo,0,52);
	strncpy_s(szErrInfo,51,rsp.OrderEcho,51);
	TRACE_DEBUG("[EsunnyTrade���ػص�����]����״̬֪ͨ��ڴ� ί�к�:%s,�����:%d,ϵͳ��:%s,״̬:%s,������Ϣ:%s.",
		szOrderNo,rsp.ReqData.RequestID,szSysNo,EsunnyDealer::GetOrderStateDesc(rsp.OrderState),szErrInfo);
	// *********** End of for test only **********
	m_pDealer->OnReturnOrder(rsp);
}

void __cdecl EsunnyDealerSpi::OnRtnMatch(TEsMatchRspField& rsp)
{
	// ***********Begin of for test only **********
	char szSysNo[22];
	memset(szSysNo,0,22);
	strncpy_s(szSysNo,21,rsp.SystemNo,21);
	char szContract[22];
	memset(szContract,0,22);
	strncpy_s(szContract,21,rsp.Contract,21);
	TRACE_DEBUG("[EsunnyTrade���ػص�����]�ɽ�֪ͨ��ڴ� ϵͳ��:%s,������:%c,��Լ:%s,�۸�:%.3f,����:%d.",
		szSysNo,rsp.Exchange,szContract,rsp.MatchPrice,rsp.MatchVol);
	// *********** End of for test only **********
	m_pDealer->OnTrade(rsp);
}
