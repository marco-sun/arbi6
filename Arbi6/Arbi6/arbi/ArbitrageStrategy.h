// ArbitrageStrategy.h: interface for the ArbitrageStrategy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARBITRAGESTRATEGY_H__81D29BD5_9F7A_48AB_B746_D1E99756F4B6__INCLUDED_)
#define AFX_ARBITRAGESTRATEGY_H__81D29BD5_9F7A_48AB_B746_D1E99756F4B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Strategy.h"
#include "HoldingGuard.h"
#include "Price.h"

class ArbitrageStrategy : public Strategy, public HoldingGuard  
{
public:
	ArbitrageStrategy();
	virtual ~ArbitrageStrategy();
	virtual void trig() = 0;




};

#endif // !defined(AFX_ARBITRAGESTRATEGY_H__81D29BD5_9F7A_48AB_B746_D1E99756F4B6__INCLUDED_)
