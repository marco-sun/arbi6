// HoldingCounter.cpp: implementation of the HoldingCounter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HoldingCounter.h"
#include "LogStore.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HoldingCounter::HoldingCounter()
{
    suite = 0;
	this->lowLimit = 0;
	this->highLimit = 1;
	InitializeCriticalSection(&cs);
}

HoldingCounter::~HoldingCounter()
{

}

void HoldingCounter::resetCount()
{
	suite = 0;
}
void HoldingCounter::add(int i)
{
	EnterCriticalSection(&cs);
	suite += i;
	TRACE_LOG("Holding count: %d", suite);
	LeaveCriticalSection(&cs);

}
void HoldingCounter::increase()
{
	add(1);
}

void HoldingCounter::decrease()
{
    add(-1);
}

int HoldingCounter::getSuite()
{
    EnterCriticalSection(&cs);
	int ret=suite;
	LeaveCriticalSection(&cs);
	return ret;
}

void HoldingCounter::setLimit(int low, int high)
{
	this->lowLimit = low;
	this->highLimit = high;
}

bool HoldingCounter::betweenLimit(int i)
{
	return (i >= lowLimit) && (i <= highLimit);
}

int HoldingCounter::getLowLimit()
{
	return lowLimit;
}

int HoldingCounter::getHighLimit()
{
	return highLimit;
}
