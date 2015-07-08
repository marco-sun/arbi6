// DayTradingStrategy.cpp: implementation of the DayTradingStrategy class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DayTradingStrategy.h"
#include "Persistence.h"
#include "LogStore.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DayTradingStrategy::DayTradingStrategy()
{
	strategyA = NULL;
	strategyB = NULL;
    initHoldingCounter();
}

DayTradingStrategy::~DayTradingStrategy()
{

}

DayTradingStrategy * DayTradingStrategy::get(string id) {
	DayTradingStrategy * dts = NULL;

    list<DayTradingStrategy *>::iterator the_iterator = dayTradingStrategies.begin();

    while( the_iterator != dayTradingStrategies.end() ) {
        if((*the_iterator)->id.compare(id) == 0) dts = (*the_iterator);
        the_iterator++;
	}

	return dts;
}

bool DayTradingStrategy::exist(string id) 
{
    return get(id) != NULL;
}

DayTradingStrategy * DayTradingStrategy::create(string id)
{
	if(exist(id)) return NULL;
	DayTradingStrategy * dts = new DayTradingStrategy();
	dts->id = id;
	dayTradingStrategies.push_back(dts);
	return dts;
}

TwoLegStrategy * DayTradingStrategy::initSubStrategyAsNecessary(TwoLegStrategy * s, string idSuffix)
{
	if(s == NULL)
	{
		s = TwoLegStrategy::create(id + idSuffix);
		s->setHoldingCounter(holdingCounter);
		TRACE_LOG("initSubStrategyAsNecessary %s", (id + idSuffix).c_str());
	}
	return s;
}

TwoLegStrategy * DayTradingStrategy::getStrategyA()
{
	return strategyA;
}

TwoLegStrategy * DayTradingStrategy::getStrategyB()
{
	return strategyB;
}

TwoLegStrategy * DayTradingStrategy::createStrategyA()
{
	strategyA = initSubStrategyAsNecessary(strategyA, "-A");
	return strategyA;
}

TwoLegStrategy * DayTradingStrategy::createStrategyB()
{
	strategyB = initSubStrategyAsNecessary(strategyB, "-B");
	return strategyB;
}

void DayTradingStrategy::setKaiCangStrategy(TwoLegStrategy * kaiCangStrategy)
{
	this->strategyA = kaiCangStrategy;
    this->setHoldingCounter(strategyA->getHoldingCountInstance());
}

void DayTradingStrategy::setPingCangStrategy(TwoLegStrategy * pingCangStrategy)
{
	this->strategyB = pingCangStrategy;
	if(strategyB != NULL)
		strategyB->setHoldingCounter(strategyA->getHoldingCountInstance());
}

void DayTradingStrategy::trig()
{
	// Do nothing.
}

string DayTradingStrategy::toString()
{
	string ret = this->id + " " + strategyA->getId() + " ";
	ret +=  (strategyB == NULL) ? "NULL" : strategyB->getId();
	return ret;
}

string intToString(int i);

string DayTradingStrategy::toDesc()
{
	string ret = "套数" + intToString(this->getHighHoldingLimit()) + " " + strategyA->toDesc() + " ";
	ret +=  (strategyB == NULL) ? "[NULL]" : strategyB->toDesc();
	ret += " 成交套数:";
	ret += intToString(this->holdingCounter->getSuite());
	return ret;
}


void DayTradingStrategy::clear()
{
    dayTradingStrategies.clear();
}
/*
void DayTradingStrategy::load()
{
	Persistence::loadDayTrading();
}
*/

void DayTradingStrategy::save()
{
	Persistence::saveDayTrading();
	this->strategyA->correctCondition();
	if (this->strategyB != NULL)
        this->strategyB->correctCondition();
	Persistence::saveTwoLeg();
}

void DayTradingStrategy::remove()
{
	deleteThis();
	this->strategyA->remove();
	if (this->strategyB != NULL)
		this->strategyB->remove();
	this->save();
}

void DayTradingStrategy::deleteThis()
{
	dayTradingStrategies.remove(this);
}


list<DayTradingStrategy *> DayTradingStrategy::listAll()
{
	Persistence::loadAsNecessary();
	return dayTradingStrategies;
}

list<DayTradingStrategy *> DayTradingStrategy::dayTradingStrategies;

void DayTradingStrategy::setStrategyA(string id)
{
	//printf("setStrategyA %s\n", id.c_str());
	TwoLegStrategy * s = TwoLegStrategy::getTwoLegStrategy(id);
	assert(s != NULL);
	this->setKaiCangStrategy(s);
}

void DayTradingStrategy::setStrategyB(string id)
{
	//printf("setStrategyB %s\n", id.c_str());
	TwoLegStrategy * s = TwoLegStrategy::getTwoLegStrategy(id);
	assert(s != NULL);
	this->setPingCangStrategy(s);
}

void DayTradingStrategy::start()
{
	this->strategyA->start();
	if (strategyB != NULL)
		this->strategyB->start();
}

bool DayTradingStrategy::hasStrategyB()
{
	return (strategyB != NULL);
}

void DayTradingStrategy::stop()
{
	TRACE_LOG("%s  strategyStatus shouldStop!", this->id.c_str());
	strategyA->stop();
	if(hasStrategyB()) strategyB->stop();
}

void DayTradingStrategy::work()
{
	strategyA->work();
	if(hasStrategyB()) strategyB->work();
}

void DayTradingStrategy::shootAnyway()
{
	if(hasStrategyB()) return;
	strategyA->shootAnyway();
}

void DayTradingStrategy::setStrategyBNull()
{
	strategyB = NULL;
	strategyB->remove();
}