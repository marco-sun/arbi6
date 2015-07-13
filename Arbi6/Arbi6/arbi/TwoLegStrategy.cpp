// TwoLegStrategy.cpp: implementation of the TwoLegStrategy class.
//
//////////////////////////////////////////////////////////////////////

#include "TwoLegStrategy.h"
#include "ShootStrategy.h"
#include "FishStrategy.h"
#include "FishShootStrategy.h"
#include "Persistence.h"
#include "../LogStore.h"
#include "../utils/Utils.h"
#include "DealHistory.h"
#include "IntentionPair.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TwoLegStrategy::TwoLegStrategy()
{
    firstOrder = NULL;
	secondOrder = NULL;
	twoLegStatus = READY;
	setHoldingOperation(INCREASE);
	initHoldingCounter();
}

TwoLegStrategy::~TwoLegStrategy()
{

}

void TwoLegStrategy::workToHalt(time_t t)
{
	if (strategyStatus == WORK && !inTrading(t))
	{
		if(this->twoLegStatus == READY)
		{
			this->strategyStatus = HALT;
		    TRACE_LOG("%s set strategyStatus halt", this->id.c_str());
		} else {
			//this->stop();
		}
	}
}

void TwoLegStrategy::haltToWork(time_t t)
{
	if(strategyStatus == HALT && inTrading(t))
	{
		strategyStatus = WORK;
		TRACE_LOG("%s From halt to work", this->id.c_str());
	}
}

void TwoLegStrategy::startStopControl(time_t t)
{
    workToHalt(t);
    haltToWork(t);
}

bool TwoLegStrategy::isKaiCang()
{
	return firstLeg.isKaiCang() && secondLeg.isKaiCang();
}

bool TwoLegStrategy::isPingCang()
{
	return firstLeg.isPingCangOrPingJin() && secondLeg.isPingCangOrPingJin();
}

bool TwoLegStrategy::isYiCang()
{
	return (firstLeg.isKaiCang() && secondLeg.isPingCang())
        || (firstLeg.isPingCang() && secondLeg.isKaiCang());
}

void TwoLegStrategy::missedLegHandled()
{
	assert(isSecondLegMissed());
	secondOrder->manuallyFinish();
	finishOneSuite();
}

bool TwoLegStrategy::isSecondLegMissed()
{
	return twoLegStatus == SECOND_LEG_MISSED;
}

void TwoLegStrategy::finishOneSuite()
{
	genDealInfo();
	updateCounter();
	this->firstOrder = NULL;
	this->secondOrder = NULL;
	twoLegStatus = READY;
	TRACE_LOG("TwoLegStrategy finishOneSuite %d!", strategyStatus);

	if(strategyStatus == SHOULD_STOP) {
		strategyStatus = STOP;
		TRACE_LOG("%s stopped.", this->id.c_str());
	}

}

void TwoLegStrategy::genDealInfo()
{
    if(firstOrder == NULL || secondOrder == NULL)
	{
		TRACE_LOG("Unable to gen deal info.");
		return;
	}

    double p1 = firstOrder->avgDealPrice();
	double p2 = secondOrder->avgDealPrice();
	TRACE_LOG("TwoLegStrategy::genDealInfo1 firstOrderDealPrice:%f,secondOrderDealPrice:%f",p1,p2);
 
	//printf("%f %f \n", p1, p2);

	double diff = this->condition->calcDiff(firstOrder->getContract(), p1, secondOrder->getContract(), p2);
	char stime[32];
	SYSTEMTIME t;
	GetLocalTime( &t );
	sprintf_s(stime, 32, "%02d时%02d分%02d秒", t.wHour, t.wMinute, t.wSecond);

	char temp[512];
	sprintf_s(temp, 512, "%s 于 %s 成交差价:%.2f", id.c_str(), stime, diff);

	DealHistory::getInstance()->add(temp); 

}


//To be overrided
void TwoLegStrategy::trig()
{
    finishOneSuite();  
}

void TwoLegStrategy::trig(time_t t)
{

}

void TwoLegStrategy::setLegA(Leg leg)
{
	this->firstLeg = leg;
}

void TwoLegStrategy::setLegB(Leg leg)
{
	this->secondLeg = leg;
}

void TwoLegStrategy::validate(Condition condition)
{
	IntentionPair * ip = IntentionPair::findFor(this->id);
	if(ip != NULL) {
        assert(condition.getContractA().compare(ip->getContractA()) == 0);
	    assert(condition.getContractB().compare(ip->getContractB()) == 0);
	} else {
		TRACE_LOG("Related IntentionPair not found: %s.", condition.toString().c_str());
	}
}

void TwoLegStrategy::setCondition(Condition condition)
{
    validate(condition);
	this->condition = condition.copy();
}

bool startWith(const char * s1, const char * s2);

void TwoLegStrategy::correctCondition()
{
	string p1 = firstLeg.getConcernPrice();
	string p2 = secondLeg.getConcernPrice();
	string c;
	if(startWith(p1.c_str(), condition->getContractA().c_str()))
	{
        c = p1 + "-" + p2;
	} else {
		c = p2 + "-" + p1;
	}
    condition->setDiffExp(c);
}

void TwoLegStrategy::setLegA(Leg * leg)
{
	setLegA(*leg);
}

