// TwoLegStrategy.h: interface for the TwoLegStrategy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TWOLEGSTRATEGY_H__9D1AED91_3F9F_486B_BE4F_F3B95E125134__INCLUDED_)
#define AFX_TWOLEGSTRATEGY_H__9D1AED91_3F9F_486B_BE4F_F3B95E125134__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ArbitrageStrategy.h"
#include "Leg.h"
#include "Condition.h"
#include "Order.h"

enum TwoLegStatus
{
	READY,
	FISHING,
	AFTER_FIRST_SHOOT,
	AFTER_SECOND_SHOOT,
	SECOND_LEG_MISSED,
	AFTER_CANCEL_FISH
};

class TwoLegStrategy : public ArbitrageStrategy  
{
public:
	TwoLegStrategy();
    TwoLegStrategy(Leg firstLeg, Leg secondLeg);
	virtual ~TwoLegStrategy();
	//void setHoldingOperation(enum HoldingOperation holdingOperation);
	bool holdingCountAllow();
	virtual void missedLegHandled();
	virtual bool isSecondLegMissed();
	virtual void finishOneSuite();
	virtual void trig();
	virtual void trig(time_t t);

	virtual void setLegA(Leg leg);
	virtual void setLegA(Leg * leg);
	virtual void setLegB(Leg leg);
	virtual void setLegB(Leg * leg);
	virtual void setCondition(Condition condition);
	virtual void setCondition(Condition * condition);

	virtual Leg getLegA();
	virtual Leg getLegB();
	virtual Condition getCondition();

	Order * getFirstOrder();
	Order * getSecondOrder();

	static void clear();
	static list<TwoLegStrategy *> listAll();
	static void add(void * p);
	void save();
	void remove();

	static TwoLegStrategy * create(string id);
	static void * createShootStrategy(string id);
	static void * createFishStrategy(string id);
	static void * createFishShootStrategy(string id);
	static TwoLegStrategy * getTwoLegStrategy(string id);

	virtual string toString();
	string toString2();
	virtual string toDesc();
    void validate(Condition condition);
	bool inTrading(time_t);
    virtual void correctCondition();
	virtual void setFish(bool s) ;
	virtual bool isFished();
	virtual void shootAnyway();

protected:

	//Order * lastOrder;
    Order * firstOrder;
	Order * secondOrder;

	enum TwoLegStatus twoLegStatus;
	Leg firstLeg;
	Leg secondLeg;
	Condition * condition;
    void afterSecondShootAction();
	bool validPrice();
	void firstOrderRejected();
    void startStopControl(time_t t);

private:
	//enum HoldingOperation holdingOperation;

	bool isKaiCang();
	bool isPingCang();
	bool isYiCang();
	static list<TwoLegStrategy *> twoLegStrategies;
	void genDealInfo();
	void workToHalt(time_t t);
	void haltToWork(time_t t);
	//enum StrategyStatus jumpToAfterOneSuite;
};

#endif // !defined(AFX_TWOLEGSTRATEGY_H__9D1AED91_3F9F_486B_BE4F_F3B95E125134__INCLUDED_)
