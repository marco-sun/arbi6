#include "CatsDealer.h"
#include "../logstore.h"
//#include "Contract.h"
#include "CatsApiEngine.h"
//#include <time.h>

CatsDealer::CatsDealer(void)
{
	isStarted = false;
}

CatsDealer::~CatsDealer(void)
{
}

void CatsDealer::ReadIniConfig()
{ // ��¼��Ҫ�Ĳ���
}

void CatsDealer::start()
{
	// �������뱣��
	if(isStarted) return;
	// ��ȡ��¼����
	ReadIniConfig();
	CatsApiEngine* pEngine = CatsApiEngine::getEngineInstance();
    TRACE_LOG("CatsDealer started.");	
	isStarted = true;
}

void CatsDealer::placeOrder(Order *order)
{
	TRACE_LOG("CatsDealer::placeOrder, orderid:%s.",order->getId().c_str());
	if(!isStarted)
	{
		TRACE_LOG("CatsDealer::placeOrder failed. CatsDealer not started.");
		order->setRejected();
		return;
	}
	CatsApiEngine* pEngine = CatsApiEngine::getEngineInstance();
	if(pEngine) 
	{
		pEngine->placeOrder(order);
	}
	else
	{//�µ�ֱ��ʧ��
		TRACE_LOG("CatsDealer::placeOrder failed. �µ�ֱ��ʧ�� can NOT find the Engine");
		order->setRejected();
	}
	return;
}

void CatsDealer::cancelOrder(Order *order)
{
	// �鿴order״̬���Ƿ���Ҫ����
	if(order->isCanceled()||order->isDealed()||order->isRejected())
	{
		TRACE_LOG("CatsDealer::cancelOrder failed. orderid:%s is not in right status.",order->getId().c_str());
		return;
	}
	CatsApiEngine* pEngine = CatsApiEngine::getEngineInstance();
	if(pEngine) 
	{
		pEngine->cancelOrder(order);
	}
	else
	{//����ֱ��ʧ��
		TRACE_LOG("CatsDealer::cancelOrder failed. ����ֱ��ʧ�� can NOT find the Engine");
	}

}
