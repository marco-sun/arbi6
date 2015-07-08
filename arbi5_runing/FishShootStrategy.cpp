// FishShootStrategy.cpp: implementation of the FishShootStrategy class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FishShootStrategy.h"
#include "LogStore.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FishShootStrategy::FishShootStrategy(string id)
{
	TRACE_LOG("Create FishShootStrategy %s", id.c_str());
	this->isFish = false;
	this->id = id;

	fishStrategy = (FishStrategy *) FishStrategy::create(id + "-F");
	shootStrategy = (ShootStrategy *) ShootStrategy::create(id + "-S");
	
	this->holdingCounter = new HoldingCounter();
	fishStrategy->setHoldingCounter(this->holdingCounter);
	shootStrategy->setHoldingCounter(this->holdingCounter);
	TRACE_LOG("Create FishShootStrategy %s finished", id.c_str());
}

FishShootStrategy::~FishShootStrategy()
{

}

void FishShootStrategy::setFish(bool s)
{
	this->isFish = s;
}

void * FishShootStrategy::create(string id)
{
    FishShootStrategy * s = new FishShootStrategy(id);
	return (void *) s;

}


void FishShootStrategy::setLegA(Leg * leg)
{
	fishStrategy->setLegA(leg);
	shootStrategy->setLegA(leg);
}

void FishShootStrategy::setLegB(Leg * leg)
{
	fishStrategy->setLegB(leg);
	shootStrategy->setLegB(leg);
}

void FishShootStrategy::setCondition(Condition * condition)
{
	fishStrategy->setCondition(condition);
	shootStrategy->setCondition(condition);
}


void FishShootStrategy::start()
{
	TRACE_LOG("fishStrategy start :: %p",fishStrategy);
	TRACE_LOG("fishStrategy: %s", fishStrategy->toString2().c_str());
	TRACE_LOG("shootStrategy start :: %p",shootStrategy);
	TRACE_LOG("shootStrategy: %s", shootStrategy->toString2().c_str());
	fishStrategy->resetCount();
	shootStrategy->resetCount();
	if(isFish) fishStrategy->start();
	shootStrategy->start();
}

void FishShootStrategy::stop()
{
	fishStrategy->stop();
	shootStrategy->stop();
}

string FishShootStrategy::toString()
{
	string ret = id + " ";
	ret += shootStrategy->toString2();
	return ret;
}

string FishShootStrategy::toDesc()
{
	return shootStrategy->toDesc();
}

void FishShootStrategy::setHoldingOperation(enum HoldingOperation holdingOperation)
{
	shootStrategy->setHoldingOperation(holdingOperation);
	fishStrategy->setHoldingOperation(holdingOperation);
}

 void FishShootStrategy::correctCondition()
 {
	 shootStrategy->correctCondition();
	 fishStrategy->correctCondition();
 }

 void FishShootStrategy::setHoldingCounter(HoldingCounter * holdingCounter)
 {
	 this->holdingCounter = holdingCounter;
	 shootStrategy->setHoldingCounter(holdingCounter);
	 fishStrategy->setHoldingCounter(holdingCounter);
 }

 Leg FishShootStrategy::getLegA()
 {
	 return shootStrategy->getLegA();
 }

 Leg FishShootStrategy::getLegB()
 {
     return shootStrategy->getLegB();
 }

 bool FishShootStrategy::isFished()
 {
	return isFish;
 }

 Condition FishShootStrategy::getCondition()
 {
	 return shootStrategy->getCondition();
 }

 bool FishShootStrategy::isSecondLegMissed()
 {
	 return shootStrategy->isSecondLegMissed() || fishStrategy->isSecondLegMissed();
 }

 void FishShootStrategy::missedLegHandled()
 {
	 if (shootStrategy->isSecondLegMissed())
		 return shootStrategy->missedLegHandled();
	 if (fishStrategy->isSecondLegMissed())
		 fishStrategy->missedLegHandled();
 }

  void FishShootStrategy::shootAnyway()
  {
	  shootStrategy->shootAnyway();
  }