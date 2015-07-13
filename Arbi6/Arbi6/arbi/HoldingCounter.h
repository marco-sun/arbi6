// HoldingCounter.h: interface for the HoldingCounter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HOLDINGCOUNTER_H__2B2EE40F_1739_4A20_AF6A_0F5575354A33__INCLUDED_)
#define AFX_HOLDINGCOUNTER_H__2B2EE40F_1739_4A20_AF6A_0F5575354A33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Windows.h>

class HoldingCounter  
{
public:
	HoldingCounter();
	HoldingCounter(int suite);
	void resetCount();
	virtual ~HoldingCounter();
	void increase();
	void decrease();
	int getSuite();
	void setLimit(int low, int high);
	bool betweenLimit(int i);
	int getLowLimit();
    int getHighLimit();
private:
	int suite;
	int lowLimit;
	int highLimit;
	CRITICAL_SECTION cs;
	void add(int i);


};

#endif // !defined(AFX_HOLDINGCOUNTER_H__2B2EE40F_1739_4A20_AF6A_0F5575354A33__INCLUDED_)
