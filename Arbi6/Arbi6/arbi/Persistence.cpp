// Persistence.cpp: implementation of the Persistence class.
//
//////////////////////////////////////////////////////////////////////
//JAN(1F), FEB(2G), MAR(3H), APR(4J), MAY(5K), JUN(6M), JUL(7N), AUG(8Q), SEP(9U), OCT(10V), NOV(11X), DEC(12Z)

#include "Persistence.h"
#include "IntentionPair.h"
#include "ImportExpression.h"
#include "TwoLegStrategy.h"
#include "DayTradingStrategy.h"
#include "DealHistory.h"
#include "../LogStore.h"
#include "../utils/Utils.h"
#include <time.h>
#include <fstream> 
#include <assert.h>
#include <iostream> 
#include "KnownContract.h"
#include <process.h>
#include <time.h>

#include<Iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib")

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Persistence::Persistence()
{

}

Persistence::~Persistence()
{

}

void writeLastDiff()
{
	TRACE_LOG("writeLastDiff");
	ofstream diffFile("lastDiff.txt");
    if(diffFile.fail()) {
		TRACE_LOG("Write lastDiff.txt fail");
		return;
	}

	list<IntentionPair *> l = IntentionPair::listAll();
	list<IntentionPair *>::iterator the_iterator = l.begin();
    while( the_iterator != l.end() ) {
		diffFile << (*the_iterator)->getName() <<"  "<< (*the_iterator)->getFirstBid() <<"  "<< (*the_iterator)->getFirstAsk()
			<<"  "<< (*the_iterator)->getSecondBid() <<"  "<< (*the_iterator)->getSecondAsk()
			<< " " << (*the_iterator)->getlastPriceDiff() << endl;
        the_iterator++;
	}
}

void start3PMTask(void* pParams)
{
	time_t _now;
    time(&_now);
	struct tm now;
    int errorNo = localtime_s(&now, &_now);
	int now_seconds = now.tm_hour * 60 * 60 + now.tm_min * 60 + now.tm_sec;
	int task_seconds = 15 * 60 * 60;
	int sleep_seconds = task_seconds - now_seconds;
	TRACE_LOG("sleep_seconds is %d", sleep_seconds);
	if(sleep_seconds > 0)
	{
		TRACE_LOG("start3PMTask start now is %d : %d : %d", now.tm_hour, now.tm_min, now.tm_sec);
		Sleep(sleep_seconds * 1000);
		writeLastDiff();
	}else	
	{
		TRACE_LOG("start3PMTask sleep_seconds is below zero %d : %d : %d", now.tm_hour, now.tm_min, now.tm_sec);
		Sleep(5 * 1000);
		writeLastDiff();
	}
}

void Persistence::load()
{
	TRACE_LOG("Load start.");
	isfirstLoad = false;
	loadKnownContract();       
    loadIntentions();
    loadTwoLegStrategy();
	loadDayTrading();
	loadHistory();
	loadImportExpression();
	loadOrderCount();
	_beginthread(start3PMTask, 0, (void *)NULL);
	
	//MAC Adress
	//MacAdress();
    TRACE_LOG("Load finished.");
}

