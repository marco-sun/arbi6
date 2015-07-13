// DiffExpression.h: interface for the DiffExpression class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIFFEXPRESSION_H__A03ECC42_E953_4225_AE1E_9F45FD07E526__INCLUDED_)
#define AFX_DIFFEXPRESSION_H__A03ECC42_E953_4225_AE1E_9F45FD07E526__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include "Contract.h"
using namespace std;

#define INVALID_DIFF -10000000

class DiffExpression  
{
public:
	//DiffExpression();
	DiffExpression(string exp);
	virtual ~DiffExpression();

	void set(string exp);
	double getValue();
	double getValue(string contract1, double p1, string contract2, double p2);
	double getLastValue();
	double getReverseValue();
	DiffExpression * flip(string contract);
	string toString();
	string getContractA();
	string getContractB();
	bool allowOrder();

private:
    //string contractOne;
	Contract * contractOne;
	string askBidOne;
	//string contractTwo;
    Contract * contractTwo;
	string askBidTwo;
	double getValue(string ab1, string ab2);
	ArbitrageComparablePriceExpression * expressionOf1to2;
	ArbitrageComparablePriceExpression * expressionOf2to1;
	bool sameSequence(string contract1, string contract2);
    double getValue(double p1, double p2);

};

#endif // !defined(AFX_DIFFEXPRESSION_H__A03ECC42_E953_4225_AE1E_9F45FD07E526__INCLUDED_)
