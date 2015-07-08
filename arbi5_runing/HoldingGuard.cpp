// HoldingGuard.cpp: implementation of the HoldingGuard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HoldingGuard.h"
#include <assert.h>
#include "LogStore.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HoldingGuard::HoldingGuard()
{

}

HoldingGuard::~HoldingGuard()
{

}

void HoldingGuard::resetCount()
{
	this->holdingCounter->resetCount();
}

int HoldingGuard::getHoldingCount()
{
	return this->holdingCounter->getSuite();
}

void HoldingGuard::setHoldingCounter(HoldingCounter * holdingCounter)
{
	//printf("setHoldingCounter %p\n", holdingCounter);
	//delete this->holdingCounter;
    this->holdingCounter = holdingCounter;
}

void HoldingGuard::initHoldingCounter()
{
    this->holdingCounter = new HoldingCounter();
}

HoldingCounter * HoldingGuard::getHoldingCountInstance()
{
	return this->holdingCounter;
}

void HoldingGuard::setHoldingLimit(int low, int high)
{
	TRACE_LOG("setHoldingLimit,Low = %d , High = %d",low,high);
	assert(low >= 0);
	assert(low < high);
	holdingCounter->setLimit(low, high);
}

bool HoldingGuard::betweenLimit(int i)
{
	return holdingCounter->betweenLimit(i);
}

void HoldingGuard::updateCounter()
{
	if (holdingOperation == INCREASE)
	{
		holdingCounter->increase();
	} else {
        holdingCounter->decrease();
	}
}

void HoldingGuard::setHoldingOperation(enum HoldingOperation holdingOperation)
{
	this->holdingOperation = holdingOperation;
}

string HoldingGuard::limitString()
{
    char s1[25];
    char s2[25];

    _itoa_s(holdingCounter->getLowLimit(), s1, 25, 10);
    _itoa_s(holdingCounter->getHighLimit(), s2, 25, 10);

	string s = "";
	s.append(s1);
	s.append(" ");
	s.append(s2);
	s.append(holdingOperation == INCREASE ? " 0" : " 1");

//printf("limitString %i\n", holdingOperation);
	return s;
}

int HoldingGuard::getHighHoldingLimit()
{
	return holdingCounter->getHighLimit();
}

int HoldingGuard::getLowHoldingLimit()
{
	return holdingCounter->getLowLimit();
}