// Condition.cpp: implementation of the Condition class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Condition.h"
#include "LogStore.h"

#include <iostream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*
Condition::Condition()
{

}
*/

Condition::Condition(string diffExp, string comparator, double diff)
{
	set(diffExp, comparator, diff);
}

Condition::~Condition()
{

}

void Condition::set(string diffExp, string comparator, double diff)
{
    setDiffExp(diffExp);
	this->comparator = comparator;
	this->diff = diff;
}

void Condition::setDiffExp(string diffExp)
{
	exp = new DiffExpression(diffExp);
}

//TODO: refactor
bool Condition::isMeet()
{
	double p = exp->getValue();
	if( !exp->allowOrder() ) return false;
	if( p < INVALID_DIFF ) return false;

	bool ret = comparator.compare("<=") == 0 ? p <= diff : p >= diff;
	//cout << p << endl;
	//TRACE_LOG("condition is %s, real diff is %.2f", this->toString().c_str(), p);
	if (ret) 
		TRACE_LOG("Meet condition %s, real diff is %.2f", this->toString().c_str(), p);
	return ret;
}

bool Condition::isMeet(string contract1, double p1, string contract2, double p2)
{
	double p = exp->getValue(contract1, p1, contract2, p2);
	if( !exp->allowOrder() ) return false;
	if( p < INVALID_DIFF ) return false;

	bool ret = comparator.compare("<=") == 0 ? p <= diff : p >= diff;
	//TRACE_LOG("Meet condition %s, real diff is %.2f", this->toString().c_str(), p);
	if(ret)
		TRACE_LOG("Meet condition %s, real diff is %.2f", this->toString().c_str(), p);
	return ret;

}

double Condition::calcDiff(string contract1, double p1, string contract2, double p2)
{
	return exp->getValue(contract1, p1, contract2, p2);
}

Condition * Condition::genFishCondition(string contract)
{
	TRACE_LOG("HERE genFishCondition");
	Condition * c = new Condition(this->exp->toString(), this->comparator, this->diff);
	c->exp = this->exp->flip(contract);
	return c;
}

string Condition::toString()
{
	char temp[1000];
	sprintf_s(temp,1000, "%s %s %.2f ", 		           
		           exp->toString().c_str(),
                   comparator.c_str(),
				   diff);
	return temp;
}

string Condition::getComparator()
{
	return this->comparator;
}

double Condition::getDiff()
{
	return this->diff;
}

string Condition::getContractA()
{
	return exp->getContractA();
}

string Condition::getContractB()
{
	return exp->getContractB();
}

Condition * Condition::copy()
{
	return new Condition(exp->toString(), comparator, diff);
}
