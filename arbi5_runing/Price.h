// Price.h: interface for the Price class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRICE_H__05BB9207_DBC0_45EF_B043_7E9D8E2538B8__INCLUDED_)
#define AFX_PRICE_H__05BB9207_DBC0_45EF_B043_7E9D8E2538B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
using namespace std;

class Price  
{
public:
	char debug[16];
	Price();
	virtual ~Price();

	double get(string askOrBid);
	double getAsk();
	double getBid();
	double getLastPrice();
	void set(time_t t, double ask, double bid);
	void set(time_t t, double ask, double bid, double lastPrice);
	bool isValid();
	static Price SBH4;
	static Price SBN4;
	static Price SBK4;
private:
	time_t time;
	string contract;
	double ask;
	double bid;
	double lastPrice;
};

#endif // !defined(AFX_PRICE_H__05BB9207_DBC0_45EF_B043_7E9D8E2538B8__INCLUDED_)
