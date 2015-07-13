// Leg.cpp: implementation of the Leg class.
//
//////////////////////////////////////////////////////////////////////

#include "Leg.h"
#include "Price.h"
#include "Contract.h"
#include "Dealer.h"
#include "../LogStore.h"
#include "../utils/Utils.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Leg::Leg()
{
	this->account = "default";
}

Leg::~Leg()
{

}

bool Leg::isKaiCang()
{
	return this->kaiPing == KAI_CANG;
}

bool Leg::isPingCang()
{
	return this->kaiPing == PING_CANG;
}

bool Leg::isPingJin()
{
	return this->kaiPing == PING_JIN;
}

bool Leg::isPingCangOrPingJin()
{
	return isPingCang() || isPingJin();
}

void Leg::set(string account,
	enum BuySell buySell,
	enum KaiPing kaiPing,
	string contract,
	int lot,
	int offset)
{
	this->account = account;
	this->buySell = buySell;
	this->kaiPing = kaiPing;
	this->contract = Contract::get(contract);
	this->lot = lot;
	this->offset = offset;
	this->increment = Commodity::findFor(contract)->increment;
	this->dealer = Dealer::getDealerFor(contract);

}

Leg::Leg(string account,
	enum BuySell buySell,
	enum KaiPing kaiPing,
	string contract,
	int lot,
	int offset)
{
	set(account, buySell, kaiPing, contract, lot, offset);
}

Order * Leg::shoot(int cancelTime)
{
	assert(dealer != NULL);
	Order * o = genShootOrder();
	if(lot != 0){
	    dealer->placeOrder(o);
	    o->cancelAfter(cancelTime);
	}
	return o;
}

Order * Leg::genShootOrder()
{
	Price p = contract->getPrice();
    double os = offset * increment;
	double shootPrice = (buySell == BUY) ? p.getAsk() + os : p.getBid() - os;
	return genOrder(contract->getName(), buySell, kaiPing, lot, shootPrice);
}

double Leg::shootPrice()
{
	Price p = contract->getPrice();
	return (buySell == BUY) ? p.getAsk() : p.getBid();
}

double Leg::fishFirstPrice()
{
	double p = fishEqualFirstPrice();
	return (buySell == BUY) ? p + increment : p - increment;
}

double Leg::betterFishPrice(double fishPrice)
{
	return (buySell == BUY) ? fishPrice - increment : fishPrice + increment;
}

double Leg::fishEqualFirstPrice()
{
	Price p = contract->getPrice();
	return (buySell == BUY) ? p.getBid() : p.getAsk();
}

Order * Leg::genFishOrder(double os)
{
	Price p = contract->getPrice();
	double fishPrice = (buySell == BUY) ? p.getBid() + os : p.getAsk() - os;
    //printf("Leg::genFishOrder %f\n",fishPrice);
	return genOrder(contract->getName(), buySell, kaiPing, lot, fishPrice);
}

//TODO: remove contract from para
Order * Leg::genOrder(string contract, enum BuySell buySell, enum KaiPing kaiPing, int lot, double Price)
{
	//this->contract->increaseOrderCount();
	Order * o = Order::create(contract, buySell, kaiPing, lot, Price);
	TRACE_LOG("genOrder,id=%s",o->getId().c_str());
	return o;
}

Order * Leg::fishAt(double fishPrice)
{
    Order * o = genOrder(contract->getName(), buySell, kaiPing, lot, fishPrice);
	dealer->placeOrder(o);
	return o;
}

//TODO: refactor to use fishAt(double fishPrice)
Order * Leg::fishFirst()
{
	assert(dealer != NULL);
	Order * o = genFishOrder(this->increment);
	dealer->placeOrder(o);
	return o;
}

Order * Leg::fishEqualFirst()
{
	assert(dealer != NULL);
	Order * o = genFishOrder(0);
	dealer->placeOrder(o);
	return o;
}

string Leg::getContract()
{
	return contract->getName();
}

string Leg::kaiPingString()
{
	if(this->kaiPing == KAI_CANG) return "KC ";
	if(this->kaiPing == PING_CANG) return "PC ";
	if(this->kaiPing == PING_JIN) return "PJ ";
	return "Unknown kaiPing";
}

string Leg::kaiPingtoDes()
{
	if(this->kaiPing == KAI_CANG) return "开仓";
	if(this->kaiPing == PING_CANG) return "平仓";
	if(this->kaiPing == PING_JIN) return "平今";
	return "Unknown kaiPing";
}

string Leg::toString()
{
	string ret = account;
	ret += (this->buySell == BUY) ? " BUY " : " SELL ";
	ret += kaiPingString();
	ret += contract->getName();
	ret += " ";
	ret += Utils::intToString(this->lot);
	ret += " ";
	ret += Utils::intToString(this->offset);
	ret += " ";
	return ret;
}

string Leg::toDes()
{
	string ret = "";
	ret += this->kaiPingtoDes();
	ret += (this->buySell == BUY) ? "买" : "抛";
	ret += this->contract->getName();
	ret += " ";
	ret += Utils::intToString(this->lot);
	ret += "手 加";
	ret += Utils::intToString(this->offset);
	return ret;
}

bool Leg::hasValidPrice()
{
	return contract->hasValidPrice();
}

int Leg::getLot()
{
	return this->lot;
}

enum BuySell Leg::getBuySell()
{
	return this->buySell;
}

string Leg::getConcernPrice()
{
	string s;
	s = buySell == BUY ? getContract() + ".ask" : getContract() + ".bid";
	TRACE_LOG("getConcernPrice : %s , %s", buySell == BUY ? "BUY" : "SELL", s.c_str()); 
    return s;
}

double Leg::getConcernedPrice()
{
	Price p = this->contract->getPrice();
	return buySell == BUY ? p.getAsk() : p.getBid();
}

enum KaiPing Leg::getKaiPing()
{
	return this->kaiPing;
}

int Leg::getOffSet()
{
	return this->offset;
}

bool Leg::inTrading(time_t t)
{
	if (this->contract->inTrading(t))
		return true;
	return false;
}


bool Leg::isFirst(double p)
{
	Price price = contract->getPrice();
	return (buySell == BUY) ? price.getBid() <= p : price.getAsk() >= p;
}

bool Leg::notFirst(double p)
{
	return !isFirst(p);
}

bool Leg::isOversea()
{
	return contract->isOversea();
}

bool Leg::isDomestic()
{
	return !isOversea();
}