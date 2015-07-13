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
{ // 登录需要的参数
}

void CatsDealer::start()
{
	// 不可重入保护
	if(isStarted) return;
	// 读取登录配置
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
	{//下单直接失败
		TRACE_LOG("CatsDealer::placeOrder failed. 下单直接失败 can NOT find the Engine");
		order->setRejected();
	}
	return;
}

void CatsDealer::cancelOrder(Order *order)
{
	// 查看order状态，是否需要撤单
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
	{//撤单直接失败
		TRACE_LOG("CatsDealer::cancelOrder failed. 撤单直接失败 can NOT find the Engine");
	}

}
