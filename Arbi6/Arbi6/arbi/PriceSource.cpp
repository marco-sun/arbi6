// PriceSource.cpp: implementation of the PriceSource class.
//
//////////////////////////////////////////////////////////////////////

#include "PriceSource.h"
#include "../ctp/SfitPriceSource.h"
#include "../cats/CatsPriceSource.h"
#include "../LogStore.h"
#include "Commodity.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PriceSource::PriceSource()
{

}

PriceSource::~PriceSource()
{

}

PriceSource * PriceSource::domastic = NULL;

PriceSource * PriceSource::oversea = NULL;


void PriceSource::listen(string contract)
{
	TRACE_LOG("listen %s", contract.c_str());
	Commodity *pCommodity = Commodity::findFor(contract);
	if (pCommodity==NULL) 
	{
		TRACE_LOG("listen error, NO Commodity for contract:%s.",
			contract.c_str());
		return;
	}

	

	if(pCommodity->isDomestic()) {
		init(1);
		domastic->add(contract);
	}
	if(pCommodity->isOversea()) {
		//TRACE_LOG("enter pCommodity->isOversea part");
		init(2);
		//TRACE_LOG("finish init(2) part");
		oversea->add(contract);
		//TRACE_LOG("finish oversea->add(contract) part");
	}
}

//TODO: make private and change to initAsNecessary
void PriceSource::init(int marketflag)
{
	if(domastic==NULL && marketflag!=2)
	{
		domastic = new SfitPriceSource();
		domastic->start();
		TRACE_LOG("Create SfitPriceSource.");

	}
	if(oversea==NULL && marketflag!=1)
	{
		oversea = new CatsPriceSource();
		oversea->start();
		TRACE_LOG("Create CatsPriceSource.");
	}
}

void PriceSource::useDumbPriceSource()
{
	domastic = new DumbPriceSource();
    oversea = domastic;
}

void PriceSource::useRealPriceSource()
{
    domastic = NULL;
    oversea = NULL;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


Pricing::Pricing(string contract, double ask, double bid) 
{
	this->ask = ask;
	this->bid = bid;
	this->contract = contract;
}

Pricing::Pricing() 
{
	this->ask = 0;
	this->bid = 0;
	this->contract = "";
}

Pricing::~Pricing()
{

}
