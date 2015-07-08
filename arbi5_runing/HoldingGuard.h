// HoldingGuard.h: interface for the HoldingGuard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HOLDINGGUARD_H__DC6AC68C_6FA3_442C_BAF6_C944B8418D86__INCLUDED_)
#define AFX_HOLDINGGUARD_H__DC6AC68C_6FA3_442C_BAF6_C944B8418D86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HoldingCounter.h"
#include <string>
using namespace std;

enum HoldingOperation
{
	INCREASE,
	DECREASE
};

class HoldingGuard  
{
public:
	HoldingGuard();
	virtual ~HoldingGuard();

	int getHoldingCount();
	HoldingCounter * getHoldingCountInstance();
	void resetCount();
	virtual void setHoldingCounter(HoldingCounter * holdingCounter);
	void setHoldingLimit(int low, int high);
	virtual void setHoldingOperation(enum HoldingOperation holdingOperation);
	int getHighHoldingLimit();
	int getLowHoldingLimit();

protected:
	HoldingCounter * holdingCounter;
	enum HoldingOperation holdingOperation;
    bool betweenLimit(int i);
	void initHoldingCounter();
	void updateCounter();
	string limitString();

private:

    //TODO: move these to HoldingCounter and make setHoldingLimit() not virtual

};

#endif // !defined(AFX_HOLDINGGUARD_H__DC6AC68C_6FA3_442C_BAF6_C944B8418D86__INCLUDED_)
