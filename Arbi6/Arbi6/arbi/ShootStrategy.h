// ShootStrategy.h: interface for the ShootStrategy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOOTSTRATEGY_H__45509D80_2ED1_4D11_81D2_7472025D0B3F__INCLUDED_)
#define AFX_SHOOTSTRATEGY_H__45509D80_2ED1_4D11_81D2_7472025D0B3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "TwoLegStrategy.h"

class ShootStrategy : public TwoLegStrategy 
{
public:
	ShootStrategy();
	virtual ~ShootStrategy();
	void trig();
	void trig(time_t t);
	static void * create(string id);
	void shootAnyway();
private:
	void readyAction();
	void afterFirstShootAction();
	//void afterSecondShootAction();

};

#endif // !defined(AFX_SHOOTSTRATEGY_H__45509D80_2ED1_4D11_81D2_7472025D0B3F__INCLUDED_)
