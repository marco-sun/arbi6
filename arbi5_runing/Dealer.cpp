// Dealer.cpp: implementation of the Dealer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dealer.h"
#include <assert.h>
#include <iostream>

#include "LogStore.h"
#include "EsunnyDealer.h"
#include "SfitDealer.h"
#include "SpDealer.h"
#include "otherDealer.h"
#include "RohonDealer.h"
#include "MockAsyncDealer.h"
#include "FixPatsDealer.h"
#include "PatsApiDealer.h"
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

//bool Dealer::useRealDealer = false;
bool Dealer::useRealDealer = true;

//Dealer * Dealer::domestic = new MockAsyncDealer();

//Dealer * Dealer::oversea = new MockAsyncDealer();

Dealer * Dealer::domestic = new DumbDealer();

Dealer * Dealer::oversea = new DumbDealer();

bool Dealer::domesticNotInited = true;

bool Dealer::overseaNotInited = true;

void Dealer::initDomestic()
{
	delete domestic;
    domestic = NULL;

	int flag_Esunny = GetPrivateProfileInt("ESUNNY", "VALID", 0, ".\\AIB.ini");
	if(flag_Esunny == 0)
	{
		domestic = new SfitDealer();
		TRACE_LOG("initDomestic Create SfitDealer...");
		((SfitDealer*)domestic)->start();
		TRACE_LOG("initDomestic Create SfitDealer...OK.");
	}
	else
	{
		domestic = EsunnyDealer::getInstance();
		TRACE_LOG("initDomestic Create EsunnyDealer...");
		((EsunnyDealer*)domestic)->start();
		TRACE_LOG("initDomestic Create EsunnyDealer...OK.");
	}
    domesticNotInited = false;
}

void Dealer::initOversea()
{
	delete oversea;
    oversea = NULL;

	int flag_Rohon = GetPrivateProfileInt("ROHONTRADE", "VALID", 0, ".\\AIB.ini");
	int flag_FixPats = GetPrivateProfileInt("FIXPATSGW", "VALID", 0, ".\\AIB.ini");
	int flag_OtherDealer = GetPrivateProfileInt("BOHAI", "VALID", 0, ".\\AIB.ini");
	int flag_patsapi = GetPrivateProfileInt("PATSAPIDEALER", "VALID", 0, ".\\AIB.ini"); 

	if(flag_Rohon==1)
	{
		oversea = new RohonDealer();
		TRACE_LOG("getInstance Create RohonDealer...");
		((RohonDealer*)oversea)->start();
		TRACE_LOG("getInstance Create RohonDealer...OK.");
		overseaNotInited = false;
	}
	else if(flag_FixPats==1)
	{
		oversea = new FixPatsDealer();
		TRACE_LOG("getInstance Create FixPatsDealer...");
		((FixPatsDealer*)oversea)->start();
		TRACE_LOG("getInstance Create FixPatsDealer...OK.");
		overseaNotInited = false;
	}
	else if(flag_OtherDealer == 1)
	{
		oversea = new otherDealer();
		TRACE_LOG("getInstance Create otherDealer...");
		((otherDealer*)oversea)->start();
		TRACE_LOG("getInstance Create otherDealer...OK.");
	    overseaNotInited = false;
	}
	else if(flag_patsapi==1)
	{
		oversea = new PatsApiDealer();
		TRACE_LOG("getInstance Create PatsApiDealer...");
		((PatsApiDealer*)oversea)->start();
		TRACE_LOG("getInstance Create PatsApiDealer...OK.");
		overseaNotInited = false;
	}
	else 
	{
		oversea = new SPDealer();
		TRACE_LOG("getInstance Create SPDealer...");
		((SPDealer*)oversea)->start();
		TRACE_LOG("getInstance Create SPDealer...OK.");
	    overseaNotInited = false;
	}

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