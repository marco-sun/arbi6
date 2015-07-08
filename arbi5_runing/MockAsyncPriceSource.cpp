// MockAsyncPriceSource.cpp: implementation of the MockAsyncPriceSource class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MockAsyncPriceSource.h"
#include <process.h>
#include "logstore.h"
#include "Contract.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MockAsyncPriceSource::MockAsyncPriceSource()
{
	InitializeCriticalSection(&cs);
	contracts.clear();
	index = 0;
}

MockAsyncPriceSource::~MockAsyncPriceSource()
{
	DeleteCriticalSection(&cs);
}

void MockAsyncPriceSource::start()
{
	//init price array with 60 steps
	double askbase = 1700.23;
	double bidbase = 1698.50;
	for(int i=0;i<60;i++)
	{
		ask[i] = askbase + i;
		bid[i] = bidbase + i;
	}
	
	//start thread
	_beginthread( threadPrice, 0, (void *)this);
}

void MockAsyncPriceSource::add(string contract)
{
	EnterCriticalSection(&cs);
	contracts.push_back(contract);
	LeaveCriticalSection(&cs);
}

void MockAsyncPriceSource::threadPrice( void* pParams ) 
{
	MockAsyncPriceSource* mainObj = (MockAsyncPriceSource*)pParams;
	 
	while(true)
	{
		mainObj->issuePrice();
		Sleep(500);
	}
	_endthread();
}


void MockAsyncPriceSource::issuePrice() 
{
	EnterCriticalSection(&cs);
	list<string>::iterator iter = contracts.begin();
	while(iter != contracts.end())
	{
		Contract::get(*iter)->setPrice(0,ask[index%60],bid[index%60]);
		index ++;
		iter++;
	}
	LeaveCriticalSection(&cs);
}
