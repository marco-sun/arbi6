// FixPrice.h: interface for the FixPrice class.

#if !defined FIXPRICE_H
#define FIXPRICE_H

#pragma once

#include <windows.h>
#include "FixEncode.h"
using namespace std;

class FixPrice
{
public:
	FixPrice();
	virtual ~FixPrice();

	static string FixUserLogout(TLogoutMsg *tLOnM, long msgSeqNum);
	static string FixUserLogin(TLogonMsg *tLOnM, long msgSeqNum);
	static string FixMDReq(TMDReqMsg *tMDReq, long msgSeqNum);
	static string FixHB(THBMsg *tHB, long m_msgSeqNum);
	static string FixResendSeq(TResendSeqMsg *tRS, long msgSeqNum);
	static string FixGrapFill(TGrapFillMsg *tGF, long msgSeqNum);
private:
	static void setTagValue(LISTFIXMSG &listFix, char *sztag, char * szvalue);
};


#endif 
