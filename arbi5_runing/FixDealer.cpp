// FixDealer.cpp: implementation of the FixDealer class.
//
//////////////////////////////////////////////////////////////////////
#pragma   warning   (disable:   4786) 

#include "FixDealer.h"
#include "LogStore.h"
#include <process.h>

extern string g_account;

FixDealer::FixDealer()
{

}

FixDealer::~FixDealer()
{

}

string FixDealer::FixNewOrderSingle(TNewOrderSingleMsg *tNOSM, long msgSeqNum)
{
	string ret;
	char szTemp[32];
	memset(szTemp, 0, sizeof(szTemp));
	LISTFIXMSG listFix;

	setTagValue(listFix, "11", tNOSM->CIOrdID);

	sprintf(szTemp, "%s", g_account.c_str());
	setTagValue(listFix, "1",  szTemp);

	setTagValue(listFix, "21", "1");

	setTagValue(listFix, "55",  tNOSM->Symbol);

	setTagValue(listFix, "200", tNOSM->MaturityMonthYear);

	setTagValue(listFix, "167", tNOSM->SecurityType);

	setTagValue(listFix, "207", tNOSM->SecurityExchange);

	sprintf(szTemp, "%d", tNOSM->Side);
	setTagValue(listFix, "54", szTemp);

	sprintf(szTemp, "%s", FixEncode::generateCurtime().c_str());
	setTagValue(listFix, "60", szTemp);

	sprintf(szTemp, "%d", tNOSM->OrderQty);
	setTagValue(listFix, "38", szTemp);

	sprintf(szTemp, "%d", tNOSM->OrdType);
	setTagValue(listFix, "40", szTemp);

	sprintf(szTemp, "%.2f", tNOSM->Price);
	setTagValue(listFix, "44", szTemp);

	setTagValue(listFix, "59", "0");

	if (strlen(tNOSM->Text) > 0)
		setTagValue(listFix, "58", tNOSM->Text);

	setTagValue(listFix, "77", tNOSM->OpenClose);
	
	ret = FixEncode::fixEncodeMsg(listFix, "D", msgSeqNum);
	TRACE_LOG("msg NewOrderSingle encode : %s", ret.c_str());
	return ret;
	
}

string FixDealer::FixOrderCancel(TOrderCancelMsg *tOCMsg, long msgSeqNum)
{
	string ret;
	char szTemp[32];
	memset(szTemp, 0, sizeof(szTemp));
	LISTFIXMSG listFix;

	setTagValue(listFix, "11", tOCMsg->CIOrdID);

	if (tOCMsg->OrderID > 0)
	{
		sprintf(szTemp, "%d", tOCMsg->OrderID);
		setTagValue(listFix, "37",  szTemp);
	}

	setTagValue(listFix, "41", tOCMsg->OrigCIOrdID);

	sprintf(szTemp, "%s", g_account.c_str());
	setTagValue(listFix, "1",  szTemp);

	setTagValue(listFix, "200", tOCMsg->MaturityMonthYear);

	setTagValue(listFix, "167", tOCMsg->SecurityType);

	setTagValue(listFix, "207", tOCMsg->SecurityExchange);

	sprintf(szTemp, "%d", tOCMsg->Side);
	setTagValue(listFix, "54", szTemp);

	sprintf(szTemp, "%s", FixEncode::generateCurtime().c_str());
	setTagValue(listFix, "60", szTemp);

	sprintf(szTemp, "%d", tOCMsg->OrderQty);
	setTagValue(listFix, "38", szTemp);
	
	ret = FixEncode::fixEncodeMsg(listFix, "F", msgSeqNum);
	TRACE_LOG("msg OrderCancel encode : %s", ret.c_str());
	return ret;
}

void FixDealer::setTagValue(LISTFIXMSG &listFix, char *sztag, char * szvalue)
{
	TFIXMSG tFix;
	tFix.msg_tag = sztag;
	tFix.msg_value = szvalue;
	listFix.push_back(tFix);
}



