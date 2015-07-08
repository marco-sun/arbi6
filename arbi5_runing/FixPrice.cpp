// FixPrice.cpp: implementation of the FixPrice class.
//
//////////////////////////////////////////////////////////////////////
#pragma   warning   (disable:   4786) 

#include "FixPrice.h"
#include "LogStore.h"
#include <process.h>

FixPrice::FixPrice()
{

}

FixPrice::~FixPrice()
{

}

string FixPrice::FixUserLogin(TLogonMsg *tLOnM, long msgSeqNum)
{
	string ret;

	char szTemp[32];
	memset(szTemp, 0, sizeof(szTemp));
	LISTFIXMSG listFix;

	sprintf(szTemp, "%d", tLOnM->EncryptMethod);
	setTagValue(listFix, "98", szTemp);

	sprintf(szTemp, "%d", tLOnM->HeartBtInt);
	setTagValue(listFix, "108", szTemp);

	sprintf(szTemp, "%d", tLOnM->RawDataLength);
	setTagValue(listFix, "95", szTemp);

	setTagValue(listFix, "96", tLOnM->RawData);

	if (msgSeqNum == 1)
		setTagValue(listFix, "141", "Y");
	else
		setTagValue(listFix, "141", "N");

	ret = FixEncode::fixEncodeMsg(listFix, "A", msgSeqNum);
	TRACE_LOG("msg login encode : %s", ret.c_str());
	return ret;
	
}


string FixPrice::FixUserLogout(TLogoutMsg *tLOut, long msgSeqNum)
{
	string ret;

	char szTemp[32];
	memset(szTemp, 0, sizeof(szTemp));
	LISTFIXMSG listFix;

	sprintf(szTemp, "%d", tLOut->Text);
	setTagValue(listFix, "58", szTemp);

	ret = FixEncode::fixEncodeMsg(listFix, "5", msgSeqNum);
	TRACE_LOG("msg logout encode : %s", ret.c_str());
	return ret;

}
string FixPrice::FixResendSeq(TResendSeqMsg *tRS, long msgSeqNum)
{
	string ret;

	char szTemp[32];
	memset(szTemp, 0, sizeof(szTemp));
	LISTFIXMSG listFix;

	sprintf(szTemp, "%d", tRS->beginNo);
	setTagValue(listFix, "7", szTemp);

	sprintf(szTemp, "%d", tRS->endNo);
	setTagValue(listFix, "16", szTemp);

	ret = FixEncode::fixEncodeMsg(listFix, "2", msgSeqNum);
	TRACE_LOG("msg Resend encode : %s", ret.c_str());
	return ret;
}

string FixPrice::FixGrapFill(TGrapFillMsg *tGF, long msgSeqNum)
{
	string ret;
	char szTemp[32];
	memset(szTemp, 0, sizeof(szTemp));
	LISTFIXMSG listFix;

	setTagValue(listFix, "123", "Y");

	sprintf(szTemp, "%d", tGF->newNo);
	setTagValue(listFix, "36", szTemp);

	ret = FixEncode::fixEncodeMsg(listFix, "4", msgSeqNum);
	TRACE_LOG("msg GrapFill encode : %s", ret.c_str());
	return ret;
}

string FixPrice::FixHB(THBMsg *tHB, long msgSeqNum)
{
	string ret;

	char szTemp[32];
	memset(szTemp, 0, sizeof(szTemp));
	LISTFIXMSG listFix;
	if (strlen(tHB->TestReqID) > 0)
	{
		setTagValue(listFix, "112", tHB->TestReqID);
	}
	ret = FixEncode::fixEncodeMsg(listFix, "0", msgSeqNum);

	return ret;
}

string FixPrice::FixMDReq(TMDReqMsg *tMDReq, long msgSeqNum)
{
	string ret;

	char szTemp[32];
	memset(szTemp, 0, sizeof(szTemp));
	LISTFIXMSG listFix;
	setTagValue(listFix, "262", tMDReq->MDReqID);
	
	sprintf(szTemp, "%d", tMDReq->SubscriptionRequestType);
	setTagValue(listFix, "263", szTemp);

	sprintf(szTemp, "%d", tMDReq->MarketDepth);
	setTagValue(listFix, "264", szTemp);

	sprintf(szTemp, "%d", tMDReq->MDUpdateType);
	setTagValue(listFix, "265", szTemp);

	sprintf(szTemp, "%d", tMDReq->NoMDEntryTypes);
	setTagValue(listFix, "267", szTemp);

	int i = 0;
	for (i = 0; i < tMDReq->NoMDEntryTypes; i++)
	{
		sprintf(szTemp, "%d", tMDReq->MDEntryType[i]);
		setTagValue(listFix, "269", szTemp);
		if(i >= 8)
			break;
	}

	setTagValue(listFix, "146", "1");// NoRelatesSym

	setTagValue(listFix, "55", tMDReq->Symbol);

	setTagValue(listFix, "167", tMDReq->SecurityType);

	setTagValue(listFix, "200", tMDReq->MaturityMonthYear);

	setTagValue(listFix, "207", tMDReq->SecurityExchange);

	ret = FixEncode::fixEncodeMsg(listFix, "V", msgSeqNum);

	return ret;

}

void FixPrice::setTagValue(LISTFIXMSG &listFix, char *sztag, char * szvalue)
{
	TFIXMSG tFix;
	tFix.msg_tag = sztag;
	tFix.msg_value = szvalue;
	listFix.push_back(tFix);
}



