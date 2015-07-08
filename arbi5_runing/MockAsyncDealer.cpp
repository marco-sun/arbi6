// MockAsyncDealer.cpp: implementation of the MockAsyncDealer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MockAsyncDealer.h"
#include <process.h>
#include "logstore.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MockAsyncDealer::MockAsyncDealer()
{
	InitializeCriticalSection(&cs);
}

MockAsyncDealer::~MockAsyncDealer()
{
	DeleteCriticalSection(&cs);
}

//ÿ��һ���̴߳���
void MockAsyncDealer::threadDeal( void* pParams )
{
	DealerThreadParam* params = (DealerThreadParam*)pParams;
	MockAsyncDealer* mainObj = params->obj;
	string orderID = params->orderID;
	delete params;
	Sleep(500);
	TRACE_LOG("MockAsyncDealer is about to reportDeal, orderid=%s.",orderID.c_str());
	mainObj->reportDeal(orderID);
	_endthread();
}

void MockAsyncDealer::reportDeal(string orderID)
{
	int flag = -1;
	OrderDetail *od = NULL;
	Order *order = NULL;
	EnterCriticalSection(&cs);
	map<string, Inventory>::iterator the_iter = inventoryMap.find(orderID);
	if(the_iter != inventoryMap.end())
	{
		order = the_iter->second.order;
		od = new OrderDetail(order->getId(),order->getLot(),order->getPrice());
		if(the_iter->second.status == 0)
		{//�㱨�ɽ�
			flag = 0;
		}
		else
		{//�㱨����
			flag = 1;
		}
		inventoryMap.erase(the_iter);
	}
	LeaveCriticalSection(&cs);
	if(flag == 0)	order->addDetail(od);
	if(flag == 1)	order->setCancel();
}

void MockAsyncDealer::placeOrder(Order *order)
{
	TRACE_LOG("MockAsyncDealer placeOrder, orderid=%s.",order->getId().c_str());
	//step1: add to inventoryMap
	EnterCriticalSection(&cs);
	if(inventoryMap.count(order->getId())!=0)
	{//orderID�����ڴ����е��ظ���Ҫ�ܾ�����µ�����
		LeaveCriticalSection(&cs);
		TRACE_LOG("MockAsyncDealer placeOrder PLACE_ORDER_FAIL,orderid=%s.",order->getId().c_str());
		order->setRejected();
	}
	Inventory inventory;
	inventory.order = order;
	inventory.status = 0;
	inventoryMap.insert(make_pair(order->getId(),inventory));
	LeaveCriticalSection(&cs);
	
	//step2: �����߳�ģ��ص�����
	DealerThreadParam* param = new DealerThreadParam;
	param->orderID = order->getId();
	param->obj = this;
	_beginthread( threadDeal, 0, (void *)param);
	TRACE_LOG("MockAsyncDealer placeOrder PLACE_ORDER_OK.");
}

void MockAsyncDealer::cancelOrder(Order *order)
{
	TRACE_LOG("MockAsyncDealer cancelOrder, orderid=%s.",order->getId().c_str());
	EnterCriticalSection(&cs);
	map<string, Inventory>::iterator iter = inventoryMap.find(order->getId());
	if(iter != inventoryMap.end())
	{
		if(iter->second.status == 0)
		{//ֻ�й��Ż�δ�ɽ����ܳ���
			iter->second.status = 1;
		}
	}
	LeaveCriticalSection(&cs);
}