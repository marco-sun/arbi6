// Price.cpp: implementation of the Price class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Price.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Price Price::SBH4;
Price Price::SBN4;
Price Price::SBK4;

Price::Price()
{
	memset(debug,0,16);
	sprintf_s(debug,16,"PriceObject");
	time = 0;
	contract = "un-inited";
    ask = 0;
	bid = 0;
}

Price::~Price()
{

}



void Price::set(time_t t, double ask, double bid)
{
	this->time = t;
	this->ask = ask;
	this->bid = bid;
}

void Price::set(time_t t, double ask, double bid, double lastPrice)
{
	this->time = t;
	this->ask = ask;
	this->bid = bid;
	this->lastPrice = lastPrice;
}

double Price::getAsk()
{
	//TODO:use throw exception
	return ask;
}

double Price::getBid()
{
	return bid;
}

double Price::getLastPrice()
{
	return lastPrice;
}

double Price::get(string askOrBid)
{
    return (askOrBid.compare("ask") == 0) ? ask : bid;
}

bool Price::isValid()
{
	return bid > 10 || ask > 10;
}



