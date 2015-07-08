// FixDealer.h: interface for the FixDealer class.

#if !defined FIXDEALER_H
#define FIXDEALER_H

#pragma once

#include <windows.h>
#include "FixEncode.h"
using namespace std;

class FixDealer
{
public:
	FixDealer();
	virtual ~FixDealer();

	static string FixNewOrderSingle(TNewOrderSingleMsg *tNOSM, long msgSeqNum);
	static string FixOrderCancel(TOrderCancelMsg *tOCMsg, long msgSeqNum);

private:
	static void setTagValue(LISTFIXMSG &listFix, char *sztag, char * szvalue);
};


#endif 
