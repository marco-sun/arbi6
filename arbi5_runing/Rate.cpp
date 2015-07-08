// Rate.cpp: implementation of the Rate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Rate.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Rate::Rate()
{

}

Rate::~Rate()
{

}

void Rate::setValue(string currency, double value)
{
	rates.insert(RateMap::value_type(currency, value));
}

double Rate::getValue(string currency)
{
	RateMap::iterator iterator = rates.find(currency);
	assert(iterator != rates.end());

	return iterator->second;
}

RateMap Rate::rates;
