// Strategy.cpp: implementation of the Strategy class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Strategy.h"
#include <process.h>
#include <assert.h>
#include <iostream> 
#include "windows.h"
#include "ShootStrategy.h"
#include "FishStrategy.h"
#include "LogStore.h"
#define TRIG_ITERVAL 50


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Strategy::Strategy()
{
    threadCreated = false;
	strategyStatus = STOP;
}

Strategy::~Strategy()
{

}


bool Strategy::alreadyExist(string id)
{
	return strategies.get(id) != NULL;
}

//TODO remove this
bool Strategy::regist(Strategy * s)
{
	if(alreadyExist(s->getId())) 
	{
		cout << "Regist strategy failed :" << s->getId() << endl;
		return false;
	}
	strategies.add(s);
	return true;
}

Strategies Strategy::strategies;

Orders * Strategy::getOrders()
{
	return NULL;
}

void WorkFunction(void* pParams)
{
	Strategy *s=(Strategy *) pParams;
	while(1)
	{
		s->trig();
		Sleep(TRIG_ITERVAL);
	}
}

void Strategy::work()
{
    strategyStatus = WORK;
	TRACE_LOG("Strategy %s to work", this->id.c_str());
}

void Strategy::start()
{

	work();
	TRACE_LOG("Strategy::start %p",this);
	if(!threadCreated)
	{
		TRACE_LOG("Strategy %s start!", this->id.c_str());
		threadCreated = true;
		_beginthread(WorkFunction, 0, (void *)this);
	} else {
		TRACE_LOG("Strategy %s already  started!", this->id.c_str());
	}

}

void Strategy::stop()
{
	strategyStatus = SHOULD_STOP;
	TRACE_LOG("%s  strategyStatus shouldStop %d!", this->id.c_str(),strategyStatus);
}

void Strategy::setId(string id)
{
	this->id = id;
}

string Strategy::getId()
{
	return id;
}

void Strategies::add(Strategy * s)
{
	strategyList.push_back(s);
}

Strategy * Strategies::get(string id)
{
    list<Strategy *>::iterator the_iterator = strategyList.begin();

    while( the_iterator != strategyList.end() ) {
        if((*the_iterator)->getId().compare(id) == 0) return (*the_iterator);
        the_iterator++;
	}

	return NULL;
}

bool Strategy::isNotStop()
{
	return !isStop();
}

bool Strategy::isStop()
{
	return this->strategyStatus == STOP;
}

bool Strategy::isStart()
{
	return this->strategyStatus == WORK;
}

bool Strategy::isHalt()
{
	return this->strategyStatus == HALT;
}

bool Strategy::isWork()
{
	return this->strategyStatus == WORK;
}