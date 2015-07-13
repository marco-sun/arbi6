// FishStrategy.cpp: implementation of the FishStrategy class.
//
//////////////////////////////////////////////////////////////////////

#include "FishStrategy.h"
#include "../LogStore.h"
#include <iostream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FishStrategy::FishStrategy()
{
    fishConditionOk = false;
}

FishStrategy::~FishStrategy()
{

}

void * FishStrategy::create(string id)
{
	TRACE_LOG("Create FishStrategy %s", id.c_str());
    FishStrategy * s = new FishStrategy();
	s->setId(id);
	return (void *) s;
}

void FishStrategy::trig()
{
	trig(time(NULL));
}

void FishStrategy::trig(time_t t)
{
	genFishConditionAsNeccessary();
	startStopControl(t);
	if(strategyStatus == STOP || strategyStatus == HALT)
	{
		return;
	}

	switch(this->twoLegStatus)
	{
        case READY:
			readyAction();
			break;
		case FISHING:
			fishingAction();
			break;
		case AFTER_SECOND_SHOOT:
			afterSecondShootAction();
			break;
		case AFTER_CANCEL_FISH:
			afterCancelFishAction();
			break;
	}
}

void FishStrategy::genFishConditionAsNeccessary()
{
	if(!fishConditionOk)
	{
		fishCondition = condition->genFishCondition(firstLeg.getContract());
		fishConditionOk = true;
		//cout << fishCondition.toString() << endl;
	}
}

bool FishStrategy::meetCondition(double firstLegPrice, double secondLegPrice)
{
	bool ret = fishCondition->isMeet(firstLeg.getContract(), firstLegPrice, secondLeg.getContract(), secondLegPrice);
	if(ret) {
		TRACE_LOG("Fish strategy %s", this->toString2().c_str());
		TRACE_LOG("Condition %s", fishCondition->toString().c_str());
	}
	return ret;
}

void FishStrategy::updateFishStatus() {
    if(firstOrder != NULL)
	{
		this->twoLegStatus = FISHING;
		concernedSecondLegPriceAtFishing = secondLeg.getConcernedPrice();
	}
}

double FishStrategy::calcFishPrice()
{
	double fishPrice = firstLeg.fishFirstPrice();
	double secondShootPrice = secondLeg.shootPrice();
	while(true)
	{
		if(meetCondition(fishPrice, secondShootPrice)) break;
		fishPrice = firstLeg.betterFishPrice(fishPrice);
	}
	return fishPrice;
}

void FishStrategy::readyAction()
{
	if(this->strategyStatus == SHOULD_STOP)
	{
        this->strategyStatus = STOP;
		TRACE_LOG("%s stopped.", this->id.c_str());
		return;
	}

	if(!holdingCountAllow()) return;

    printf("%f %f \n", firstLeg.fishFirstPrice(), secondLeg.shootPrice());
	if(firstMeetCondition())
	{
		TRACE_LOG("fish strategy %s",this->toString2().c_str());
		this->firstOrder = firstLeg.fishFirst();
	}
	else if(equalFirstMeetCondition())
	{
		this->firstOrder = firstLeg.fishEqualFirst();
	}
	else if(firstLeg.isOversea())
	{
		this->firstOrder = firstLeg.fishAt(calcFishPrice());
	}
	updateFishStatus();


}

void FishStrategy::cancelBecause(const char * s)
{
    TRACE_LOG(s);
    firstOrder->cancelAfter(0);
    this->twoLegStatus = AFTER_CANCEL_FISH;
}

void FishStrategy::cancelAsNecessary()
{
	if(!holdingCountAllow()) 
		return cancelBecause("Cacel fish because holding count");
	if(this->strategyStatus == SHOULD_STOP) 
		return cancelBecause("Cacel fish because should stop");
	if(!inTrading(time(NULL))) 
		return cancelBecause("Cacel fish because not in trading time");
	if(conditionNotMeet()) 
		return cancelBecause("Cacel fish because condition not meet");
	if(notFirst() && firstWillMeet()) 
		return cancelBecause("Cacel fish because not first, but first meet condition");
    if(notFirst() && firstLeg.isDomestic())
	  return cancelBecause("Cacel fish because not first for domestic");
	//if(notFirst())
	    //return cancelBecause("Cacel fish because not first");
    if(secondLegMoved()) 
		return cancelBecause("Cacel fish because second Leg moved");
}

void FishStrategy::fishingAction()
{

    if(firstOrder->getStatus() == FINISHED)
	{
		TRACE_LOG("second fishLeg shoot");
        secondOrder = this->secondLeg.shoot(5);
		this->twoLegStatus = AFTER_SECOND_SHOOT;
		return;
	}

	if(firstOrder->isCanceled() || firstOrder->isMaxCancelNumber()) return firstOrderCanceled();

	if(firstOrder->isRejected()) return firstOrderRejected();

	cancelAsNecessary();

}

void FishStrategy::firstOrderCanceled()
{
	TRACE_LOG("firstOrder canceled!");
	firstOrder = NULL;
	twoLegStatus = READY;
}

bool FishStrategy::secondLegMoved()
{
	double concernedSecondLegPrice = secondLeg.getConcernedPrice();
	double vary = (concernedSecondLegPriceAtFishing - concernedSecondLegPrice) / concernedSecondLegPrice;
    return vary > 0.002 || vary < -0.002;
}

bool FishStrategy::notFirst()
{
	return firstLeg.notFirst(firstOrder->getPrice());
}

void FishStrategy::afterCancelFishAction()
{
    if(firstOrder->getStatus() == FINISHED)
	{
		TRACE_LOG("firstOrder cancel but finished!");
        secondOrder = this->secondLeg.shoot(5);
		this->twoLegStatus = AFTER_SECOND_SHOOT;
		return;
	}

	if(firstOrder->isCanceled() || firstOrder->isMaxCancelNumber())
	{
        firstOrderCanceled();
	}

}

bool FishStrategy::conditionNotMeet()
{
	return !fishCondition->isMeet(firstOrder->getContract(), firstOrder->getPrice(), secondLeg.getContract(), secondLeg.shootPrice());
}

bool FishStrategy::firstWillMeet()
{
	return firstMeetCondition() || equalFirstMeetCondition();
}

bool FishStrategy::firstMeetCondition()
{
	return meetCondition(firstLeg.fishFirstPrice(), secondLeg.shootPrice());
}

bool FishStrategy::equalFirstMeetCondition()
{
	return meetCondition(firstLeg.fishEqualFirstPrice(), secondLeg.shootPrice());
}

void FishStrategy::setCondition(Condition * condition)
{
	this->condition = condition;
	this->fishCondition = condition->genFishCondition(firstLeg.getContract());
}