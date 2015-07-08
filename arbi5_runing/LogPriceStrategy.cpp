// LogPriceStrategy.cpp: implementation of the LogPriceStrategy class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogPriceStrategy.h"
#include "Contract.h"
#include <iostream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LogPriceStrategy::LogPriceStrategy()
{

}

LogPriceStrategy::LogPriceStrategy(int i)
{
}

LogPriceStrategy::~LogPriceStrategy()
{

}

void LogPriceStrategy::trig()
{
	
	cout << Contract::get(contract)->getPrice().getAsk() << ":";
	cout << Contract::get(contract)->getPrice().getBid() << endl;
}

void LogPriceStrategy::set(string contract)
{
	this->contract = contract;
}
