// commodity.h: interface for the Commodity class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMODITY_H__FE3D9BD5_E80A_40E5_AD88_462F0235D904__INCLUDED_)
#define AFX_COMMODITY_H__FE3D9BD5_E80A_40E5_AD88_462F0235D904__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#pragma warning(disable : 4786)

#include "ArbitrageComparablePriceExpression.h"
#include "Exchange.h"
#include <string>
#include <list>
using namespace std;

class Commodity  
{
public:
	char debug[16];
	virtual ~Commodity();

	static Commodity * get(string name);
	static Commodity * findFor(string contract);

    int getDefaultArbitrageLot();
	bool isOnTradingNow();
	double calcArbitrageComparablePriceFor(double p, Commodity * comparedCommodity);
	bool isDomestic();
	bool isOversea();
	double increment;
	string getName();
	bool inTrading(time_t t);
	char getExchangeNumberForYiSheng();

private:
	static list<Commodity*> commodities;
    static bool inited;
	static void addCommodity(string name, 
                             enum Market market,
                             double increment);
	string name;
	enum Market market;
	Exchange * exchange;

	Commodity();
	Commodity(string name, 
		      enum Market market,
			  double increment);
	static void initAsNeccissary();
};

#endif // !defined(AFX_COMMODITY_H__FE3D9BD5_E80A_40E5_AD88_462F0235D904__INCLUDED_)
