// Exchange.h: interface for the Exchange class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXCHANGE_H__610D2B26_2236_4762_A51B_B47B4FF219BB__INCLUDED_)
#define AFX_EXCHANGE_H__610D2B26_2236_4762_A51B_B47B4FF219BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <time.h>
#include <list>
using namespace std;

#define TIME_OFFSET 2
#define ONE_DAY 24 * 60 *60

enum Market {
	CBOT,
	NBOT_SB,
	NBOT_CT,
	NYMEX,
	LME,
	BMD,
	TOCOM,
	SHANG_HAI,
	ZHENG_ZHOU,
	DA_LIAN,
	ZHONGJIN,
	LMELSU,
	BOHAI,
	SH,
	SZ
};

class TimeInterval
{
public:
	TimeInterval(int startHour, int startMinute, int startSecond, int endHour, int endMinute, int endSecond);
    bool include(time_t t);

private:
	struct tm start;
	struct tm end;
	bool nextDay(time_t t);
	void initStartEnd();

};

class Exchange  
{
public:
	virtual ~Exchange();
	static Exchange * getInstance(enum Market m);
	bool inTrading(time_t t);
	static Exchange * tocom;
	static Exchange * nbot_SB;
	static Exchange * nbot_CT;
	static Exchange * cbot;
	static Exchange * nymex;
	static Exchange * lme;
	static Exchange * bmd;
	static Exchange * shanghai;
	static Exchange * dalian;
	static Exchange * zhengzhou;
	static Exchange * zhongjin;
	static Exchange * lmelsu;
	static Exchange * bohai;
	static Exchange * sh;
	static Exchange * sz;
	void add(TimeInterval * ti);

private:
	Exchange(enum Market m);
	static void initAll();
	list<TimeInterval *> timeIntervals;
};

#endif // !defined(AFX_EXCHANGE_H__610D2B26_2236_4762_A51B_B47B4FF219BB__INCLUDED_)
