// FixDealer.h: interface for the FixDealer class.
#if !defined FixDealer_H
#define FixDealer_H

#pragma once
#include <list>
#include <map>
#include <string>
using namespace std;
#include "FixEncode.h"

class FixDecode //: public Dealer
{
public:
	FixDecode();
	~FixDecode();

	static LISTFIXMSGALL fixDecodeMsg(string sbody);

	static string fixDecodeMsg2Txt(string sbody);
protected:
	static TFIXMSG fix_decode_tag_value(string sparts);
	static string fix_get_taginfo(string stag);
};

#endif 