string get_mac ()
{
    string all_mac = "";
    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = NULL;
    DWORD dwRetVal = 0;
    ULONG ulOutBufLen;
    pAdapterInfo=(PIP_ADAPTER_INFO)malloc(sizeof(IP_ADAPTER_INFO));
    ulOutBufLen = sizeof(IP_ADAPTER_INFO);

    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
    {
        free(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO *) malloc (ulOutBufLen);
    }
    if ((dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
    {
        pAdapter = pAdapterInfo;
        while (pAdapter)
        {
            char mac[100];
            sprintf_s(mac, 100, "%02x-%02x-%02x-%02x-%02x-%02x;", pAdapter->Address[0], pAdapter->Address[1], pAdapter->Address[2], pAdapter->Address[3], pAdapter->Address[4], pAdapter->Address[5]);
            all_mac += mac;
            pAdapter = pAdapter->Next;
        }
    }
    return all_mac;
}


void Persistence::MacAdress()
{
	string macadress;
	macadress = get_mac();
	TRACE_LOG("MacAdress is %s", macadress.c_str());
/*
	if(macadress != "00-1d-09-cf-f9-f5;")
	{
		TRACE_LOG("MacAdress is not right!");
		exit(1);
	}
*/
}

void Persistence::loadKnownContract()
{
	/*
	string spName,fixName,ronhonName;

	ifstream file("KnownContract.txt");        // "KnownContract.txt" we have initially created
	if(file.fail()){
		TRACE_LOG("load KnownContract.txt fail!");
		return;
	}

	KnownContract::names.clear();

	while(!file.eof()){
		file>>spName>>fixName>>ronhonName;
		KnownContract::add(spName,fixName,ronhonName);
	}
	*/

	//patsapiName example "10YR SWAP-DEC13@CME_CBT"

	//BPTA
	KnownContract::add("510050", "BPTA-201303@BOHAI", "BOHAI NB BPTA MAR13", "BPTA-201303@BOHAI");
	KnownContract::add("510500", "BPTA-201303@BOHAI", "BOHAI NB BPTA MAR13", "BPTA-201303@BOHAI");
	KnownContract::add("510300", "BPTA-201303@BOHAI", "BOHAI NB BPTA MAR13", "BPTA-201303@BOHAI");

	KnownContract::add("BPTA", "BPTA-201303@BOHAI", "BOHAI NB BPTA MAR13", "BPTA-201303@BOHAI");
	KnownContract::add("BCK", "BCK-201303@BOHAI", "BOHAI NB BCK MAR13", "BCK-201303@BOHAI");
	KnownContract::add("BAG", "BAG-201303@BOHAI", "BOHAI NB BAG MAR13", "BAG-201303@BOHAI");
	KnownContract::add("BRURV", "BRURV-201303@BOHAI", "BOHAI NB BRURV MAR13", "BRURV-201303@BOHAI");
	KnownContract::add("BSC", "BSC-201303@BOHAI", "BOHAI NB BSC MAR13", "BSC-201303@BOHAI");
	KnownContract::add("BCUSH", "BCUSH-201303@BOHAI", "BOHAI NB BCUSH MAR13", "BCUSH-201303@BOHAI");
	KnownContract::add("BRBWX", "BRBWX-201303@BOHAI", "BOHAI NB BRBWX MAR13", "BRBWX-201303@BOHAI");
	//SB
	KnownContract::add("SBH4", "SB-201303@NYBOT", "NYBOT NB SU11 MAR14", "SUGAR11-MAR14@NYBOT");
	KnownContract::add("SBK4", "SB-201305@NYBOT", "NYBOT NB SU11 MAY14", "SUGAR11-MAY14@NYBOT");
	KnownContract::add("SBN4", "SB-201307@NYBOT", "NYBOT NB SU11 JUL14", "SUGAR11-JUL14@NYBOT");
	KnownContract::add("SBV4", "SB-201310@NYBOT", "NYBOT NB SU11 OCT14", "SUGAR11-OCT14@NYBOT");
	KnownContract::add("SBH5", "SB-201303@NYBOT", "NYBOT NB SU11 MAR14", "SUGAR11-MAR14@NYBOT");
	KnownContract::add("SBK5", "SB-201305@NYBOT", "NYBOT NB SU11 MAY14", "SUGAR11-MAY14@NYBOT");
	KnownContract::add("SBN5", "SB-201307@NYBOT", "NYBOT NB SU11 JUL14", "SUGAR11-JUL14@NYBOT");
	KnownContract::add("SBV5", "SB-201310@NYBOT", "NYBOT NB SU11 OCT14", "SUGAR11-OCT14@NYBOT");
	//LSU
	KnownContract::add("LSUK3", "LSU-201305@NYBOT", "NYBOT NB SU11 MAY13", "SUGAR-JUL13@Liffe");
	KnownContract::add("LSUN3", "LSU-201307@NYBOT", "NYBOT NB SU11 JUL13", "SUGAR-OCT13@Liffe");
	KnownContract::add("LSUQ4", "LSU-201407@NYBOT", "NYBOT NB SU11 JUL14", "SUGAR-OCT14@Liffe");
	KnownContract::add("LSUH5", "LSU-201407@NYBOT", "NYBOT NB SU11 JUL14", "SUGAR-OCT14@Liffe");
	KnownContract::add("LSUK5", "LSU-201305@NYBOT", "NYBOT NB SU11 MAY13", "SUGAR-JUL13@Liffe");
	KnownContract::add("LSUN5", "LSU-201307@NYBOT", "NYBOT NB SU11 JUL13", "SUGAR-OCT13@Liffe");
	KnownContract::add("LSUQ5", "LSU-201407@NYBOT", "NYBOT NB SU11 JUL14", "SUGAR-OCT14@Liffe");
	//CT
	KnownContract::add("CTZ2", "CT-201212@NYBOT", "NYBOT NB COTT DEC12", "COTTON-DEC12@NYBOT");
	KnownContract::add("CTF3", "CT-201301@NYBOT", "NYBOT NB COTT JAN13", "COTTON-JAN13@NYBOT");
	KnownContract::add("CTG3", "CT-201302@NYBOT", "NYBOT NB COTT FEB13", "COTTON-FEB13@NYBOT");
	KnownContract::add("CTH3", "CT-201303@NYBOT", "NYBOT NB COTT MAR13", "COTTON-MAR13@NYBOT");
	KnownContract::add("CTJ3", "CT-201304@NYBOT", "NYBOT NB COTT APR13", "COTTON-APR13@NYBOT");
	KnownContract::add("CTK3", "CT-201305@NYBOT", "NYBOT NB COTT MAY13", "COTTON-MAY13@NYBOT");
	KnownContract::add("CTN3", "CT-201307@NYBOT", "NYBOT NB COTT JUL13", "COTTON-JUL13@NYBOT");
	KnownContract::add("CTU3", "CT-201309@NYBOT", "NYBOT NB COTT SEP13", "COTTON-SEP13@NYBOT");
	KnownContract::add("CTX3", "CT-201311@NYBOT", "NYBOT NB COTT NOV13", "COTTON-NOV13@NYBOT");
	KnownContract::add("CTZ3", "CT-201312@NYBOT", "NYBOT NB COTT DEC13", "COTTON-DEC13@NYBOT");
	KnownContract::add("CTF4", "CT-201301@NYBOT", "NYBOT NB COTT JAN14", "COTTON-JAN14@NYBOT");
	KnownContract::add("CTG4", "CT-201302@NYBOT", "NYBOT NB COTT FEB14", "COTTON-FEB14@NYBOT");
	KnownContract::add("CTH4", "CT-201303@NYBOT", "NYBOT NB COTT MAR14", "COTTON-MAR14@NYBOT");
	KnownContract::add("CTJ4", "CT-201304@NYBOT", "NYBOT NB COTT APR14", "COTTON-APR14@NYBOT");
	KnownContract::add("CTK4", "CT-201305@NYBOT", "NYBOT NB COTT MAY14", "COTTON-MAY14@NYBOT");
	KnownContract::add("CTN4", "CT-201307@NYBOT", "NYBOT NB COTT JUL14", "COTTON-JUL14@NYBOT");
	KnownContract::add("CTU4", "CT-201309@NYBOT", "NYBOT NB COTT SEP14", "COTTON-SEP14@NYBOT");
	KnownContract::add("CTV4", "CT-201311@NYBOT", "NYBOT NB COTT NOV14", "COTTON-OCT14@NYBOT");
	KnownContract::add("CTV4", "CT-201311@NYBOT", "NYBOT NB COTT NOV14", "COTTON-OCT14@NYBOT");
	KnownContract::add("CTZ4", "CT-201311@NYBOT", "NYBOT NB COTT NOV14", "COTTON-DEC14@NYBOT");
	KnownContract::add("CTF5", "CT-201301@NYBOT", "NYBOT NB COTT JAN15", "COTTON-JAN15@NYBOT");
	KnownContract::add("CTG5", "CT-201302@NYBOT", "NYBOT NB COTT FEB15", "COTTON-FEB15@NYBOT");
	KnownContract::add("CTH5", "CT-201303@NYBOT", "NYBOT NB COTT MAR15", "COTTON-MAR15@NYBOT");
	KnownContract::add("CTJ5", "CT-201305@NYBOT", "NYBOT NB COTT APR15", "COTTON-APR15@NYBOT");
	KnownContract::add("CTK5", "CT-201305@NYBOT", "NYBOT NB COTT MAY15", "COTTON-MAY15@NYBOT");
	KnownContract::add("CTN5", "CT-201307@NYBOT", "NYBOT NB COTT JUL15", "COTTON-JUL15@NYBOT");
	KnownContract::add("CTU5", "CT-201309@NYBOT", "NYBOT NB COTT SEP15", "COTTON-SEP15@NYBOT");
	KnownContract::add("CTV5", "CT-201311@NYBOT", "NYBOT NB COTT NOV15", "COTTON-OCT15@NYBOT");
	KnownContract::add("CTV5", "CT-201311@NYBOT", "NYBOT NB COTT NOV15", "COTTON-OCT15@NYBOT");
	KnownContract::add("CTZ5", "CT-201311@NYBOT", "NYBOT NB COTT NOV15", "COTTON-DEC15@NYBOT");
	
	//ZM----------not open in patsapiName
	KnownContract::add("ZMZ2", "ZC-201212@CME_CBT", "CME_CBT SOYMEAL DEC12", "SOYMEAL-DEC12@CME_CBT");
	KnownContract::add("ZMF3", "ZC-201301@CME_CBT", "CME_CBT SOYMEAL JAN13", "SOYMEAL-JAN13@CME_CBT");
	KnownContract::add("ZMH3", "ZC-201303@CME_CBT", "CME_CBT SOYMEAL DEC13", "SOYMEAL-DEC13@CME_CBT");
	KnownContract::add("ZMK3", "ZC-201305@CME_CBT", "CME_CBT SOYMEAL MAY13", "SOYMEAL-MAY13@CME_CBT");
	KnownContract::add("ZMN3", "ZC-201307@CME_CBT", "CME_CBT SOYMEAL JUL13", "SOYMEAL-JUL13@CME_CBT");
	KnownContract::add("ZMU3", "ZC-201309@CME_CBT", "CME_CBT SOYMEAL SEP13", "SOYMEAL-SEP13@CME_CBT");
	KnownContract::add("ZMX3", "ZC-201311@CME_CBT", "CME_CBT SOYMEAL NOV13", "SOYMEAL-NOV13@CME_CBT");
	KnownContract::add("ZMZ3", "ZC-201212@CME_CBT", "CME_CBT SOYMEAL DEC12", "SOYMEAL-DEC12@CME_CBT");
	KnownContract::add("ZMF4", "ZC-201301@CME_CBT", "CME_CBT SOYMEAL JAN13", "SOYMEAL-JAN14@CME_CBT");
	KnownContract::add("ZMH4", "ZC-201303@CME_CBT", "CME_CBT SOYMEAL DEC13", "SOYMEAL-DEC14@CME_CBT");
	KnownContract::add("ZMK4", "ZC-201305@CME_CBT", "CME_CBT SOYMEAL MAY13", "SOYMEAL-MAY14@CME_CBT");
	KnownContract::add("ZMN4", "ZC-201307@CME_CBT", "CME_CBT SOYMEAL JUL13", "SOYMEAL-JUL14@CME_CBT");
	KnownContract::add("ZMU4", "ZC-201309@CME_CBT", "CME_CBT SOYMEAL SEP13", "SOYMEAL-SEP14@CME_CBT");
	KnownContract::add("ZMX4", "ZC-201311@CME_CBT", "CME_CBT SOYMEAL NOV13", "SOYMEAL-NOV14@CME_CBT");
	//ZC
	KnownContract::add("ZCZ2", "ZC-201212@CME_CBT", "CME_CBT CORN DEC12", "CORN-DEC12@CME_CBT");
	KnownContract::add("ZCF3", "ZC-201301@CME_CBT", "CME_CBT CORN JAN13", "CORN-JAN13@CME_CBT");
	KnownContract::add("ZCH3", "ZC-201303@CME_CBT", "CME_CBT CORN MAR13", "CORN-MAR13@CME_CBT");
	KnownContract::add("ZCK3", "ZC-201305@CME_CBT", "CME_CBT CORN DEC13", "CORN-DEC13@CME_CBT");
	KnownContract::add("ZCN3", "ZC-201307@CME_CBT", "CME_CBT CORN JUL13", "CORN-JUL13@CME_CBT");
	KnownContract::add("ZCU3", "ZC-201309@CME_CBT", "CME_CBT CORN SEP13", "CORN-SEP13@CME_CBT");
	KnownContract::add("ZCX3", "ZC-201311@CME_CBT", "CME_CBT CORN NOV13", "CORN-NOV13@CME_CBT");
	KnownContract::add("ZCZ3", "ZC-201212@CME_CBT", "CME_CBT CORN DEC12", "CORN-DEC12@CME_CBT");
	KnownContract::add("ZCF4", "ZC-201301@CME_CBT", "CME_CBT CORN JAN13", "CORN-JAN14@CME_CBT");
	KnownContract::add("ZCH4", "ZC-201303@CME_CBT", "CME_CBT CORN MAR13", "CORN-MAR14@CME_CBT");
	KnownContract::add("ZCK4", "ZC-201305@CME_CBT", "CME_CBT CORN DEC13", "CORN-DEC14@CME_CBT");
	KnownContract::add("ZCN4", "ZC-201307@CME_CBT", "CME_CBT CORN JUL13", "CORN-JUL14@CME_CBT");
	KnownContract::add("ZCU4", "ZC-201309@CME_CBT", "CME_CBT CORN SEP13", "CORN-SEP14@CME_CBT");
	KnownContract::add("ZCX4", "ZC-201311@CME_CBT", "CME_CBT CORN NOV13", "CORN-NOV14@CME_CBT");
	//ZS
	KnownContract::add("ZSF3", "ZS-201301@CME_CBT", "CME_CBT SOYBEAN JAN13", "SOYBEAN-JAN13@CME_CBT");
	KnownContract::add("ZSK3", "ZS-201305@CME_CBT", "CME_CBT SOYBEAN MAR13", "SOYBEAN-MAR13@CME_CBT");
	KnownContract::add("ZSH3", "ZS-201303@CME_CBT", "CME_CBT SOYBEAN MAY13", "SOYBEAN-MAY13@CME_CBT");
	KnownContract::add("ZSN3", "ZS-201307@CME_CBT", "CME_CBT SOYBEAN JUL13", "SOYBEAN-JUL13@CME_CBT");
	KnownContract::add("ZSU3", "ZS-201309@CME_CBT", "CME_CBT SOYBEAN SEP13", "SOYBEAN-SEP13@CME_CBT");
	KnownContract::add("ZSX3", "ZS-201311@CME_CBT", "CME_CBT SOYBEAN NOV13", "SOYBEAN-NOV13@CME_CBT");
	KnownContract::add("ZSF4", "ZS-201301@CME_CBT", "CME_CBT SOYBEAN JAN13", "SOYBEAN-JAN14@CME_CBT");
	KnownContract::add("ZSK4", "ZS-201305@CME_CBT", "CME_CBT SOYBEAN MAR13", "SOYBEAN-MAR14@CME_CBT");
	KnownContract::add("ZSH4", "ZS-201303@CME_CBT", "CME_CBT SOYBEAN MAY13", "SOYBEAN-MAY14@CME_CBT");
	KnownContract::add("ZSN4", "ZS-201307@CME_CBT", "CME_CBT SOYBEAN JUL13", "SOYBEAN-JUL14@CME_CBT");
	KnownContract::add("ZSU4", "ZS-201309@CME_CBT", "CME_CBT SOYBEAN SEP13", "SOYBEAN-SEP14@CME_CBT");
	KnownContract::add("ZSX4", "ZS-201311@CME_CBT", "CME_CBT SOYBEAN NOV13", "SOYBEAN-NOV14@CME_CBT");
	//PL
	KnownContract::add("PLZ2", "PL-201212@CME", "CME NYM PAL DEC12", "PLATINUM-DEC12@CME");
	KnownContract::add("PLF3", "PL-201301@CME", "CME NYM PAL JAN13", "PLATINUM-JAN13@CME");
	KnownContract::add("PLH3", "PL-201303@CME", "CME NYM PAL MAR13", "PLATINUM-MAR13@CME");
	KnownContract::add("PLJ3", "PL-201304@CME", "CME NYM PAL APR13", "PLATINUM-APR13@CME");
	KnownContract::add("PLK3", "PL-201305@CME", "CME NYM PAL MAY13", "PLATINUM-MAY13@CME");
	KnownContract::add("PLM3", "PL-201306@CME", "CME NYM PAL JUN13", "PLATINUM-JUN13@CME");
	KnownContract::add("PLN3", "PL-201307@CME", "CME NYM PAL JUL13", "PLATINUM-JUL13@CME");
	KnownContract::add("PLQ3", "PL-201308@CME", "CME NYM PAL AUG13", "PLATINUM-AUG13@CME");
	KnownContract::add("PLU3", "PL-201309@CME", "CME NYM PAL SEP13", "PLATINUM-SEP13@CME");
	KnownContract::add("PLV3", "PL-201310@CME", "CME NYM PAL OCT13", "PLATINUM-OCT13@CME");
	KnownContract::add("PLX3", "PL-201311@CME", "CME NYM PAL NOV13", "PLATINUM-NOV13@CME");
	KnownContract::add("PLZ3", "PL-201312@CME", "CME NYM PAL DEC13", "PLATINUM-DEC13@CME");
	KnownContract::add("PLF4", "PL-201301@CME", "CME NYM PAL JAN13", "PLATINUM-JAN14@CME");
	KnownContract::add("PLH4", "PL-201303@CME", "CME NYM PAL MAR13", "PLATINUM-MAR14@CME");
	KnownContract::add("PLJ4", "PL-201304@CME", "CME NYM PAL APR13", "PLATINUM-APR14@CME");
	KnownContract::add("PLK4", "PL-201305@CME", "CME NYM PAL MAY13", "PLATINUM-MAY14@CME");
	KnownContract::add("PLM4", "PL-201306@CME", "CME NYM PAL JUN13", "PLATINUM-JUN14@CME");
	KnownContract::add("PLN4", "PL-201307@CME", "CME NYM PAL JUL13", "PLATINUM-JUL14@CME");
	KnownContract::add("PLQ4", "PL-201308@CME", "CME NYM PAL AUG13", "PLATINUM-AUG14@CME");
	KnownContract::add("PLU4", "PL-201309@CME", "CME NYM PAL SEP13", "PLATINUM-SEP14@CME");
	KnownContract::add("PLV4", "PL-201310@CME", "CME NYM PAL OCT13", "PLATINUM-OCT14@CME");
	KnownContract::add("PLX4", "PL-201311@CME", "CME NYM PAL NOV13", "PLATINUM-NOV14@CME");
	KnownContract::add("PLZ4", "PL-201312@CME", "CME NYM PAL DEC13", "PLATINUM-DEC14@CME");
	KnownContract::add("PLF5", "PL-201301@CME", "CME NYM PAL JAN13", "PLATINUM-JAN15@CME");
	KnownContract::add("PLH5", "PL-201303@CME", "CME NYM PAL MAR13", "PLATINUM-MAR15@CME");
	KnownContract::add("PLJ5", "PL-201305@CME", "CME NYM PAL APR13", "PLATINUM-APR15@CME");
	KnownContract::add("PLK5", "PL-201305@CME", "CME NYM PAL MAY13", "PLATINUM-MAY15@CME");
	KnownContract::add("PLM5", "PL-201306@CME", "CME NYM PAL JUN13", "PLATINUM-JUN15@CME");
	KnownContract::add("PLN5", "PL-201307@CME", "CME NYM PAL JUL13", "PLATINUM-JUL15@CME");
	KnownContract::add("PLQ5", "PL-201308@CME", "CME NYM PAL AUG13", "PLATINUM-AUG15@CME");
	KnownContract::add("PLU5", "PL-201309@CME", "CME NYM PAL SEP13", "PLATINUM-SEP15@CME");
	KnownContract::add("PLV5", "PL-201310@CME", "CME NYM PAL OCT13", "PLATINUM-OCT15@CME");
	KnownContract::add("PLX5", "PL-201311@CME", "CME NYM PAL NOV13", "PLATINUM-NOV15@CME");
	KnownContract::add("PLZ5", "PL-201312@CME", "CME NYM PAL DEC13", "PLATINUM-DEC15@CME");
	//GC                                                            
	KnownContract::add("GCZ2", "GC-201212@CME", "CME CMX GLD DEC12", "CMX GLD-DEC12@CME");
	KnownContract::add("GCG3", "GC-201302@CME", "CME CMX GLD FEB13", "CMX GLD-FEB13@CME");
	KnownContract::add("GCF3", "GC-201301@CME", "CME CMX GLD JAN13", "CMX GLD-JAN13@CME");
	KnownContract::add("GCH3", "GC-201303@CME", "CME CMX GLD MAR13", "CMX GLD-MAR13@CME");
	KnownContract::add("GCJ3", "GC-201304@CME", "CME CMX GLD APR13", "CMX GLD-APR13@CME");
	KnownContract::add("GCK3", "GC-201305@CME", "CME CMX GLD MAY13", "CMX GLD-MAY13@CME");
	KnownContract::add("GCM3", "GC-201306@CME", "CME CMX GLD JUN13", "CMX GLD-JUN13@CME");
	KnownContract::add("GCN3", "GC-201307@CME", "CME CMX GLD JUL13", "CMX GLD-JUL13@CME");
	KnownContract::add("GCQ3", "GC-201308@CME", "CME CMX GLD AUG13", "CMX GLD-AUG13@CME");
	KnownContract::add("GCU3", "GC-201309@CME", "CME CMX GLD SEP13", "CMX GLD-SEP13@CME");
	KnownContract::add("GCV3", "GC-201310@CME", "CME CMX GLD OCT13", "CMX GLD-OCT13@CME");
	KnownContract::add("GCX3", "GC-201311@CME", "CME CMX GLD NOV13", "CMX GLD-NOV13@CME");
	KnownContract::add("GCZ3", "GC-201312@CME", "CME CMX GLD DEC13", "CMX GLD-DEC13@CME");
	KnownContract::add("GCG4", "GC-201302@CME", "CME CMX GLD FEB13", "CMX GLD-FEB14@CME");
	KnownContract::add("GCF4", "GC-201301@CME", "CME CMX GLD JAN13", "CMX GLD-JAN14@CME");
	KnownContract::add("GCH4", "GC-201303@CME", "CME CMX GLD MAR13", "CMX GLD-MAR14@CME");
	KnownContract::add("GCJ4", "GC-201304@CME", "CME CMX GLD APR13", "CMX GLD-APR14@CME");
	KnownContract::add("GCK4", "GC-201305@CME", "CME CMX GLD MAY13", "CMX GLD-MAY14@CME");
	KnownContract::add("GCM4", "GC-201306@CME", "CME CMX GLD JUN13", "CMX GLD-JUN14@CME");
	KnownContract::add("GCN4", "GC-201307@CME", "CME CMX GLD JUL13", "CMX GLD-JUL14@CME");
	KnownContract::add("GCQ4", "GC-201308@CME", "CME CMX GLD AUG13", "CMX GLD-AUG14@CME");
	KnownContract::add("GCU4", "GC-201309@CME", "CME CMX GLD SEP13", "CMX GLD-SEP14@CME");
	KnownContract::add("GCV4", "GC-201310@CME", "CME CMX GLD OCT13", "CMX GLD-OCT14@CME");
	KnownContract::add("GCX4", "GC-201311@CME", "CME CMX GLD NOV13", "CMX GLD-NOV14@CME");
	KnownContract::add("GCZ4", "GC-201312@CME", "CME CMX GLD DEC13", "CMX GLD-DEC14@CME");
	KnownContract::add("GCG5", "GC-201302@CME", "CME CMX GLD FEB13", "CMX GLD-FEB15@CME");
	KnownContract::add("GCF5", "GC-201301@CME", "CME CMX GLD JAN13", "CMX GLD-JAN15@CME");
	KnownContract::add("GCH5", "GC-201303@CME", "CME CMX GLD MAR13", "CMX GLD-MAR15@CME");
	KnownContract::add("GCJ5", "GC-201305@CME", "CME CMX GLD APR13", "CMX GLD-APR15@CME");
	KnownContract::add("GCK5", "GC-201305@CME", "CME CMX GLD MAY13", "CMX GLD-MAY15@CME");
	KnownContract::add("GCM5", "GC-201306@CME", "CME CMX GLD JUN13", "CMX GLD-JUN15@CME");
	KnownContract::add("GCN5", "GC-201307@CME", "CME CMX GLD JUL13", "CMX GLD-JUL15@CME");
	KnownContract::add("GCQ5", "GC-201308@CME", "CME CMX GLD AUG13", "CMX GLD-AUG15@CME");
	KnownContract::add("GCU5", "GC-201309@CME", "CME CMX GLD SEP13", "CMX GLD-SEP15@CME");
	KnownContract::add("GCV5", "GC-201310@CME", "CME CMX GLD OCT13", "CMX GLD-OCT15@CME");
	KnownContract::add("GCX5", "GC-201311@CME", "CME CMX GLD NOV13", "CMX GLD-NOV15@CME");
	KnownContract::add("GCZ5", "GC-201312@CME", "CME CMX GLD DEC13", "CMX GLD-DEC15@CME");
	//ZR
	KnownContract::add("ZRF3", "ZR-201301@CME_CBT", "CME_CBT-RICE-JAN13", "RICE-JAN13@CME_CBT");
	KnownContract::add("ZRH3", "ZR-201303@CME_CBT", "CME_CBT-RICE-MAR13", "RICE-MAR13@CME_CBT");
	KnownContract::add("ZRK3", "ZR-201305@CME_CBT", "CME_CBT-RICE-MAY13", "RICE-MAY13@CME_CBT");
	//SN
	KnownContract::add("SN3M", "SN-3M@LME", "LME SN 3M", "SN-3M@LME");
	//HG
	KnownContract::add("HGZ3", "HG-Z3@NYMEX", "CME CMX COP DEC13", "CMX COP-DEC13@CME");
	KnownContract::add("HGF3", "HG-201301@NYMEX", "CME CMX COP JAN13", "CMX COP-JAN13@CME");
	KnownContract::add("HGN4", "HG-N4@NYMEX", "CME CMX COP JUL14", "CMX COP-JUL14@CME");
	KnownContract::add("HGH3", "HG-201303@NYMEX", "CME CMX COP MAR13", "CMX COP-MAR13@CME");
	KnownContract::add("HGK3", "HG-201305@NYMEX", "CME CMX COP MAY13", "CMX COP-MAY13@CME");
	KnownContract::add("HGU3", "HG-U2@NYMEX", "CME CMX COP MAR13", "CMX COP-SEP13@CME");
	KnownContract::add("HGZ4", "HG-Z3@NYMEX", "CME CMX COP DEC13", "CMX COP-DEC14@CME");
	KnownContract::add("HGF4", "HG-201401@NYMEX", "CME CMX COP JAN14", "CMX COP-JAN14@CME");
	KnownContract::add("HGG4", "HG-201402@NYMEX", "CME CMX COP FEB14", "CMX COP-FEB14@CME");
	KnownContract::add("HGH4", "HG-201303@NYMEX", "CME CMX COP MAR14", "CMX COP-MAR14@CME");
	KnownContract::add("HGK4", "HG-201305@NYMEX", "CME CMX COP MAY14", "CMX COP-MAY14@CME");
	KnownContract::add("HGU4", "HG-U2@NYMEX", "CME CMX COP MAR14", "CMX COP-SEP14@CME");
	KnownContract::add("HGZ5", "HG-Z3@NYMEX", "CME CMX COP DEC13", "CMX COP-DEC14@CME");
	KnownContract::add("HGF5", "HG-201401@NYMEX", "CME CMX COP JAN14", "CMX COP-JAN14@CME");
	KnownContract::add("HGG5", "HG-201402@NYMEX", "CME CMX COP FEB14", "CMX COP-FEB14@CME");
	KnownContract::add("HGH5", "HG-201303@NYMEX", "CME CMX COP MAR14", "CMX COP-MAR14@CME");
	KnownContract::add("HGK5", "HG-201305@NYMEX", "CME CMX COP MAY14", "CMX COP-MAY14@CME");
	KnownContract::add("HGU5", "HG-U2@NYMEX", "CME CMX COP MAR14", "CMX COP-SEP14@CME");

	//LME series
	KnownContract::add("LCA3M", "CA-3M@LME", "LME CA 3M" ,"CA-3M@LME");
	KnownContract::add("LAH3M", "AH-3M@LME", "LME AH 3M", "AH-3M@LME");
	KnownContract::add("LZS3M", "ZS-3M@LME", "LME ZS 3M", "ZS-3M@LME");
	KnownContract::add("LNI3M", "NI-3M@LME", "LME NI 3M", "ZS-3M@LME");
	//CL
	KnownContract::add("CLX2", "CL-201211@LME", "CME CRUDE NOV10","CRUDE CL-NOV10@CME");
	KnownContract::add("CLV2", "CL-201210@LME", "CME CRUDE OCT10","CRUDE CL-OCT10@CME");
	KnownContract::add("CLF3", "CL-201301@LME", "CME CRUDE JAN13","CRUDE CL-JAN13@CME");
	KnownContract::add("CLG3", "CL-201302@LME", "CME CRUDE FEB13","CRUDE CL-FEB13@CME");
	KnownContract::add("CLH3", "CL-201303@LME", "CME CRUDE MAR13","CRUDE CL-MAR13@CME");
	KnownContract::add("CLJ3", "CL-201304@LME", "CME CRUDE APR13","CRUDE CL-APR13@CME");
	KnownContract::add("CLK3", "CL-201305@LME", "CME CRUDE MAY13","CRUDE CL-MAY13@CME");
	KnownContract::add("CLF5", "CL-201212@LME", "CME CRUDE DEC12","CRUDE CL-DEC12@CME");
	KnownContract::add("CLG5", "CL-201212@LME", "CME CRUDE DEC12","CRUDE CL-DEC12@CME");
	KnownContract::add("CLH5", "CL-201212@LME", "CME CRUDE DEC12","CRUDE CL-DEC12@CME");
	KnownContract::add("CLJ5", "CL-201212@LME", "CME CRUDE DEC12","CRUDE CL-DEC12@CME");
	KnownContract::add("CLK5", "CL-201212@LME", "CME CRUDE DEC12","CRUDE CL-DEC12@CME");
	KnownContract::add("CLM5", "CL-201212@LME", "CME CRUDE DEC12","CRUDE CL-DEC12@CME");
	KnownContract::add("CLN5", "CL-201212@LME", "CME CRUDE DEC12","CRUDE CL-DEC12@CME");
	KnownContract::add("CLQ5", "CL-201212@LME", "CME CRUDE DEC12","CRUDE CL-DEC12@CME");
	KnownContract::add("CLU5", "CL-201212@LME", "CME CRUDE DEC12","CRUDE CL-DEC12@CME");
	KnownContract::add("CLV5", "CL-201212@LME", "CME CRUDE DEC12","CRUDE CL-DEC12@CME");
	KnownContract::add("CLX5", "CL-201212@LME", "CME CRUDE DEC12","CRUDE CL-DEC12@CME");
	
    //6A 6C
	KnownContract::add("6AU4", "xx-xxxxxx@xxx", "xxx-xx-xxxxx", "AUD-SEP14@CME");
	KnownContract::add("6CU4", "xx-xxxxxx@xxx", "xxx-xx-xxxxx", "CAN-SEP14@CME");

	//JRU-------------------not open in patsapiName
	KnownContract::add("JRUF3", "JRU-201301@CME_CBT", "CME_CBT-JRU-JAN13", "JRU-JAN13@TOCOM");
	KnownContract::add("JRUH3", "JRU-201303@CME_CBT", "CME_CBT-JRU-JAN13", "JRU-MAR13@TOCOM");
	KnownContract::add("JRUK3", "JRU-201305@CME_CBT", "CME_CBT-JRU-MAY13", "JRU-MAY13@TOCOM");
	KnownContract::add("JRUJ3", "JRU-201304@CME_CBT", "CME_CBT-JRU-APR13", "JRU-APR13@TOCOM");
	KnownContract::add("JRUN3", "JRU-201307@CME_CBT", "CME_CBT-JRU-JUL13", "JRU-JUL13@TOCOM");
	KnownContract::add("JRUQ3", "JRU-201308@CME_CBT", "CME_CBT-JRU-AUG13", "JRU-AUG13@TOCOM");
	KnownContract::add("JRUF4", "JRU-201301@CME_CBT", "CME_CBT-JRU-JAN13", "JRU-JAN14@TOCOM");
	KnownContract::add("JRUH4", "JRU-201303@CME_CBT", "CME_CBT-JRU-JAN13", "JRU-MAR14@TOCOM");
	KnownContract::add("JRUK4", "JRU-201305@CME_CBT", "CME_CBT-JRU-MAY13", "JRU-MAY14@TOCOM");
	KnownContract::add("JRUJ4", "JRU-201304@CME_CBT", "CME_CBT-JRU-APR13", "JRU-APR14@TOCOM");
	KnownContract::add("JRUN4", "JRU-201307@CME_CBT", "CME_CBT-JRU-JUL13", "JRU-JUL14@TOCOM");
	KnownContract::add("JRUQ4", "JRU-201308@CME_CBT", "CME_CBT-JRU-AUG13", "JRU-AUG14@TOCOM");
	KnownContract::add("JRUU4", "JRU-201409@CME_CBT", "CME_CBT-JRU-SEP14", "JRU-SEP14@TOCOM");
	KnownContract::add("JRUV4", "JRU-201410@CME_CBT", "CME_CBT-JRU-OCT14", "JRU-OCT14@TOCOM");
	KnownContract::add("JRUX4", "JRU-201411@CME_CBT", "CME_CBT-JRU-NOV14", "JRU-NOV14@TOCOM");
	KnownContract::add("JRUZ4", "JRU-201412@CME_CBT", "CME_CBT-JRU-NOV14", "JRU-DEC14@TOCOM");
	KnownContract::add("JRUF5", "JRU-201301@CME_CBT", "CME_CBT-JRU-JAN13", "JRU-JAN15@TOCOM");
	KnownContract::add("JRUG5", "JRU-201303@CME_CBT", "CME_CBT-JRU-JAN13", "JRU-FEB15@TOCOM");
	KnownContract::add("JRUH5", "JRU-201305@CME_CBT", "CME_CBT-JRU-MAY13", "JRU-MAR15@TOCOM");
	KnownContract::add("JRUJ5", "JRU-201305@CME_CBT", "CME_CBT-JRU-APR13", "JRU-APR15@TOCOM");
	KnownContract::add("JRUK5", "JRU-201305@CME_CBT", "CME_CBT-JRU-APR13", "JRU-MAY15@TOCOM");
	KnownContract::add("JRUM5", "JRU-201307@CME_CBT", "CME_CBT-JRU-JUL13", "JRU-JUN15@TOCOM");
	KnownContract::add("JRUN5", "JRU-201307@CME_CBT", "CME_CBT-JRU-JUL13", "JRU-JUL15@TOCOM");
	KnownContract::add("JRUQ5", "JRU-201308@CME_CBT", "CME_CBT-JRU-AUG13", "JRU-AUG15@TOCOM");
	KnownContract::add("JRUU5", "JRU-201509@CME_CBT", "CME_CBT-JRU-SEP15", "JRU-SEP15@TOCOM");
	KnownContract::add("JRUV5", "JRU-201510@CME_CBT", "CME_CBT-JRU-OCT15", "JRU-OCT15@TOCOM");
	KnownContract::add("JRUX5", "JRU-201511@CME_CBT", "CME_CBT-JRU-NOV15", "JRU-NOV15@TOCOM");
	KnownContract::add("JRUZ5", "JRU-201512@CME_CBT", "CME_CBT-JRU-NOV15", "JRU-DEC15@TOCOM");
	//GJRU-------------------not open in patsapiName
	KnownContract::add("GJRUQ4", "GJRU-201308@CME_CBT", "CME_CBT-GJRU-AUG13", "RUBBER-AUG14@TOCOM_M");
	KnownContract::add("GJRUU4", "GJRU-201409@CME_CBT", "CME_CBT-GJRU-SEP14", "RUBBER-SEP14@TOCOM_M");
	KnownContract::add("GJRUV4", "GJRU-201410@CME_CBT", "CME_CBT-GJRU-OCT14", "RUBBER-OCT14@TOCOM_M");
	KnownContract::add("GJRUX4", "GJRU-201411@CME_CBT", "CME_CBT-GJRU-NOV14", "RUBBER-NOV14@TOCOM_M");
	KnownContract::add("GJRUZ4", "GJRU-201411@CME_CBT", "CME_CBT-GJRU-DEC14", "RUBBER-DEC14@TOCOM_M");
	KnownContract::add("GJRUF5", "GJRU-201411@CME_CBT", "CME_CBT-GJRU-DEC14", "RUBBER-JAN15@TOCOM_M");
	KnownContract::add("GJRUH5", "GJRU-201303@CME_CBT", "CME_CBT-GJRU-JAN13", "RUBBER-MAR15@TOCOM_M");
	//SI
	KnownContract::add("SIZ3", "SI-201213@CME", "CME CMX SIL DEC13", "CMX SIL-DEC13@CME");
	KnownContract::add("SIF3", "SI-201301@CME", "CME CMX SIL JAN13", "CMX SIL-JAN13@CME");
	KnownContract::add("SIG3", "SI-201302@CME", "CME CMX SIL FEB13", "CMX SIL-FEB13@CME");
	KnownContract::add("SIH3", "SI-201303@CME", "CME CMX SIL MAR13", "CMX SIL-MAR13@CME");
	KnownContract::add("SIJ3", "SI-201304@CME", "CME CMX SIL APR13", "CMX SIL-APR13@CME");
	KnownContract::add("SIK3", "SI-201305@CME", "CME CMX SIL MAY13", "CMX SIL-MAY13@CME");
	KnownContract::add("SIU3", "SI-201309@CME", "CME CMX SIL SEP13", "CMX SIL-SEP13@CME");
	KnownContract::add("SIZ4", "SI-201213@CME", "CME CMX SIL DEC13", "CMX SIL-DEC14@CME");
	KnownContract::add("SIF4", "SI-201301@CME", "CME CMX SIL JAN13", "CMX SIL-JAN14@CME");
	KnownContract::add("SIG4", "SI-201302@CME", "CME CMX SIL FEB13", "CMX SIL-FEB14@CME");
	KnownContract::add("SIH4", "SI-201303@CME", "CME CMX SIL MAR13", "CMX SIL-MAR14@CME");
	KnownContract::add("SIJ4", "SI-201304@CME", "CME CMX SIL APR13", "CMX SIL-APR14@CME");
	KnownContract::add("SIK4", "SI-201305@CME", "CME CMX SIL MAY13", "CMX SIL-MAY14@CME");
	KnownContract::add("SIU4", "SI-201309@CME", "CME CMX SIL SEP13", "CMX SIL-SEP14@CME");
	KnownContract::add("SIN4", "SI-201307@CME", "CME CMX SIL SEP13", "CMX SIL-SEP14@CME");
	//KPO------------------not open in patsapiName
	KnownContract::add("KPOZ4", "KPO-201212@CME", "CME_CBT-KPO-DEC12", "KPO-DEC14@BMD");
	KnownContract::add("KPOF3", "KPO-201301@CME", "CME_CBT-KPO-JAN13", "KPO-DEC13@BMD");
	KnownContract::add("KPOH3", "KPO-201303@CME", "CME_CBT-KPO-MAR13", "KPO-DEC13@BMD");
	KnownContract::add("KPOK3", "KPO-201305@CME", "CME_CBT-KPO-MAR13", "KPO-DEC13@BMD");
	KnownContract::add("KPOU3", "KPO-201309@CME", "CME_CBT-KPO-MAR13", "KPO-DEC13@BMD");
	KnownContract::add("KPOX3", "KPO-201311@CME", "CME_CBT-KPO-MAR13", "KPO-DEC13@BMD");
	KnownContract::add("KPOF4", "KPO-201301@CME", "CME_CBT-KPO-JAN13", "KPO-DEC14@BMD");
	KnownContract::add("KPOH4", "KPO-201303@CME", "CME_CBT-KPO-MAR13", "KPO-DEC14@BMD");
	KnownContract::add("KPOK4", "KPO-201305@CME", "CME_CBT-KPO-MAR13", "KPO-DEC14@BMD");
	KnownContract::add("KPOU4", "KPO-201309@CME", "CME_CBT-KPO-MAR13", "KPO-DEC14@BMD");
	KnownContract::add("KPOX4", "KPO-201311@CME", "CME_CBT-KPO-MAR13", "KPO-DEC14@BMD");
	KnownContract::add("KPOF5", "KPO-201301@CME", "CME_CBT-KPO-JAN13", "KPO-DEC15@BMD");
	KnownContract::add("KPOH5", "KPO-201303@CME", "CME_CBT-KPO-MAR13", "KPO-DEC15@BMD");
	KnownContract::add("KPOK5", "KPO-201305@CME", "CME_CBT-KPO-MAR13", "KPO-DEC15@BMD");
	KnownContract::add("KPOU5", "KPO-201309@CME", "CME_CBT-KPO-MAR13", "KPO-DEC15@BMD");
	KnownContract::add("KPOX5", "KPO-201311@CME", "CME_CBT-KPO-MAR13", "KPO-DEC15@BMD");
	//GKPO------------------not open in patsapiName
	KnownContract::add("GKPOQ4", "GKPO-201412@CME", "CME_CBT-GKPO-DEC12", "CPO-AUG14@BMD_OK");
	KnownContract::add("GKPOV4", "GKPO-201412@CME", "CME_CBT-GKPO-DEC12", "CPO-OCT14@BMD_OK");
	KnownContract::add("GKPOX4", "GKPO-201412@CME", "CME_CBT-GKPO-DEC12", "CPO-NOV14@BMD_OK");
	KnownContract::add("GKPOZ4", "GKPO-201412@CME", "CME_CBT-GKPO-DEC12", "CPO-DEC14@BMD_OK");
	KnownContract::add("GKPOF5", "GKPO-201412@CME", "CME_CBT-GKPO-DEC12", "CPO-JAN15@BMD_OK");
	KnownContract::add("GKPOH5", "GKPO-201412@CME", "CME_CBT-GKPO-DEC12", "CPO-MAR15@BMD_OK");
	KnownContract::add("GKPOK5", "GKPO-201412@CME", "CME_CBT-GKPO-DEC12", "CPO-MAY15@BMD_OK");
	//GFPO------------------not open in patsapiName
	KnownContract::add("GFPOQ4", "GFPO-201412@CME", "CME_CBT-GFPO-DEC12", "FPOL-AUG14@BMD_OK");
	KnownContract::add("GFPOV4", "GFPO-201412@CME", "CME_CBT-GFPO-DEC12", "FPOL-OCT14@BMD_OK");
	KnownContract::add("GFPOX4", "GFPO-201412@CME", "CME_CBT-GFPO-DEC12", "FPOL-NOV14@BMD_OK");
	KnownContract::add("GFPOZ4", "GFPO-201412@CME", "CME_CBT-GFPO-DEC12", "FPOL-DEC14@BMD_OK");
	KnownContract::add("GFPOF5", "GFPO-201412@CME", "CME_CBT-GFPO-DEC12", "FPOL-JAN15@BMD_OK");
	KnownContract::add("GFPOH5", "GFPO-201412@CME", "CME_CBT-GFPO-DEC12", "FPOL-MAR15@BMD_OK");
	KnownContract::add("GFPOK5", "GFPO-201412@CME", "CME_CBT-GFPO-DEC12", "FPOL-MAY15@BMD_OK");

}
void Persistence::saveIntentions()
{
	ofstream file("intentions.txt");
    if(file.fail()) {
		TRACE_LOG("Write intentions.txt fail");
		return;
	}


    list<IntentionPair *> l = IntentionPair::listAll();
    list<IntentionPair *>::iterator the_iterator = l.begin();

    while( the_iterator != l.end() ) {
        file << (*the_iterator)->toString() << endl;
        the_iterator++;
	}
}

void Persistence::saveTwoLeg()
{
	ofstream file("twoleg.txt");
    if(file.fail()) {
		TRACE_LOG("Write twoleg.txt fail");
		return;
	}


    list<TwoLegStrategy *> l = TwoLegStrategy::listAll();
    list<TwoLegStrategy *>::iterator the_iterator = l.begin();

    while( the_iterator != l.end() ) {
        file << (*the_iterator)->toString() << endl;
		TRACE_LOG("Save %s", (*the_iterator)->toString().c_str());
        the_iterator++;
	}

	file.close();
}

void Persistence::loadIntentions()
{
    TRACE_LOG("Load IntentionPair.");
	IntentionPair::clear();
	ifstream file("intentions.txt");
    if(file.fail()) {
		TRACE_LOG("Read intentions.txt fail");
		return;
	}

	string name, legA, legB, diffExp;
	int lotA, lotB;

	while(!file.eof()) {
		file >> name >> legA >> legB >> diffExp >> lotA >> lotB;

		if(name.length() >= 1)
		{
			IntentionPair * ip = IntentionPair::add(name, legA, legB, diffExp, lotA, lotB);
			name = "";
			assert(ip != NULL);
			TRACE_LOG("Read IntentionPair %s", ip->toString().c_str());
		}
	}	

}

Leg * assembleLeg(string account, string buySell, string kaiPing, string contract, int lot, int offset)
{
    enum BuySell bs = (buySell.compare("BUY") == 0) ? BUY : SELL;
	enum KaiPing kp = KAI_CANG;
	if(kaiPing.compare("PC") == 0) kp = PING_CANG;
    if(kaiPing.compare("PJ") == 0) kp = PING_JIN;
    Leg * l = new Leg(account, bs, kp, contract, lot, offset);
	return  l;
}

void Persistence::loadTwoLegStrategy()
{
    TRACE_LOG("Load TwoLegStrategy.");
	TwoLegStrategy::clear();

	ifstream file("twoleg.txt");
    if(file.fail()) {
		TRACE_LOG("Read twoleg.txt fail");
		return;
	}

	//JRU-ru-2 2639 SELL KC JRUU1 1 0 10110882 BUY KC ru1109 1 0 ru1109.ask-JRUU1.bid >= 1000.00 0 5
	string id, account, buySell, kaiPing, contract, exp, comparator;
	int lot, offset, lowLimit, highLimit, holdingOperation;
	double diff;

	TwoLegStrategy * s;
	while(!file.eof()) {
		file >> id;
		if(id.length() > 1)
		{
			s = TwoLegStrategy::create(id);
			file >> account >> buySell >> kaiPing >> contract >> lot >> offset;
            s->setLegA(assembleLeg(account, buySell, kaiPing, contract, lot, offset));
            file >> account >> buySell >> kaiPing >> contract >> lot >> offset;
            s->setLegB(assembleLeg(account, buySell, kaiPing, contract, lot, offset));
			file >> exp >> comparator >> diff >> lowLimit >> highLimit >> holdingOperation;
            s->setCondition(new Condition(exp, comparator, diff));
			s->setHoldingLimit(lowLimit, highLimit);
			if(holdingOperation == 1) s->setHoldingOperation(DECREASE);
			TRACE_LOG("Read strategy: %s", s->toString().c_str());
			id = "";
		}
	}	
}
bool Persistence::isfirstLoad = true;

void Persistence::loadAsNecessary()
{
	if (isfirstLoad) {
		load();
	}
}

void Persistence::saveDayTrading()
{
	TRACE_LOG("saveDayTrading");
	ofstream file("DayTrading.txt");
    if(file.fail()) {
		TRACE_LOG("Write DayTrading.txt fail");
		return;
	}

    list<DayTradingStrategy *> l = DayTradingStrategy::listAll();
    list<DayTradingStrategy *>::iterator the_iterator = l.begin();

    while( the_iterator != l.end() ) {
        file << (*the_iterator)->toString() << endl;
        the_iterator++;
	}
	file.close();
}

string todayFile()
{
	time_t t = time(NULL);
	struct tm tmp; 
	int errorNo = localtime_s(&tmp, &t);
    return Utils::intToString(tmp.tm_year + 1900) + Utils::intToString(tmp.tm_mon + 1) + Utils::intToString(tmp.tm_mday) + ".his";
}

void Persistence::loadHistory()
{
    TRACE_LOG("Load deal history.");
    ifstream fin(todayFile().c_str()); 
    const int LINE_LENGTH = 500; 
    char str[LINE_LENGTH];  
    while( fin.getline(str,LINE_LENGTH) )
    {    
		DealHistory::getInstance()->insertOld(str);
    }
	fin.close();
}

void Persistence::loadDayTrading()
{
    TRACE_LOG("Load dayTradingStraty.");
	DayTradingStrategy::clear();

	ifstream file("DayTrading.txt");
    if(file.fail()) {
		TRACE_LOG("Read DayTrading.txt fail");
		return;
	}

	string id, idA, idB;
	DayTradingStrategy * s;


	while(!file.eof()) {
		file >> id >> idA >> idB;
		//printf("loadDayTrading %s %s %s\n", id.c_str(), idA.c_str(), idB.c_str());
		if(id.length() > 1)
		{
			s = DayTradingStrategy::create(id);
			assert(s != NULL);
            s->setStrategyA(idA);
			//TRACE_LOG("Persistence0 dts:%p,B:%p,idB:%s",s,s->getStrategyB(),idB.c_str());
			s->setStrategyB(idB);
			TwoLegStrategy * tlsA = TwoLegStrategy::getTwoLegStrategy(idA);
			s->setHoldingLimit(tlsA->getLowHoldingLimit(), tlsA->getHighHoldingLimit());
			TRACE_LOG("Read DayTradingStrategy: %s", s->toString().c_str());
			id = "";
		}
	}	
}

void Persistence::loadImportExpression()
{
	TRACE_LOG("Load ImportExpression.");
	ifstream file("ImportExpression.txt");
    if(file.fail()) {
		TRACE_LOG("Read ImportExpression.txt fail");
		return;
	}
	string commodity;
	double refProfit;
	double a;
	double b;
	double c;
	double d;
	double e;
	double f;
	double g;
	double h;

/*
	while(1) {
		file >> commodity >> refProfit >> a >> b >> c >> d >> e >> f >> g >> h;
		if(commodity.length() > 1)
		{
			ImportExpression::add(commodity, refProfit, a, b, c, d, e, f, g, h);
			TRACE_LOG("Read %s ImportExpression", commodity.c_str());
			commodity = "";
		}
		if(file.peek() == EOF) break;
	}
*/
	while(file >> commodity) {
		file >> refProfit >> a >> b >> c >> d >> e >> f >> g >> h;
		ImportExpression::add(commodity, refProfit, a, b, c, d, e, f, g, h);
		TRACE_LOG("Read %s ImportExpression", commodity.c_str());
	}
}

void Persistence::loadOrderCount()
{
	Contract::loadOrderCount();
}
/*
void Persistence::saveKnownContract()
{
	ofstream file("KnownContract.txt");
    if(file.fail()) {
		TRACE_LOG("Write KnownContract.txt fail");
		return;
	}


    list<ContractNames *> cn = ContractNames::listAll();
    list<ContractNames *>::iterator the_iterator = cn.begin();

    while(the_iterator != cn.end() ) {
        file << (*the_iterator)-> << endl;
        the_iterator++;
	}
	file.close();
}


*/
/*
void Persistence::loadKnownContract()
{
	TRACE_LOG("Load KnownContract.");
	KnownContract::clear();
	
	ifstream file("KnownContract.txt");
	if(file.fail()) {
		TRACE_LOG("Read KnownContract.txt fail");
		return;
	}

	string spName,fixName,rohonName;

	while(!file.eof()){
		file>>spName;

		if(spName.length()>1){
			KnownContract *cc = ContractNames(spName,getFixName(spName),getRonhonNameFor(spName));
			assert(cc!=NULL);
			TRACE_LOG("Read KnownContract : %s",spName);
		}
	}
	
}

*/
