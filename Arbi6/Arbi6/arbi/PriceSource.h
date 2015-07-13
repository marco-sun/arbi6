// PriceSource.h: interface for the PriceSource class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRICESOURCE_H__DE2C721C_DB87_4F74_9734_63BA714A30AB__INCLUDED_)
#define AFX_PRICESOURCE_H__DE2C721C_DB87_4F74_9734_63BA714A30AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#pragma warning(disable : 4786)
#include <string>
using namespace std;

class PriceSource  
{
public:
	PriceSource();
	virtual ~PriceSource();
	static void listen(string contract);
	static void init(int marketflag=0); //0:both, 1:demestic, 2:oversea

	virtual	void start()=0;
	virtual void add(string contract)=0;

	static PriceSource * domastic;
	static PriceSource * oversea;

	static void useDumbPriceSource();
	static void useRealPriceSource();
};


class DumbPriceSource : public PriceSource
{
public:
	void start(){};
	void add(string contract){};
};

class Pricing  
{
public:

	Pricing(string contract, double ask, double bid);
	Pricing();
	virtual ~Pricing();

	double ask;
	double bid;
	double lastprice;
	string contract;

};

#endif // !defined(AFX_PRICESOURCE_H__DE2C721C_DB87_4F74_9734_63BA714A30AB__INCLUDED_)
