// ArbitrageComparablePriceExpression.h: interface for the ArbitrageComparablePriceExpression class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARBITRAGECOMPARABLEPRICEEXPRESSION_H__0D61530F_869B_4837_A817_80634A220B62__INCLUDED_)
#define AFX_ARBITRAGECOMPARABLEPRICEEXPRESSION_H__0D61530F_869B_4837_A817_80634A220B62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4786)

#include <string>
#include <list>
using namespace std;

class ArbitrageComparablePriceExpression  
{
public:
	ArbitrageComparablePriceExpression();

	virtual ~ArbitrageComparablePriceExpression();
	double getValueFor(double para);

    void set(string exp);
	void set(double rate, double factor, double cons);

	static void add(string commodityA, string commodityB, string exp);
	static ArbitrageComparablePriceExpression * find(string commodityA, string commodityB);


private:
    static list<ArbitrageComparablePriceExpression *> expressions;
	string commodityA;
	string commodityB;
	double rate;
	double factor;
	double cons;
	ArbitrageComparablePriceExpression(string commodityA, string commodityB, string exp);
	static ArbitrageComparablePriceExpression * defaultExp;
};

#endif // !defined(AFX_ARBITRAGECOMPARABLEPRICEEXPRESSION_H__0D61530F_869B_4837_A817_80634A220B62__INCLUDED_)
