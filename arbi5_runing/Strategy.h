// Strategy.h: interface for the Strategy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRATEGY_H__500B6F7E_FF49_4660_94C1_8BCE6BB4FE84__INCLUDED_)
#define AFX_STRATEGY_H__500B6F7E_FF49_4660_94C1_8BCE6BB4FE84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Order.h"
#include <string>
#include <list>
using namespace std;

enum StrategyStatus
{
	WORK,
    SHOULD_STOP,
	STOP,
	HALT
};



class Strategies;

class Strategy  
{
public:
	Strategy();
	virtual ~Strategy();

	static void * getTwoLegStrategy(string name);
	static void * getShootStrategy(string name);

	static bool regist(Strategy * s);
	static bool alreadyExist(string id); 
	static Strategies strategies;

	virtual void trig() = 0;
	virtual void start();
	virtual void stop();
	virtual	void work();
	bool isNotStop();
	bool isStop();
	bool isWork();


	Orders * getOrders();
	void setId(string id);
	string getId();
    bool isStart();
	bool isHalt();


protected:
	string type;
	string id;
	enum StrategyStatus strategyStatus;
	bool threadCreated;


};

class Strategies
{
    //Strategies * selectWhereIdStartWith(string str);
public:
	void add(Strategy * s);
	Strategy * get(string id);
private:
	list<Strategy *> strategyList;
};

#endif // !defined(AFX_STRATEGY_H__500B6F7E_FF49_4660_94C1_8BCE6BB4FE84__INCLUDED_)
