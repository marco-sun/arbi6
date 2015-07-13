// Dealer.cpp: implementation of the Dealer class.
//
//////////////////////////////////////////////////////////////////////

#include "Dealer.h"
#include <assert.h>
#include <iostream>

#include "../LogStore.h"
#include "../cats/CatsDealer.h"
#include "../ctp/SfitDealer.h"
#include "Contract.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Dealer::Dealer()
{

}

Dealer::~Dealer()
{

}

bool Dealer::useRealDealer = true;

Dealer * Dealer::domestic = new DumbDealer();

Dealer * Dealer::oversea = new DumbDealer();

bool Dealer::domesticNotInited = true;

bool Dealer::overseaNotInited = true;

void Dealer::initDomestic()
{
	delete domestic;
    domestic = NULL;

	domestic = new SfitDealer();
	TRACE_LOG("initDomestic Create SfitDealer...");
	((SfitDealer*)domestic)->start();
	TRACE_LOG("initDomestic Create SfitDealer...OK.");

    domesticNotInited = false;
}

void Dealer::initOversea()
{
	delete oversea;
    oversea = NULL;

	oversea = new CatsDealer();
	TRACE_LOG("initOversea Create CatsDealer...");
	((CatsDealer*)oversea)->start();
	TRACE_LOG("initOversea Create CatsDealer...OK.");

    overseaNotInited = false;
}

void Dealer::initAsNeccessaryFor(string contract)
{
	assert(useRealDealer);
    if(Contract::get(contract)->isDomestic() && domesticNotInited)
	{
		//cout << "initDomestic" << endl;
		initDomestic();
	}
    if(Contract::get(contract)->isOversea() && overseaNotInited)
	{
		//cout << "initOversea" << endl;
		initOversea();
	}
}

Dealer * Dealer::getDealerFor(string contract)
{
	if(useRealDealer)
	{
		//cout << "getDealerFor" << endl;
		initAsNeccessaryFor(contract);
	}

    Dealer * d = Contract::get(contract)->isDomestic() ? domestic : oversea;

	return d;
}
/*
void Dealer::placeOrder(Order * order)
{
	Dealer *pDealer = getDealerFor(order->getContract());
	if(pDealer) pDealer->placeOrder(order);
}

void Dealer::cancelOrder(Order * order)
{
	Dealer *pDealer = getDealerFor(order->getContract());
	if(pDealer) pDealer->cancelOrder(order);
}
*/
void Dealer::cancel(Order * order)
{
	TRACE_LOG("Cancel Order: %s ", order->getId().c_str());
	Dealer * d = Dealer::getDealerFor(order->getContract());
	d->cancelOrder(order);
}

//

DumbDealer::DumbDealer()
{

}

DumbDealer::~DumbDealer()
{

}

void DumbDealer::placeOrder(Order * order)
{
}

void DumbDealer::cancelOrder(Order * order)
{
}