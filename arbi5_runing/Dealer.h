// Dealer.h: interface for the Dealer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEALER_H__3177AEB8_3E88_48E3_9E29_0DEA66D87FFD__INCLUDED_)
#define AFX_DEALER_H__3177AEB8_3E88_48E3_9E29_0DEA66D87FFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Order.h"

class Dealer  
{
public:
	static void initAsNeccessaryFor(string contract);
    static Dealer * getDealerFor(string contract);
	static bool useRealDealer;
	static void cancel(Order * order);

	Dealer();
	virtual ~Dealer();

	virtual void placeOrder(Order * order)=0;
	virtual void cancelOrder(Order * order)=0;
	//virtual void redisStart()=0;
	//virtual void redisPause()=0;
	//virtual void redisStop()=0;
	static Dealer * oversea;
private:
    static bool domesticNotInited;
    static bool overseaNotInited;
	static Dealer * domestic;
	static void initDomestic();
	static void initOversea();
    
};

class DumbDealer : public Dealer
{
public:
	DumbDealer();
	virtual ~DumbDealer();
	void placeOrder(Order * order);
	void cancelOrder(Order * order);
};

#endif // !defined(AFX_DEALER_H__3177AEB8_3E88_48E3_9E29_0DEA66D87FFD__INCLUDED_)