void TwoLegStrategy::setLegB(Leg * leg)
{
	setLegB(*leg);
}

void TwoLegStrategy::setCondition(Condition * condition)
{
	setCondition(*condition);
}

Order * TwoLegStrategy::getFirstOrder()
{
	return this->firstOrder;
}

Order * TwoLegStrategy::getSecondOrder()
{
	return this->secondOrder;
}

bool TwoLegStrategy::holdingCountAllow()
{
	int expectedHoldingCount = this->getHoldingCount();
	this->holdingOperation == INCREASE ? expectedHoldingCount++ : expectedHoldingCount--;
    return betweenLimit(expectedHoldingCount);
}


void TwoLegStrategy::afterSecondShootAction()
{
	//TRACE_LOG("afterSecondShootAction!");
    if(secondOrder->getStatus() == FINISHED)
	{
		this->finishOneSuite();
		TRACE_LOG("finishOneSuite!");
	} 
	else if(secondOrder->getStatus() == CANCELED || secondOrder->isMaxCancelNumber())
	{
		this->twoLegStatus = SECOND_LEG_MISSED;
		TRACE_LOG("secondOrder is canceled!");
	}
	else if(secondOrder->isRejected())
	{
		TRACE_LOG("Second order rejected! %s", secondOrder->toString().c_str());
		this->strategyStatus = STOP;
	}
}

list<TwoLegStrategy *> TwoLegStrategy::twoLegStrategies;

list<TwoLegStrategy *> TwoLegStrategy::listAll()
{
	Persistence::loadAsNecessary();
	return twoLegStrategies;
}

void TwoLegStrategy::clear()
{
	twoLegStrategies.clear();
}

void TwoLegStrategy::save()
{
	TRACE_LOG("TwoLegStrategy::save , %s", this->toString().c_str());
	correctCondition();
	Persistence::saveTwoLeg();
}

void * TwoLegStrategy::createShootStrategy(string id)
{
	void * p = ShootStrategy::create(id);
	TwoLegStrategy::add(p);
	return p;
}

void * TwoLegStrategy::createFishStrategy(string id)
{
	void * p = FishStrategy::create(id);
	TwoLegStrategy::add(p);
	return p;
}
void * TwoLegStrategy::createFishShootStrategy(string id)
{
	void * p = FishShootStrategy::create(id);
	TwoLegStrategy::add(p);
	return p;
}


TwoLegStrategy * TwoLegStrategy::getTwoLegStrategy(string id)
{
    list<TwoLegStrategy *>::iterator the_iterator = twoLegStrategies.begin();
    while( the_iterator != twoLegStrategies.end() ) {
        if((*the_iterator)->id.compare(id) == 0) return (*the_iterator);
        the_iterator++;
	}
	//TRACE_LOG("getTwoLegStrategy %s return NULL", id.c_str());
	return NULL;
}

TwoLegStrategy * TwoLegStrategy::create(string id)
{
	//TwoLegStrategy * s = (TwoLegStrategy * )createShootStrategy(id);
	//TwoLegStrategy * s = (TwoLegStrategy * )createFishStrategy(id);
	TwoLegStrategy * s = (TwoLegStrategy * )createFishShootStrategy(id);
	s->setHoldingLimit(0, 1);
	return s;
}

void TwoLegStrategy::add(void * p)
{
    twoLegStrategies.push_back((TwoLegStrategy *)p);
}

string TwoLegStrategy::toString()
{
	string ret = id + " ";
	ret += toString2();
	return ret;
}

string TwoLegStrategy::toString2()
{
	string ret = this->firstLeg.toString();
	ret += this->secondLeg.toString();
	ret += this->condition->toString();
	ret += this->limitString();
	return ret;
}

string TwoLegStrategy::toDesc()
{
	string ret;
	ret += "[" ;
	ret += this->condition->getComparator();
	ret += Utils::doubleToString(this->condition->getDiff());
	ret += this->firstLeg.toDes();
	ret += " ";
	ret += this->secondLeg.toDes();
	ret += "]";
	return ret;
}

bool TwoLegStrategy::validPrice()
{
	return firstLeg.hasValidPrice() && secondLeg.hasValidPrice();
}

void TwoLegStrategy::firstOrderRejected()
{
	TRACE_LOG("Order rejected! %s", firstOrder->toString().c_str());
	this->strategyStatus = STOP;
	this->twoLegStatus = READY;
	TRACE_LOG("TwoLegStrategy: %s stoped!",this->id.c_str());
}

Leg TwoLegStrategy::getLegA()
{
	return this->firstLeg;
}

Leg TwoLegStrategy::getLegB()
{
	return this->secondLeg;
}

Condition TwoLegStrategy::getCondition()
{
    return *condition;
}


void TwoLegStrategy::remove()
{
	twoLegStrategies.remove(this);	
}

bool TwoLegStrategy::inTrading(time_t t)
{
	if (this->firstLeg.inTrading(t) && this->secondLeg.inTrading(t))
		return true;
	return false;
}

void TwoLegStrategy::setFish(bool s)
{
	//have been covered;
}

bool TwoLegStrategy::isFished()
{
	//have been covered;
	return false;
}

void TwoLegStrategy::shootAnyway()
{
	//have been covered;
}