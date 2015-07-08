// ArbitrageComparablePriceExpression.cpp: implementation of the ArbitrageComparablePriceExpression class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ArbitrageComparablePriceExpression.h"
#include "Commodity.h"
#include "Contract.h"
#include "LogStore.h"
#include <sstream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//TODO move this to find(string commodityA, string commodityB)
ArbitrageComparablePriceExpression::ArbitrageComparablePriceExpression()
{
	string defaultExp = "p * 1 * 1 + 0";
    set(defaultExp);
}

ArbitrageComparablePriceExpression::ArbitrageComparablePriceExpression(string commodityA, string commodityB, string exp)
{
	this->commodityA = commodityA;
	this->commodityB = commodityB;
    set(exp);
}



ArbitrageComparablePriceExpression::~ArbitrageComparablePriceExpression()
{

}

double ArbitrageComparablePriceExpression::getValueFor(double para)
{
	return para * rate * factor + cons;
}

void ArbitrageComparablePriceExpression::set(string exp)
{
    std::stringstream stream(exp);
	string a;
	stream >> a >> a >> rate >> a >> factor >> a >> cons;
}

void ArbitrageComparablePriceExpression::set(double rate, double factor, double cons)
{
	this->rate = rate;
	this->factor = factor;
	this->cons = cons;
}

list<ArbitrageComparablePriceExpression *> ArbitrageComparablePriceExpression::expressions;

ArbitrageComparablePriceExpression * ArbitrageComparablePriceExpression::defaultExp = new ArbitrageComparablePriceExpression("", "", "P * 1 * 1 + 0");

void ArbitrageComparablePriceExpression::add(string commodityA, string commodityB, string exp)
{
    expressions.push_back(new ArbitrageComparablePriceExpression(commodityA, commodityB, exp));
}

bool startWith(const char * s1, const char * s2);

ArbitrageComparablePriceExpression * ArbitrageComparablePriceExpression::find(string contractA, string contractB)
{

    list<ArbitrageComparablePriceExpression *>::iterator the_iterator = expressions.begin();

    while( the_iterator != expressions.end() ) {
        if(startWith(contractA.c_str(), (*the_iterator)->commodityA.c_str())
			&& startWith(contractB.c_str(), (*the_iterator)->commodityB.c_str())) return (*the_iterator);
        the_iterator++;
	}
	//TRACE_LOG("expressions.size = %d", expressions.size());
	TRACE_LOG("Use default expression for %s-%s", contractA.c_str(),contractB.c_str());
	return defaultExp;
}
