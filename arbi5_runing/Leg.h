// Leg.h: interface for the Leg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LEG_H__8D73EE6F_54B1_49CA_8E1D_502CCB18145F__INCLUDED_)
#define AFX_LEG_H__8D73EE6F_54B1_49CA_8E1D_502CCB18145F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Order.h"
#include "Dealer.h"
#include "Contract.h"
#include <string>
using namespace std;

class Leg  
{
public:
	Leg();
	Leg(string account,
	enum BuySell buySell,
	enum KaiPing kaiPing,
	string contract,
	int lot,
	int offset);
	virtual ~Leg();
	
	void set(string account,
	enum BuySell buySell,
	enum KaiPing kaiPing,
	string contract,
	int lot,
	int offset);
	bool isKaiCang();
	bool isPingCang();
	bool isPingJin();
	bool isPingCangOrPingJin();

    Order * shoot(int cancelTime);
	Order * fishAt(double);
	Order * fishEqualFirst();
	Order * fishFirst();
	string getContract();
	int getLot();
	enum BuySell getBuySell();
	enum KaiPing getKaiPing();
	int getOffSet();

	string toString();
	string toDes();
	string kaiPingtoDes();
	bool hasValidPrice();
	bool inTrading(time_t t); 
	string getConcernPrice();
	double getConcernedPrice();
	bool notFirst(double p);
	bool isFirst(double p);
	bool isOversea();
	bool isDomestic();
	double fishFirstPrice();
	double fishEqualFirstPrice();
	double shootPrice();
	double betterFishPrice(double fishPrice);

private:
    string account;
	//string contract;
	Contract * contract;
	enum BuySell buySell;
	enum KaiPing kaiPing;
	int lot;
	int offset;
	double increment;
	Dealer * dealer;

	Order * genShootOrder();
	Order * genFishOrder(double os);
	string kaiPingString();
	Order * genOrder(string contract, enum BuySell buySell, enum KaiPing kaiPing, int lot, double Price);
};

#endif // !defined(AFX_LEG_H__8D73EE6F_54B1_49CA_8E1D_502CCB18145F__INCLUDED_)
