// IntentionPair.h: interface for the IntentionPair class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTENTIONPAIR_H__9CDD46A9_008A_43F0_8792_3A5ECE523182__INCLUDED_)
#define AFX_INTENTIONPAIR_H__9CDD46A9_008A_43F0_8792_3A5ECE523182__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "contract.h"
#include "DiffExpression.h"
#include <string>
#include <fstream> 
using namespace std;

enum IntentionOrder
{
	ONE,
	TWO
};

enum Sequence
{
	UP,
	DOWN
};

class IntentionPair  
{
public:
	char debug[16];
	virtual ~IntentionPair();

	static IntentionPair * get(string name);
	static IntentionPair * create(string name, string contractA, string contractB, string diffExp, int lotA, int lotB);
	static IntentionPair * create(string name, string contractA, string contractB, string diffExp, int lotA, int lotB, double refParaOne, double refParaTwo, enum IntentionOrder orderBy);
	static IntentionPair * add(string name, string contractA, string contractB, string diffExp, int lotA, int lotB, double refParaOne, double refParaTwo, enum IntentionOrder orderBy);
	static IntentionPair * add(string name, string contractA, string contractB, string diffExp, int lotA, int lotB);

	static void clear();

	Contract * getContract(string leg);
	string getName();
	string getContractA();
	string getContractB();
	
	double getFirstDiff();
	double getSecondDiff();
	double getlastPriceDiff();
	double getFirstBid();
	double getFirstAsk();
	double getSecondBid();
	double getSecondAsk();
    double getFirstRatio();
	double getSecondRatio();
	int getLotA();
	int getLotB();

	string toString();
    static list<IntentionPair *> listAll();
	static list<IntentionPair *> sortListAll(enum Sequence SortBy);
	static IntentionPair * findFor(string strategyId);

	double importProfit();
    double sortValue();
	double importValue;
    string getDateFileName();
	void writeToFile();
private:
	string name;
	Contract * contractA;
	Contract * contractB;
	int lotA;
	int lotB;
	static list<IntentionPair *> intentionPairs;
	double refParaOne;
	double refParaTwo;
	enum IntentionOrder orderBy;
	ofstream dataFile;
	DiffExpression * diffExp;
	static bool exist(string name);
    IntentionPair(string name, string contractA, string contractB, string diffExp, int lotA, int lotB,  double refParaOne, double refParaTwo, enum IntentionOrder orderBy);
    void set(string contractA, string contractB, string firstDiffExp, int lotA, int lotB, double refParaOne, double refParaTwo, enum IntentionOrder orderBy);	
};

template<>
struct std::greater<IntentionPair *>
{
	bool operator()(IntentionPair * p1, IntentionPair * p2) const
	{
			return (p1->importValue > p2->importValue);
	}
};

template<>
struct std::less<IntentionPair *>
{
	bool operator()(IntentionPair * p1, IntentionPair * p2) const
	{
			return (p1->importValue < p2->importValue);
	}
};

#endif // !defined(AFX_INTENTIONPAIR_H__9CDD46A9_008A_43F0_8792_3A5ECE523182__INCLUDED_)
