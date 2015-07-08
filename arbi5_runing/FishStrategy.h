// FishStrategy.h: interface for the FishStrategy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FISHSTRATEGY_H__0E712B76_0862_402B_8CA0_B41EED7A6386__INCLUDED_)
#define AFX_FISHSTRATEGY_H__0E712B76_0862_402B_8CA0_B41EED7A6386__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TwoLegStrategy.h"

class FishStrategy : public TwoLegStrategy  
{
public:
	FishStrategy();
	virtual ~FishStrategy();
	static void * create(string id);
	void trig();
	void trig(time_t t);
	void setCondition(Condition * condition);

private:
	bool fishConditionOk;
	Condition * fishCondition;

	void genFishConditionAsNeccessary();
	void readyAction();
	void fishingAction();
	string genFishCondition();
	void afterCancelFishAction();
	bool notFirst();
	bool secondLegMoved();
	double concernedSecondLegPriceAtFishing;
	void firstOrderCanceled();
	bool meetCondition(double firstLegPrice, double secondLegPrice);
	void updateFishStatus();
	double calcFishPrice();
	bool conditionNotMeet();
	bool firstWillMeet();
	bool firstMeetCondition();
	bool equalFirstMeetCondition();
	void cancelAsNecessary();
    void cancelBecause(const char * s);

};

#endif // !defined(AFX_FISHSTRATEGY_H__0E712B76_0862_402B_8CA0_B41EED7A6386__INCLUDED_)
