// LogPriceStrategy.h: interface for the LogPriceStrategy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGPRICESTRATEGY_H__ABDECC7D_4F78_4948_B618_F0CB9077D92A__INCLUDED_)
#define AFX_LOGPRICESTRATEGY_H__ABDECC7D_4F78_4948_B618_F0CB9077D92A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Strategy.h"
#include <string>
using namespace std;

class LogPriceStrategy : public Strategy  
{
public:
	LogPriceStrategy();
	LogPriceStrategy(int i);
	virtual ~LogPriceStrategy();
	void trig();
	void set(string contract);
private:
	string contract;
};

#endif // !defined(AFX_LOGPRICESTRATEGY_H__ABDECC7D_4F78_4948_B618_F0CB9077D92A__INCLUDED_)
