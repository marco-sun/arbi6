// CancelQueue.cpp: implementation of the CancelQueue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CancelQueue.h"
#include "Dealer.h"
#include "LogStore.h"
#include <iostream>

#define CHECK_INTERVAL 1 * 1000



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CancelQueue::CancelQueue()
{
	InitializeCriticalSection(&cs);
	this->threadStarted = false;
}

CancelQueue::~CancelQueue()
{

}

void  cancelFunction( void* pParams )
{
	CancelQueue * cq = (CancelQueue *) pParams;
	while(1)
	{
		cq->checkQueue();
		Sleep(CHECK_INTERVAL);
	}
}

void CancelQueue::startThreadAsNecessary()
{
    if(!threadStarted)
	{
		TRACE_LOG("start CancelQueue cancelFunction");
		_beginthread(cancelFunction, 0, (void *)this);
	}
	threadStarted = true;
}

void CancelQueue::add(Order * o)
{
	EnterCriticalSection(&cs);
	TRACE_LOG("CancelQueue::add order");
	if(o->shouldCancel())
	{
		Dealer::cancel(o);
	}
	orders.push_back(o);
	startThreadAsNecessary();
	LeaveCriticalSection(&cs);
}

void CancelQueue::checkQueue()
{
	EnterCriticalSection(&cs);
    list<Order *>::iterator the_iterator = orders.begin();

    while( the_iterator != orders.end() ) {
		Order * o = (*the_iterator);
        if(o->isFinish())
		{
            the_iterator = orders.erase(the_iterator);
		} else if (o->shouldCancel()){
			Dealer::cancel(o);
			o->increaseCancelNumber();
            the_iterator++;
		} else {
			the_iterator++;
		}
	}
	LeaveCriticalSection(&cs);
}

CancelQueue * CancelQueue::instance = NULL;

CancelQueue * CancelQueue::getInstance()
{
	if(instance == NULL) {
		instance = new CancelQueue();
	}
	return instance;
}

void CancelQueue::clear()
{
    orders.clear();
}

int CancelQueue::size()
{
	return orders.size();
}
