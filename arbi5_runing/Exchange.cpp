// Exchange.cpp: implementation of the Exchange class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Exchange.h"
#include "LogStore.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Exchange::Exchange(enum Market m)
{

}

Exchange::~Exchange()
{

}

Exchange * Exchange::lmelsu = NULL;
Exchange * Exchange::tocom = NULL;
Exchange * Exchange::cbot = NULL;
Exchange * Exchange::nbot_SB = NULL;
Exchange * Exchange::nbot_CT = NULL;
Exchange * Exchange::nymex = NULL;
Exchange * Exchange::lme = NULL;
Exchange * Exchange::bmd = NULL;
Exchange * Exchange::shanghai = NULL;
Exchange * Exchange::dalian = NULL;
Exchange * Exchange::zhengzhou = NULL;
Exchange * Exchange::zhongjin = NULL;
Exchange * Exchange::bohai = NULL;

Exchange * Exchange::getInstance(enum Market m)
{
	if(tocom == NULL) {
		initAll();
	}
	switch(m)
	{
        case TOCOM:
	        return tocom;
		case CBOT:
			return cbot;
		case NBOT_SB:
			return nbot_SB;
		case NBOT_CT:
			return nbot_CT;
		case NYMEX:
			return nymex;
		case LME:
			return lme;
		case BMD:
			return bmd;
		case SHANG_HAI:
			return shanghai;
		case DA_LIAN:
			return dalian;
		case ZHENG_ZHOU:
			return zhengzhou;
		case ZHONGJIN:
			return zhongjin;
		case LMELSU:
			return lmelsu;
		case BOHAI:
			return bohai;
	}
	TRACE_LOG("Unkown Exchange.%i",m);
	assert(false);
	return NULL;
}

bool Exchange::inTrading(time_t t)
{
	list<TimeInterval *>::iterator the_iterator = timeIntervals.begin();

    while( the_iterator != timeIntervals.end() ) {
		if  ((*the_iterator)->include(t))
		{
			return true;
		}
        the_iterator++;
	}
	return false;
}

void Exchange::initAll()
{
	zhongjin = new Exchange(ZHONGJIN);
	zhongjin->add(new TimeInterval(9, 15, 0, 11, 30, 00));
	zhongjin->add(new TimeInterval(13, 0, 0, 15, 15, 00));

	cbot = new Exchange(CBOT);
	cbot->add(new TimeInterval(0, 0, 0, 23, 59, 59));

	nbot_SB = new Exchange(NBOT_SB);
	nbot_SB->add(new TimeInterval(14, 30, 0, 23, 59, 59));
	nbot_SB->add(new TimeInterval(0, 0, 0, 3, 0, 0));

	nbot_CT = new Exchange(NBOT_CT);
	nbot_CT->add(new TimeInterval(9, 0, 0, 23, 59, 59));
	nbot_CT->add(new TimeInterval(0, 0, 0, 3, 20, 0));

	nymex = new Exchange(NYMEX);
	nymex->add(new TimeInterval(0, 0, 0, 23, 59, 59));
	//nymex->add(new TimeInterval(0, 0, 0, 6, 0, 0));	

    lme = new Exchange(LME);
	lme->add(new TimeInterval(9, 0, 0, 23, 59, 59));
    lme->add(new TimeInterval(0, 0, 0, 3, 0, 0));

	lmelsu = new Exchange(LMELSU);
	lmelsu->add(new TimeInterval(16, 45, 0, 23, 59, 59));
    lmelsu->add(new TimeInterval(0, 0, 0, 1, 25, 0));

	bmd = new Exchange(BMD);
	bmd->add(new TimeInterval(10, 30, 0, 12, 30, 0));
	bmd->add(new TimeInterval(15, 0, 0, 18, 0, 0));

    tocom = new Exchange(TOCOM);
	tocom->add(new TimeInterval(8, 0, 0, 14, 30, 0));

	shanghai = new Exchange(SHANG_HAI);
	shanghai->add(new TimeInterval(9, 0, 0, 10, 14, 30));
	shanghai->add(new TimeInterval(10, 30, 0, 11, 29, 30));
	shanghai->add(new TimeInterval(13, 30, 0, 14, 59, 30));
	shanghai->add(new TimeInterval(21, 0, 0, 23, 59, 59));
	shanghai->add(new TimeInterval(0, 0, 0, 0, 59, 30));

	dalian = new Exchange(DA_LIAN);
	dalian->add(new TimeInterval(9, 0, 0, 10, 14, 30));
	dalian->add(new TimeInterval(10, 30, 0, 11, 29, 30));
	dalian->add(new TimeInterval(13, 30, 0, 14, 59, 30));
	dalian->add(new TimeInterval(21, 0, 0, 23, 59, 59));
	dalian->add(new TimeInterval(0, 0, 0, 2, 29, 30));

	zhengzhou = new Exchange(ZHENG_ZHOU);
	zhengzhou->add(new TimeInterval(9, 0, 0, 10, 14, 30));
	zhengzhou->add(new TimeInterval(10, 30, 0, 11, 29, 30));
	zhengzhou->add(new TimeInterval(13, 30, 0, 14, 59, 30));
	zhengzhou->add(new TimeInterval(21, 0, 0, 23, 29, 29));

	bohai = new Exchange(BOHAI);
	bohai->add(new TimeInterval(9, 0, 0, 16, 0, 0));
	bohai->add(new TimeInterval(19, 0, 0, 23, 59, 59));
	bohai->add(new TimeInterval(0, 0, 0, 3, 0, 0));
}

void Exchange::add(TimeInterval * ti)
{
	timeIntervals.push_back(ti);
}

//====================================================================================
TimeInterval::TimeInterval(int startHour, int startMinute, int startSecond, 
						   int endHour, int endMinute, int endSecond)
{
	time_t t = time(NULL);

	start = *localtime(&t);
	start.tm_hour = startHour;
	start.tm_min = startMinute;
	start.tm_sec = startSecond;

	end = *localtime(&t);
	end.tm_hour = endHour;
	end.tm_min = endMinute;
	end.tm_sec = endSecond;
}

bool TimeInterval::include(time_t t)
{
    if(nextDay(t)) return include(t - ONE_DAY);

	time_t s = mktime(&start);
	time_t e = mktime(&end);

	//TRACE_LOG("%ld  %ld %ld" , t , s + TIME_OFFSET ,e - TIME_OFFSET);
	//TODO: move TIME_OFFSET to Exchange::initAll
    return t > (s + TIME_OFFSET) && t < (e - TIME_OFFSET);
}

bool TimeInterval::nextDay(time_t t)
{
	time_t s = mktime(&start);
	return t - s >= ONE_DAY;
}
