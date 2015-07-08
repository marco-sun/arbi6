// DayTradingStrategy.h: interface for the DayTradingStrategy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DAYTRADINGSTRATEGY_H__A42369BE_126A_4635_924D_E735056F3E0B__INCLUDED_)
#define AFX_DAYTRADINGSTRATEGY_H__A42369BE_126A_4635_924D_E735056F3E0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ArbitrageStrategy.h"
#include "TwoLegStrategy.h"

class DayTradingStrategy : public ArbitrageStrategy  
{
public:
	DayTradingStrategy();
	virtual ~DayTradingStrategy();

    void setKaiCangStrategy(TwoLegStrategy * kaiCangStrategy);
    void setPingCangStrategy(TwoLegStrategy * pingCangStrategy);
	void trig();
	void setStrategyA(string id);
	void setStrategyB(string id);
	bool hasStrategyB();

	static DayTradingStrategy * create(string id);

	TwoLegStrategy * getStrategyA();
	TwoLegStrategy * getStrategyB();
    TwoLegStrategy * createStrategyA();
    TwoLegStrategy * createStrategyB();

	void save();
	string toString();
	string toDesc();
	static void clear();
	void remove();
	//static void load();
	void start();
	void stop();
	void work();
	static list<DayTradingStrategy *> listAll();
    static DayTradingStrategy * get(string id);
	static bool exist(string id);
	void shootAnyway();
	void setStrategyBNull();

private:
	TwoLegStrategy * strategyA;
	TwoLegStrategy * strategyB;
    TwoLegStrategy * initSubStrategyAsNecessary(TwoLegStrategy * s, string idSuffix);
	static list<DayTradingStrategy *> dayTradingStrategies;
	void deleteThis();

};


#endif // !defined(AFX_DAYTRADINGSTRATEGY_H__A42369BE_126A_4635_924D_E735056F3E0B__INCLUDED_)
