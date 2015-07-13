#include "CatsPriceSource.h"
#include <iostream>
#include "../LogStore.h"
#include "CatsApiEngine.h"

CatsPriceSource::CatsPriceSource(void)
{
	started = false;
}


CatsPriceSource::~CatsPriceSource(void)
{
}

void CatsPriceSource::add(string contract)
{
	CatsApiEngine* pEngine = CatsApiEngine::getEngineInstance();
	pEngine->add(contract);
}

void CatsPriceSource::start(void)
{
	if(started) {
        TRACE_LOG("CatsPriceSource already started.");
	    return;
	}
	CatsApiEngine* pEngine = CatsApiEngine::getEngineInstance();
    TRACE_LOG("CatsPriceSource started.");	
	started = true;
	Sleep(500);

	return;
}
