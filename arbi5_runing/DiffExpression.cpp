// DiffExpression.cpp: implementation of the DiffExpression class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DiffExpression.h"
#include "Contract.h"
#include "LogStore.h"
#include <sstream>
#include <iostream>

#define POSSIBLE_LOW_PRICE 0.000001
#define POSSIBLE_HIGH_RPICE 10000000

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
DiffExpression::DiffExpression()
{

}
*/
DiffExpression::DiffExpression(string exp)
{
    set(exp);
}

DiffExpression::~DiffExpression()
{

}

void DiffExpression::set(string exp)
{

	int pos = exp.find_first_of(".");
    exp.replace(pos, 1, " . ");
    pos = exp.find_last_of(".");
    exp.replace(pos, 1, " . ");
    pos = exp.find("-");
    exp.replace(pos, 1, " - ");

    std::stringstream stream(exp);
	string a;
	string s1;
	string s2;
	stream >> s1 >>a >> askBidOne >> a >> s2 >> a >> askBidTwo;
	this->contractOne = Contract::get(s1);
    this->contractTwo = Contract::get(s2);
	expressionOf1to2 = ArbitrageComparablePriceExpression::find(s1, s2);
	expressionOf2to1 = ArbitrageComparablePriceExpression::find(s2, s1);
}

double DiffExpression::getValue()
{
	return getValue(askBidOne, askBidTwo);
}

string DiffExpression::getContractA()
{
	return contractOne->getName();
}

string DiffExpression::getContractB()
{
	return contractTwo->getName();
}


double DiffExpression::getReverseValue()
{
	return getValue(askBidTwo, askBidOne);
}

double DiffExpression::getValue(string ab1, string ab2)
{

	double p1 = contractOne->getPrice().get(ab1);
	double p2 = contractTwo->getPrice().get(ab2);

	return getValue(contractOne->getName(), p1, contractTwo->getName(), p2);
}

double DiffExpression::getLastValue()
{
	double p1 = contractOne->getPrice().getLastPrice();
	double p2 = contractTwo->getPrice().getLastPrice();
	//TRACE_LOG("DiffExpression last price, %.2f, %.2f", p1, p2);
	return getValue(contractOne->getName(), p1, contractTwo->getName(), p2);
}

bool invalid(double price)
{
	return price < POSSIBLE_LOW_PRICE || price > POSSIBLE_HIGH_RPICE;
}

bool startWith(const char * s1, const char * s2);

double DiffExpression::getValue(string contract1, double p1, string contract2, double p2)
{
	if(invalid(p1) || invalid(p2)) {
		return INVALID_DIFF - 1;
	}

    double ret = sameSequence(contract1, contract2)
		         ? getValue(p1, p2)
		         : getValue(p2, p1);

    return ret;
}

double DiffExpression::getValue(double p1, double p2)
{
    return expressionOf1to2->getValueFor(p1) - expressionOf2to1->getValueFor(p2);
}

bool DiffExpression::sameSequence(string contract1, string contract2)
{
	return contractOne->getName().compare(contract1) == 0;
}

DiffExpression * DiffExpression::flip(string contract)
{
    DiffExpression * de = new DiffExpression(this->toString());
	if(de->contractOne->getName().compare(contract) == 0)
	{
		de->askBidOne = (de->askBidOne.compare("ask") == 0) ? "bid" : "ask";
	} else {
		de->askBidTwo = (de->askBidTwo.compare("ask") == 0) ? "bid" : "ask";
	}
	return de;
}

string DiffExpression::toString()
{
	return contractOne->getName() + "." + askBidOne + "-" + contractTwo->getName() + "." + askBidTwo;
}

bool DiffExpression::allowOrder()
{
	return this->contractOne->allowOrder() && this->contractTwo->allowOrder();
}
