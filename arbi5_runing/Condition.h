// Condition.h: interface for the Condition class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONDITION_H__9A3265CC_3E44_4D21_9717_9B23B98EE79D__INCLUDED_)
#define AFX_CONDITION_H__9A3265CC_3E44_4D21_9717_9B23B98EE79D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DiffExpression.h"
#include <string>
using namespace std;

class Condition  
{
public:
	//Condition();
	Condition(string diffExp, string comparator, double diff);
	virtual ~Condition();

	void set(string diffExp, string comparator, double diff);
	void setDiffExp(string diffExp);
	bool isMeet();
	bool isMeet(string contract1, double p1, string contract2, double p2);

	Condition * genFishCondition(string contract);
	string toString();
	double calcDiff(string contract1, double p1, string contract2, double p2);
	//string getExp();
	string getComparator();
	double getDiff();
	string getContractA();
	string getContractB();
	Condition * copy();

private:
	DiffExpression * exp;
	string comparator;
	double diff;
};

#endif // !defined(AFX_CONDITION_H__9A3265CC_3E44_4D21_9717_9B23B98EE79D__INCLUDED_)
