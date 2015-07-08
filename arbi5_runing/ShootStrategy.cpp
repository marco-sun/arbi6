// ShootStrategy.cpp: implementation of the ShootStrategy class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ShootStrategy.h"
#include <assert.h>
#include <iostream>
#include "LogStore.h"

#include <sstream>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "urlmon.lib")
#include "alert\Alert.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ShootStrategy::ShootStrategy()
{
}

ShootStrategy::~ShootStrategy()
{

}


void ShootStrategy::trig(time_t t)
{
	//TRACE_LOG("strategyStatus : %d , twoLegStatus : %d  ", strategyStatus, twoLegStatus);

	startStopControl(t);
	if(strategyStatus == STOP || strategyStatus == HALT) return;

	switch(this->twoLegStatus)
	{
		case READY:
			readyAction();
			break;
		case AFTER_FIRST_SHOOT:
			afterFirstShootAction();
			break;
		case AFTER_SECOND_SHOOT:
			afterSecondShootAction();
			break;
	}	

}

//TODO: void ShootStrategy::trigAnyTime() to make test pass any time

void ShootStrategy::trig()
{
	trig(time(NULL));
}


void ShootStrategy::afterFirstShootAction()
{
    if(firstOrder->getStatus() == FINISHED)
	{
        secondOrder = this->secondLeg.shoot(5);
		this->twoLegStatus = AFTER_SECOND_SHOOT;
		TRACE_LOG("ShootStrategy:: SecondLeg shooted.");
	} 
	else if(firstOrder->getStatus() == CANCELED || firstOrder->isMaxCancelNumber())
	{
        this->twoLegStatus = READY;
		TRACE_LOG("StrategyState: %d, twoLegStatus: %d, suit: %d", strategyStatus, twoLegStatus, this->holdingCounter->getSuite());
	} 
	else if(firstOrder->isRejected())
	{
		firstOrderRejected();
	}
}

void ShootStrategy::readyAction()
{
	//TRACE_LOG("readyAction %d" , this->strategyStatus);
	if(this->strategyStatus == SHOULD_STOP)
	{
        this->strategyStatus = STOP;
		TRACE_LOG("%s stopped.", this->id.c_str());
		return;
	}

	if(!validPrice()) return;
	if(holdingCountAllow() && condition->isMeet())
	{
        static int count = 0;
        if (firstLeg.getLot() == 0 && secondLeg.getLot() == 0)
        {
            char log[2024]; sprintf_s(log, "%s¼õ%s¼Û²î%s%dÁË", firstLeg.getContract().c_str(), secondLeg.getContract().c_str(), condition->getComparator().c_str(), int(condition->getDiff()));
            if (count++ % 1000 == 0) { TRACE_LOG(log); ThreadGoogleAlert(log); }
            return;
        }
		TRACE_LOG(this->toString().c_str());
		shootAnyway();
	}
}

void * ShootStrategy::create(string id)
{
	TRACE_LOG("Create ShootStrategy %s", id.c_str());
    ShootStrategy * s = new ShootStrategy();
	s->setId(id);
    return (void *) s;
}

void ShootStrategy::shootAnyway()
{
	TRACE_LOG("%s shootAnyway",id.c_str());
	if(strategyStatus != READY) return;
	if(twoLegStatus != READY) return;
	firstOrder = this->firstLeg.shoot(1);
	this->twoLegStatus = AFTER_FIRST_SHOOT;
	TRACE_LOG("%s : first Leg Shoot " ,this->id.c_str());
}

