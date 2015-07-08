// PriceSource.cpp: implementation of the PriceSource class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PriceSource.h"
#include "EsunnyPriceSource.h"
#include "SfitPriceSource.h"
#include "SpPriceSource.h"
#include "otherPriceSource.h"
#include "RohonPriceSource.h"
#include "MockAsyncPriceSource.h"
#include "FixPatsPriceSource.h"
#include "PatsApiPriceSource.h"
#include "LogStore.h"
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
	int flag_Esunny = GetPrivateProfileInt("ESUNNY", "VALID", 0, ".\\AIB.ini");
	int flag_Rohon = GetPrivateProfileInt("ROHONPS", "VALID", 0, ".\\AIB.ini");
	int flag_FixPats = GetPrivateProfileInt("FIXPATSGW", "VALID", 0, ".\\AIB.ini");
	int flag_OtherPriceSource = GetPrivateProfileInt("BOHAI", "VALID", 0, ".\\AIB.ini");
	int flag_PatsApi = GetPrivateProfileInt("PATSAPIPS", "VALID", 0, ".\\AIB.ini");

	if(domastic==NULL && marketflag!=2)
	{
		if(flag_Esunny==1) 
		{
			domastic = EsunnyPriceSource::getInstance();
			domastic->start();
			TRACE_LOG("Create EsunnyPriceSource.");
		}
		else
		{
			domastic = new SfitPriceSource();
			domastic->start();
			TRACE_LOG("Create SfitPriceSource.");
		}
	}
	if(oversea==NULL && marketflag!=1)
	{
		
		if(flag_Rohon==1)
		{
			oversea = new RohonPriceSource();
			oversea->start();
			TRACE_LOG("Create RohonPriceSource.");
		}
		else if(flag_FixPats==1)
		{
			oversea = new FixPatsPriceSource();
			oversea->start();
			TRACE_LOG("Create FixPatsPriceSource.");
		}
		else if(flag_OtherPriceSource == 1)
		{	
			oversea = new otherPriceSource();
			oversea->start();
			TRACE_LOG("Create otherPriceSource.");
		}
		else if(flag_PatsApi==1)
		{
			oversea = new PatsApiPriceSource();
			oversea->start();
			TRACE_LOG("Create PatsApiPriceSource.");
		}
		else 
		{
			oversea = new SpPriceSource();
			oversea->start();
			TRACE_LOG("Create SpPriceSource.");
		}
	}
	/*
	if(domastic==NULL) {
		domastic = new SfitPriceSource();
		domastic->start();
		TRACE_LOG("Create SfitPriceSource.");
	}
	if(oversea==NULL) {
		oversea = new RohonPriceSource();
		oversea->start();
		TRACE_LOG("Create RohonPriceSource.");
	}*/
}

void PriceSource::useDumbPriceSource()
{
	domastic = new DumbPriceSource();
    oversea = domastic;
}

void PriceSource::useMockPriceSource()
{
	domastic = new MockAsyncPriceSource();
	domastic->start();
    oversea = domastic;
}

void PriceSource::useRealPriceSource()
{
    domastic = NULL;
    oversea = NULL;
}

void PriceSource::useFixPriceSource()
{
    oversea = new FixPatsPriceSource();
	oversea->start();
	domastic = NULL;
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
