// FishShootStrategy.h: interface for the FishShootStrategy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FISHSHOOTSTRATEGY_H__990EF097_BB28_40A1_A309_B08DEE613B08__INCLUDED_)
#define AFX_FISHSHOOTSTRATEGY_H__990EF097_BB28_40A1_A309_B08DEE613B08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TwoLegStrategy.h"
#include "FishStrategy.h"
#include "ShootStrategy.h"

class FishShootStrategy : public TwoLegStrategy  
{
public:
	FishShootStrategy(string id);
	virtual ~FishShootStrategy();
	bool isFish;
	void setFish(bool s);
	static void * create(string id);

    void setLegA(Leg * leg);
	Leg getLegA();
	Leg getLegB();
    void setLegB(Leg * leg);
    void setCondition(Condition * condition);

	void start();
	void stop();
	string toString();
	string toDesc();
	void setHoldingOperation(enum HoldingOperation holdingOperation);
	void correctCondition();
	void setHoldingCounter(HoldingCounter * holdingCounter);
	bool isFished();
	Condition getCondition();
	bool isSecondLegMissed();
	void missedLegHandled();
	void shootAnyway();

private:
	FishStrategy * fishStrategy;
	ShootStrategy * shootStrategy;

};

#endif // !defined(AFX_FISHSHOOTSTRATEGY_H__990EF097_BB28_40A1_A309_B08DEE613B08__INCLUDED_)
